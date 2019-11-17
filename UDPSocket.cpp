#include "UDPSocket.h"

UDPSocket :: UDPSocket ()
{   
}

bool UDPSocket ::initializeSocket (char * _myAddr, unsigned int _myPort)
{ //TODO: Get socket primary IP

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock<0)
    {
        perror("Initializing socket of server failed");
        return false;
    }
    int enableReuse = 1;
    int n = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &enableReuse, sizeof(enableReuse));
    if (n < 0)
        perror("setsockopt(SO_REUSEADDR) failed");
    this->myAddress_str = _myAddr;
    this->myAddr.sin_family    = AF_INET; // IPv4
    this->myAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    this->myAddr.sin_port = htons(_myPort);
    this->myPort = _myPort;

    n = bind(sock, (struct sockaddr *)&myAddr, sizeof(struct sockaddr_in));

    if(n!= 0)
    {
        perror("Bind of server socket failed\n");
        close(sock);
        return false;
    }
    this->ReceiveThread = new thread(&UDPSocket::receiveHandler,this,this);
    this->SendThread = new thread(&UDPSocket::sendingHandler,this,this);  
    return true; 
}

int UDPSocket ::writeToSocket (char * buffer,  int maxBytes )
{
    int n ;
    if( (n = sendto(sock, buffer, strlen(buffer), 0,(sockaddr*) &peerAddr,sizeof(peerAddr))) < 0)
    {
        printf("Send from client failed\n");
    }
    return n;
}

int UDPSocket ::writeToSocketToAddr (char * buffer,  int maxBytes ,struct sockaddr_in targetAddr){
    int n ;
    if( (n = sendto(sock, buffer, strlen(buffer), 0,(sockaddr*) &targetAddr,sizeof(targetAddr))) < 0)
    {
        printf("Send from client failed\n");
    }
    return n;
}

int UDPSocket ::readFromSocketWithBlock (char * buffer,  int maxBytes )
{
    socklen_t aLength;
    aLength = sizeof(peerAddr);

    int n;
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

    if((n = recvfrom(sock, buffer,  maxBytes, 0, (struct sockaddr*)&peerAddr, &aLength))<0){
        perror("Receive 1") ;
    }
    else
    {
        buffer[n]='\0';
        //printf("%s \n", buffer);
    }
    return n;
}

int UDPSocket ::readFromSocketWithTimeout (char * buffer, int maxBytes, struct timeval tv)
{
    socklen_t aLength;
    aLength = sizeof(peerAddr);

    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

    int n; 
    if((n = recvfrom(sock, buffer,  maxBytes, 0, (struct sockaddr*)&peerAddr, &aLength))<0){
        perror("Receive 1") ;
    }
    else
    {
        buffer[n]='\0';
        //printf("%s \n", buffer);
    }

    return n;
}

Message * UDPSocket::receiveMsg()
{
        ReceiveBufferMtx.lock();
        Message * m = ReceiveBuffer.front();
        ReceiveBuffer.pop();
        ReceiveBufferMtx.unlock();
        return m;
}

bool UDPSocket::sendMessage(Message * FullMessage)
{
    //vector <Message *> fragments;
    //cout << "UDPSocket::sendMessage before fragementMsg" << endl;
    //this->fragmentMsg(FullMessage, fragments);
    //cout << "Done fragmenting" << endl;
    //for(auto x: fragments)
    //{
    //    cout << "frag loop send" << endl;

    SendBufferMtx.lock();
    SendBuffer.push(FullMessage);
    SendBufferMtx.unlock();
    //string serializedMsg = x->marshal();
    //char * serializedMsgPtr = new char [serializedMsg.size()];
    //strcpy(serializedMsgPtr, serializedMsg.c_str());
    //writeToSocket(serializedMsgPtr, serializedMsg.size());
    //}
}
string UDPSocket::getMsgID(Message* message)
{
    string ID = message->getSourceIP() + to_string(message->getRPCId());
    return ID;
}
void UDPSocket::receiveHandler(UDPSocket* myUDPSocket)
{
    cout << "Thread receive" << endl;
    char * buffer = new char [MAX_BUFFER_SIZE];
    //<msg id, <fragTotal,msg>>
    unordered_map <string ,pair <int, vector<Message *>> > Map;

    struct sockaddr_in from;
    socklen_t fromlen = sizeof(from);
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;


    setsockopt(myUDPSocket->sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

    while(true)
    {

        int n = recvfrom(sock, buffer,  MAX_BUFFER_SIZE, 0, (struct sockaddr*)&from, &fromlen);

        if(n<0)
            printf("ReceiveHandler: receive failed");


        Message * currMessage = new Message(buffer);

        string msgID = myUDPSocket->getMsgID(currMessage);


        //if first msg from this fragment set, create frags vector
        if(Map.find(msgID) == Map.end())
        {

            vector<Message *> frags;
            frags.resize(currMessage->getFragmentTotal());

            for(int i = 0; i < frags.size(); i++) frags[i] = NULL;
            //first fragment, frag count = 0;
            Map[msgID] = pair<int,vector<Message *>>(0,frags);
        }

        //build frags chain horizontally in map
        if(((Map[msgID]).second)[currMessage->getFragmentCount()] == NULL)
        {

            ((Map[msgID]).second)[currMessage->getFragmentCount()] = currMessage;
            //increment frag count
            (Map[msgID]).first++;
        }
        //if fragsCount == frags size = fragTotal, defragment
        if((Map[msgID]).first == (Map[msgID]).second.size())
        {

            string MsgStr((char *)(Map[msgID]).second[0]->getMessage());

            Message * fullMsg = (Map[msgID]).second[0];
            for(int i = 1; i < fullMsg->getFragmentTotal(); i++)
            {
                MsgStr += string((char *)(Map[msgID]).second[i]->getMessage());
            }
            char * c = new char[MsgStr.size()+1];
            strcpy(c, MsgStr.c_str());
            fullMsg->setMessage(c, MsgStr.size()+1);

        
            (myUDPSocket->ReceiveBufferMtx).lock();
            (myUDPSocket->ReceiveBuffer).push(fullMsg);
            (myUDPSocket->ReceiveBufferMtx).unlock();
        }
    }

}





int UDPSocket ::getMyPort ()
{
    return this->myPort;
}
string UDPSocket::getMyIP()
{
    return this->myAddress_str;
}

void UDPSocket::fragmentMsg(Message * FullMessage, vector<Message *> &frags)
{
    string MessageWithoutHeader = string(FullMessage->getMessage());
    cout << "msg size" << FullMessage->getMessageSize() << endl;
    unsigned int NumberOfFrags = ceil((float)FullMessage->getMessageSize()/(FRAG_MSG_SIZE));
    cout << "Number of fargs " << NumberOfFrags << endl;
    cout << "msg without header" << MessageWithoutHeader << endl;

    vector<string> subMessagesWithoutHeader;

    for(int i=0;i<NumberOfFrags; i++)
    {
            subMessagesWithoutHeader.push_back(MessageWithoutHeader.substr(i*FRAG_MSG_SIZE, FRAG_MSG_SIZE));

    }

    for(unsigned int i = 1; i<=NumberOfFrags; i++)
    {
        Message * fragi = new Message;
        char * s = new char[subMessagesWithoutHeader[i-1].size()+1];
        strcpy(s, subMessagesWithoutHeader[i-1].c_str());
        fragi->setDestinationIP(FullMessage->getDestinationIP());
        fragi->setFragState(i, NumberOfFrags);
        fragi->setMessage(s);
        fragi->setMessageSize(subMessagesWithoutHeader[i-1].size());
        cout << "sub msg without header " << s<<endl;
        fragi->setSourceIP(FullMessage->getSourceIP());
        fragi->setSourcePort(FullMessage->getSourcePort());
        fragi->setDestinationIP(FullMessage->getDestinationIP());
        fragi->setDestinationPort(FullMessage->getDestinationPort());
        fragi->setRPCID(FullMessage->getRPCId());
        fragi->setMessageType(FullMessage->getMessageType());
        fragi->setOperation(FullMessage->getOperation());
        frags.push_back(fragi);
    }
}


void UDPSocket::sendingHandler(UDPSocket* myUDPSocket)
{
    cout << "Thread send" << endl;
    vector<Message *> fragments;

    while(true)
    {
        if(SendBuffer.size() >0)
        {
            cout << "I am in sending handler" << endl;
            myUDPSocket->SendBufferMtx.lock(); //lock      
            Message* topMsg = (myUDPSocket->SendBuffer).front(); //front
            (myUDPSocket->SendBuffer).pop(); //pop
            SendBufferMtx.unlock(); //unlock

            //set source field
            topMsg->setSourceIP(string(myUDPSocket->myAddress_str));
            topMsg->setSourcePort(myUDPSocket->getMyPort());
            string destIP = (topMsg->getDestinationIP());
            //fragment
            fragmentMsg(topMsg, fragments);

            //generate destAddr
            struct sockaddr_in destAddr;
            memset((char*)&destAddr, 0, sizeof(destAddr));
            char *meh = new char [destIP.size()];
            strcpy(meh, destIP.c_str());
            inet_aton(meh, &destAddr.sin_addr);
            destAddr.sin_family = AF_INET;
            destAddr.sin_addr.s_addr = htonl(INADDR_ANY);
            destAddr.sin_port = htons(topMsg->getDestinationPort());
        
            for(int i=0; i<fragments.size(); i++)
            {
                string msgStr = fragments[i]->marshal();
                char *msgPtr = new char [msgStr.size()];
                strcpy(msgPtr, msgStr.c_str());
                //TO DO replace with write to socket
                int n = sendto(myUDPSocket->sock, msgPtr, strlen(msgPtr), 0,(sockaddr*) &destAddr,sizeof(destAddr));
            }   
            cout << "done  sending" << endl;

            for(int i = 0; i <fragments.size();i++)
                delete fragments[i];
        fragments.clear();
        }

    }
}


// Message * UDPSocket::defragment(vector<Message *> frags)
// {
//     /*for(int i=0; i<frags.size(); i++)
//     {
//          s += string((char *)frags[i]->getMessage());
//     }
//     char * msgPtr = new char [s.size()];
//     strcpy(msgPtr, s);
//     Message * defragmented = frags[frags.size()-1];
//     defragmented->setMessage(msgPtr);
//     return defragmented;   */

// }




UDPSocket :: ~UDPSocket ( ){
    close(sock);
}