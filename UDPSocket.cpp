#include "UDPSocket.h"

UDPSocket :: UDPSocket ()
{   
}
void UDPSocket :: setBroadcast(int s)
{
    int arg;
    #ifdef  SO_BROADCAST
    arg = 1;
    if(setsockopt(s, SOL_SOCKET, SO_BROADCAST, &arg, sizeof(arg)) <0)
    {
        printf("setsockopt  SO_BROADCAST---");
        exit(-1);
    }
    #endif
}
char * UDPSocket::getMachineIP()
{
    const char* google_dns_server = "8.8.8.8";
    int dns_port = 53;
    struct sockaddr_in serv;
    int tempSock = socket(AF_INET, SOCK_DGRAM, 0);
    if(tempSock < 0)
    {
        std::cout << "Socket error" << std::endl;
    }
    memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr(google_dns_server);
    serv.sin_port = htons(dns_port);

    int err = connect(tempSock, (const struct sockaddr*)&serv, sizeof(serv));
        if (err < 0)
    {
        std::cout << "Error number: " << errno
            << ". Error message: " << strerror(errno) << std::endl;
    }
    struct sockaddr_in name;
    socklen_t namelen = sizeof(name);
    err = getsockname(tempSock, (struct sockaddr*)&name, &namelen);
    char * buff = new char[80];
    const char* p = inet_ntop(AF_INET, &name.sin_addr, buff, 80);
    close(tempSock);
    return buff;
}
bool UDPSocket ::initializeSocket(char * _myAddr, unsigned int _myPort)
{ 

    this->sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(this->sock<0)
    {
        perror("Initializing socket of server failed");
        return false;
    }
    setBroadcast(this->sock);

    int enableReuse = 1;
    int n = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &enableReuse, sizeof(enableReuse));
    if (n < 0)
        perror("setsockopt(SO_REUSEADDR) failed");

    this->myAddress_str = _myAddr;
    this->myPort = _myPort;
    this->myAddr.sin_family    = AF_INET; // IPv4
    this->myAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    this->myAddr.sin_port = htons(_myPort);

    n = bind(sock, (struct sockaddr *)&myAddr, sizeof(struct sockaddr_in));

    if(n<0)
    {
        perror("Bind of server socket failed\n");
        close(sock);
        return false;
    }

    this->ReceiveThread = new thread(&UDPSocket::receiveHandler,this, this);
    this->SendThread = new thread(&UDPSocket::sendingHandler,this, this);  

    return true; 
}
bool UDPSocket ::initializeSocket(unsigned int _myPort)
{
      //TODO: Get socket primary IP
    char * machineIP = new char[90];
    machineIP = getMachineIP();
    this->sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(this->sock<0)
    {
        perror("Initializing socket of server failed");
        return false;
    }
    setBroadcast(this->sock);

    int enableReuse = 1;
    int n = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &enableReuse, sizeof(enableReuse));
    if (n < 0)
        perror("setsockopt(SO_REUSEADDR) failed");

    this->myAddress_str = string(machineIP);
    this->myPort = _myPort;
    this->myAddr.sin_family    = AF_INET; // IPv4
    this->myAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    this->myAddr.sin_port = htons(_myPort);

    n = bind(sock, (struct sockaddr *)&myAddr, sizeof(struct sockaddr_in));

    if(n<0)
    {
        perror("Bind of server socket failed\n");
        close(sock);
        return false;
    }

    this->ReceiveThread = new thread(&UDPSocket::receiveHandler,this, this);
    this->SendThread = new thread(&UDPSocket::sendingHandler,this, this);  

    return true; 
}
void UDPSocket::setMyPort(unsigned int _myPort)
{
    this->myPort = _myPort;
}

int UDPSocket ::getMyPort()
{
    return this->myPort;
}

string UDPSocket::getMyIP()
{
    return this->myAddress_str;
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

void UDPSocket::fragmentMsg(Message * FullMessage, vector<Message *> &frags)
{
    string MessageWithoutHeader = "";
    char * msgPtrWithoutHeader = new char [FullMessage->getMessageSize()];
    msgPtrWithoutHeader = FullMessage->getMessage();
    int size= FullMessage->getMessageSize();
    int i=0;
    while(size--)
    MessageWithoutHeader+=msgPtrWithoutHeader[i++];

    unsigned int NumberOfFrags = (FullMessage->getMessageSize()-1)/(FRAG_MSG_SIZE)+1;

    vector<string> subMessagesWithoutHeader;
    for(int i=0;i<NumberOfFrags;i++)
    {
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
        fragi->setSourceIP(FullMessage->getSourceIP());
        fragi->setSourcePort(FullMessage->getSourcePort());
        fragi->setDestinationIP(FullMessage->getDestinationIP());
        fragi->setDestinationPort(FullMessage->getDestinationPort());
        fragi->setRPCID(FullMessage->getRPCId());
        fragi->setMessageType(FullMessage->getMessageType());
        fragi->setOperation(FullMessage->getOperation());
        frags.push_back(fragi);
    }

    #ifdef DEBUG
    #endif
}

string UDPSocket::getMsgID(Message* message)
{
    string ID = message->getSourceIP() +to_string(message->getSourcePort()) +to_string(message->getRPCId());
    return ID;
}
void UDPSocket::receiveHandler(UDPSocket * myUDPSocket)
{

    char * buffer = new char [myUDPSocket->SOCK_MAX_BUFFER_SIZE];

    //msg ID {sourceIP + RPCid}
    unordered_map <string ,pair <int, vector<Message *>> > Map;

    struct sockaddr_in from;
    socklen_t fromlen = sizeof(from);
    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    //Blocking recv
    setsockopt(myUDPSocket->sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
    
    #ifdef RECEIVE_OUTPUT_FILE_LOG
    (myUDPSocket->outFile).open ("receivedStuff.txt");
    #endif 
    

    while(!dest)
    {
        int bytesAtPort = recvfrom(sock, buffer, myUDPSocket->SOCK_MAX_BUFFER_SIZE,0, (struct sockaddr*)&from, &fromlen);

        if(bytesAtPort<0){
            myUDPSocket->outFile<<"ReceiveHandler: receive failed";
            continue;
        }

        Message * currMessage = new Message(buffer);
        string msgID = myUDPSocket->getMsgID(currMessage);

        //First fragment
        if(Map.find(msgID) == Map.end())
        {
            vector<Message *> frags;
            frags.resize(currMessage->getFragmentTotal());

            for(int i = 0; i < frags.size(); i++) frags[i] = NULL;
            Map[msgID] = pair<int,vector<Message *>>(0,frags);
        }

        if(((Map[msgID]).second)[currMessage->getFragmentCount()-1] == NULL)
        {
            ((Map[msgID]).second)[currMessage->getFragmentCount()-1] = currMessage;
            (Map[msgID]).first++;
        }

        if((Map[msgID]).first == (Map[msgID]).second.size())
        {
            string MsgStr((char *)(Map[msgID]).second[0]->getMessage());
            Message * fullMsg = (Map[msgID]).second[0];

            //Concatenate Message field of all fragments
            for(int i = 1; i < fullMsg->getFragmentTotal(); i++)
                MsgStr += string((char *)(Map[msgID]).second[i]->getMessage());

            #ifdef RECEIVE_OUTPUT_FILE_LOG
            myUDPSocket->outFile <<  MsgStr;
            #endif

            char * cStrsAreMeh = new char[MsgStr.size()+1];
            strcpy(cStrsAreMeh, MsgStr.c_str());
            fullMsg->setMessage(cStrsAreMeh, MsgStr.size());
            (myUDPSocket->ReceiveBufferMtx).lock();
            (myUDPSocket->ReceiveBuffer).push(fullMsg);
            (myUDPSocket->ReceiveBufferMtx).unlock();
        }
    }

}


void UDPSocket::sendingHandler(UDPSocket * myUDPSocket)
{
    vector<Message *> fragments;
    while(!dest)
    {
        if(SendBuffer.size())
        {
            myUDPSocket->SendBufferMtx.lock();     
            Message* topMsg = (myUDPSocket->SendBuffer).front();
            (myUDPSocket->SendBuffer).pop();
            SendBufferMtx.unlock();


            topMsg->setSourceIP(string(myUDPSocket->myAddress_str));
            topMsg->setSourcePort(myUDPSocket->getMyPort());


            fragmentMsg(topMsg, fragments);

            struct sockaddr_in destAddr;
            memset((char*)&destAddr, 0, sizeof(destAddr));
            string destIP = (topMsg->getDestinationIP());
            char *meh = new char [destIP.size()+1];
            strcpy(meh, destIP.c_str());
            //inet_aton(meh, &destAddr.sin_addr);
            //destAddr.sin_addr.s_addr = htonl(INADDR_ANY);
            //destAddr.sin_port = htons(topMsg->getDestinationPort());

            struct hostent *host;
            destAddr.sin_family  =  AF_INET;
            if((host = gethostbyname(meh))== (void*)(0))
            {
                printf("Unknown host name\n");
                exit(-1);
            }
            destAddr.sin_addr = *(struct in_addr *) (host->h_addr_list[0]);
            destAddr.sin_port = htons(topMsg->getDestinationPort());
        
            for(int i=0; i<fragments.size(); i++)
            {
                string msgStr = fragments[i]->marshal();
                char *msgPtr = new char [msgStr.size()+1];
                strcpy(msgPtr, msgStr.c_str());
                int n = sendto(myUDPSocket->sock, msgPtr, strlen(msgPtr), 0,(sockaddr*) &destAddr,sizeof(destAddr));
                usleep(1000);
            }  
            #ifdef DEBUG 
            #endif

            for(int i = 0; i <fragments.size();i++)
                delete fragments[i];
        fragments.clear();
        }

    }
}

UDPSocket :: ~UDPSocket ( ){
    dest = true;
    ReceiveThread->join();
    SendThread->join();
    close(sock);
}