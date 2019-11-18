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
    cout << "Before threads BS" << endl;
    this->ReceiveThread = new thread(&UDPSocket::receiveHandler,this, this);
    this->SendThread = new thread(&UDPSocket::sendingHandler,this, this);  
    return true; 
}

Message * UDPSocket::receiveMsg()
{
    while(ReceiveBuffer.empty());
        ReceiveBufferMtx.lock();
        Message * m = ReceiveBuffer.front();
        ReceiveBuffer.pop();
        ReceiveBufferMtx.unlock();
        return m;
}

bool UDPSocket::sendMessage(Message * FullMessage)
{
    SendBufferMtx.lock();
    SendBuffer.push(FullMessage);
    SendBufferMtx.unlock();
    return true;
}
string UDPSocket::getMsgID(Message* message)
{
    string ID = message->getSourceIP() + to_string(message->getRPCId());
    return ID;
}
void UDPSocket::receiveHandler(UDPSocket * myUDPSocket)
{
    cout << "Thread receiveHandler" << endl;

    char * buffer = new char [MAX_BUFFER_SIZE];
    //msg ID {sourceIP + RPCid} 
    unordered_map <string ,pair <int, vector<Message *>> > Map;

    struct sockaddr_in from;
    socklen_t fromlen = sizeof(from);
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    //blocking recv
    setsockopt(myUDPSocket->sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
    cout << "Thread receive after opt" << endl;

    //to write to file
    ofstream myfile;
    myfile.open ("receivedStuff.txt");
    

    while(true)
    {
        int n = recvfrom(sock, buffer, MAX_BUFFER_SIZE,0, (struct sockaddr*)&from, &fromlen);
        cout << "N = " << n << endl;
        if(n<0)
            printf("ReceiveHandler: receive failed");

        Message * currMessage = new Message(buffer);
        //cout << "currMessage = "  << currMessage->getMessage() << endl;
        cout << "currMessage size =" << currMessage->getMessageSize() << endl;
        string msgID = myUDPSocket->getMsgID(currMessage);


        //if first msg from this fragment set, create frags vector
        if(Map.find(msgID) == Map.end())
        {
            cout << "First fragment of msg with ID. " << msgID << endl;
            vector<Message *> frags;
            cout<<"Total Number of Fragments to be received is "<<currMessage->getFragmentTotal()<<endl;
            frags.resize(currMessage->getFragmentTotal());
            for(int i = 0; i < frags.size(); i++) frags[i] = NULL;
            //first fragment, frag count = 0;
            Map[msgID] = pair<int,vector<Message *>>(0,frags);
        }

        //build frags chain horizontally in map
        if(((Map[msgID]).second)[currMessage->getFragmentCount()-1] == NULL)
        {
            cout << "Building frags";
            ((Map[msgID]).second)[currMessage->getFragmentCount()-1] = currMessage;
            //increment frag count
            cout << (Map[msgID]).first++ << endl;
        }
        //if fragsCount == frags size = fragTotal, defragment
        if((Map[msgID]).first == (Map[msgID]).second.size())
        {
            cout << "Last fragment fragC = " << (Map[msgID]).first << " total = "<< (Map[msgID]).second.size() << endl;

            string MsgStr((char *)(Map[msgID]).second[0]->getMessage());

            Message * fullMsg = (Map[msgID]).second[0];
            cout << "Start of msg concatenation= " << endl;
            for(int i = 1; i < fullMsg->getFragmentTotal(); i++)
            {
                MsgStr += string((char *)(Map[msgID]).second[i]->getMessage());
            }
            //cout << "Full message " << MsgStr<<endl;
            cout << "Full messgae size after concatenation " << MsgStr.size() << endl;
            myfile <<  MsgStr;
            char * c = new char[MsgStr.size()+1];
            strcpy(c, MsgStr.c_str());
            fullMsg->setMessage(c, MsgStr.size());

        
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
    cout<<"before string"<<endl;
    string MessageWithoutHeader = "";
    char * msgPtrWithoutHeader = new char [FullMessage->getMessageSize()];
    msgPtrWithoutHeader = FullMessage->getMessage();
    int size= FullMessage->getMessageSize();
    int i=0;
    while(size--)
    MessageWithoutHeader+=msgPtrWithoutHeader[i++];

    cout << "fragmentMsg: Full Message size" << MessageWithoutHeader.size() << endl;
    unsigned int NumberOfFrags = ceil((float)FullMessage->getMessageSize()/(FRAG_MSG_SIZE));
    cout << "fragmentMsg: Number of fargs= " << NumberOfFrags << endl;
    //cout << "msg without header" << MessageWithoutHeader << endl;
    vector<string> subMessagesWithoutHeader;
    cout << "Before frag loop1" << endl;

    for(int i=0;i<NumberOfFrags; i++)
    {
        cout << "i" <<i<< endl;
        if(i == NumberOfFrags-1)
            subMessagesWithoutHeader.push_back(MessageWithoutHeader.substr(i*FRAG_MSG_SIZE, MessageWithoutHeader.size()-(i-1)*FRAG_MSG_SIZE));         
        else        
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
        //cout << "sub msg without header " << s<<endl;
        fragi->setSourceIP(FullMessage->getSourceIP());
        fragi->setSourcePort(FullMessage->getSourcePort());
        fragi->setDestinationIP(FullMessage->getDestinationIP());
        fragi->setDestinationPort(FullMessage->getDestinationPort());
        fragi->setRPCID(FullMessage->getRPCId());
        fragi->setMessageType(FullMessage->getMessageType());
        fragi->setOperation(FullMessage->getOperation());
        frags.push_back(fragi);
    }
    cout << "Fragmentation End" << endl;
}


void UDPSocket::sendingHandler(UDPSocket * myUDPSocket)
{
    cout << "Thread send meh" << endl;
    vector<Message *> fragments;

    while(true)
    {
        if(SendBuffer.size() >0)
        {
            cout << "Send Buffer Size: "<<SendBuffer.size() << endl;
            myUDPSocket->SendBufferMtx.lock(); //lock      
            Message* topMsg = (myUDPSocket->SendBuffer).front(); //front
            (myUDPSocket->SendBuffer).pop(); //pop
            SendBufferMtx.unlock(); //unlock

            //set source field
            topMsg->setSourceIP(string(myUDPSocket->myAddress_str));
            topMsg->setSourcePort(myUDPSocket->getMyPort());
            string destIP = (topMsg->getDestinationIP());
            cout << "Before frag func" << endl;
            //fragment
            fragmentMsg(topMsg, fragments);

            //generate destAddr
            struct sockaddr_in destAddr;
            memset((char*)&destAddr, 0, sizeof(destAddr));
            char *meh = new char [destIP.size()+1];
            strcpy(meh, destIP.c_str());
            inet_aton(meh, &destAddr.sin_addr);
            destAddr.sin_family = AF_INET;
            destAddr.sin_addr.s_addr = htonl(INADDR_ANY);
            destAddr.sin_port = htons(topMsg->getDestinationPort());
        
            for(int i=0; i<fragments.size(); i++)
            {
                string msgStr = fragments[i]->marshal();
                //cout <<  "Sender fragement " << i  << " " << msgStr <<  " of size " << msgStr.size() <<endl;
                char *msgPtr = new char [msgStr.size()+1];
                strcpy(msgPtr, msgStr.c_str());
                //TO DO replace with write to socket
                int n = sendto(myUDPSocket->sock, msgPtr, strlen(msgPtr), 0,(sockaddr*) &destAddr,sizeof(destAddr));
                usleep(1000);
            }   
            cout << "done  sending" << endl;

            for(int i = 0; i <fragments.size();i++)
                delete fragments[i];
        fragments.clear();
        }

    }
}

UDPSocket :: ~UDPSocket ( ){
    close(sock);
}