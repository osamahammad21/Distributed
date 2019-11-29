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
    this->FaultThread = new thread(&UDPSocket::faultToleranceHandler,this, this);  


    return true; 
}
bool UDPSocket ::initializeSocket(unsigned int _myPort)
{
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
    this->FaultThread = new thread(&UDPSocket::faultToleranceHandler,this, this);  


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

        if(FullMessage->getMessageType() == MessageType::Ack)
        {
            fragi->setFragState(FullMessage->getFragmentCount(), NumberOfFrags);
            // string fragid = this->getAckFragmentID(fragi);
            // char * ackContent = new char[fragid.size()];
            // cout << "fragment function frag id " << ackContent << endl;
            // strcpy(ackContent, fragid.c_str());
            // fragi->setMessage(ackContent);
            // fragi->setMessageSize(fragid.size());
            fragi->setMessage(s);
            fragi->setMessageSize(FullMessage->getMessageSize());
        }
        else
        {  
            fragi->setMessage(s);
            fragi->setMessageSize(subMessagesWithoutHeader[i-1].size());

        }
        
        fragi->setSourceIP(FullMessage->getSourceIP());
        fragi->setSourcePort(FullMessage->getSourcePort());
        fragi->setDestinationIP(FullMessage->getDestinationIP());
        fragi->setMessageTimestamp(FullMessage->getMessageTimestamp());
        fragi->setDestinationPort(FullMessage->getDestinationPort());
        fragi->setRPCID(FullMessage->getRPCId());
        fragi->setMessageType(FullMessage->getMessageType());
        fragi->setOperation(FullMessage->getOperation());
        fragi->setMessageTimestamp(FullMessage->getMessageTimestamp());
        frags.push_back(fragi);
    }

    #ifdef DEBUG
    #endif
}

string UDPSocket::getMsgID(Message* message)
{
    string ID = message->getSourceIP() +to_string(message->getSourcePort()) +to_string(message->getRPCId()) + to_string(message->getMessageTimestamp());
    return ID;
}
string UDPSocket::getFragmentID(Message* message)
{
    string ID = getMsgID(message) +to_string(message->getFragmentCount());
    return ID;
}
string UDPSocket::getAckFragmentID(Message* message)
{
    string ID = message->getDestinationIP() +to_string(message->getDestinationPort()) +to_string(message->getRPCId()) + to_string(message->getMessageTimestamp())+to_string(message->getFragmentCount());
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
    tv.tv_sec = 0;
    tv.tv_usec = 0;

    //Blocking recv
    setsockopt(myUDPSocket->sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
    
    #ifdef RECEIVE_OUTPUT_FILE_LOG
    (myUDPSocket->outFile).open ("receivedStuff.txt");
    #endif 
    

    while(!dest)
    {
        int bytesAtPort = recvfrom(sock, buffer, myUDPSocket->SOCK_MAX_BUFFER_SIZE,0, (struct sockaddr*)&from, &fromlen);

        if(bytesAtPort<0)
        {
            myUDPSocket->outFile<<"ReceiveHandlNonAckeder: receive failed";
            continue;
        }

        Message * currMessage = new Message(buffer);
        string msgID = myUDPSocket->getMsgID(currMessage);

        if(currMessage->getMessageType() == MessageType::Ack)
        {
            int size = currMessage->getMessageSize();
            cout << "Message size of ack " << size << endl;
            int i =0;
            char *c = new char [size+1];
            c = currMessage->getMessage();
            string str = "";
            while(size--)
            str += c[i++];
            string ackidddd = myUDPSocket->getAckFragmentID(currMessage);


            myUDPSocket->NonAckedMtx.lock();
            if(NonAcked.end() != NonAcked.find(ackidddd))
            {
                myUDPSocket->NonAcked.erase(ackidddd);
                cout << "Received ACK of frag ID " << ackidddd << endl;
                cout << "Erasing ACK of frag ID " << ackidddd << endl;

            }
            myUDPSocket->NonAckedMtx.unlock();

            continue;

        }
        else
        {
            Message * AckMsg = currMessage;
            string id = myUDPSocket->getFragmentID(currMessage);
            char * ackID = new char [id.size()+1];
            strcpy(ackID, id.c_str());
            cout << "Sending ack of id = " << ackID << endl;
            AckMsg->setMessage(ackID);
            AckMsg->setMessageType(MessageType::Ack);
            AckMsg->setMessageSize(id.size());
            //seconds ms = duration_cast< seconds >(system_clock::now().time_since_epoch());  
            string destIP = (currMessage->getSourceIP());
            unsigned int destPort = (currMessage->getSourcePort());
            // //AckMsg->setMessageTimestamp(ms.count());
            // AckMsg->setSourceIP(string(myUDPSocket->myAddress_str));
            // AckMsg->setSourcePort(myUDPSocket->getMyPort());
            AckMsg->setDestinationIP(destIP);
            AckMsg->setDestinationPort(destPort);

            myUDPSocket->sendMessage(AckMsg);

        }
        

        //First fragment
        if(Map.find(msgID) == Map.end())
        {
            //cout << "First frag" << endl;
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
                        //cout << "last msg" << endl;

            char * cStrsAreMeh = new char[MsgStr.size()+1];
            strcpy(cStrsAreMeh, MsgStr.c_str());
            fullMsg->setMessage(cStrsAreMeh, MsgStr.size());
            (myUDPSocket->ReceiveBufferMtx).lock();
            (myUDPSocket->ReceiveBuffer).push(fullMsg);
            (myUDPSocket->ReceiveBufferMtx).unlock();
                        //cout << "last msg" << endl;

        }
    }

}

void UDPSocket::faultToleranceHandler(UDPSocket * myUDPSocket)
{
    while(!dest)
    {
        if(myUDPSocket->NonAcked.size())
        {
            
            seconds ms = duration_cast< seconds >(system_clock::now().time_since_epoch());  
            int now = ms.count();

            myUDPSocket->NonAckedMtx.lock();
            auto it = myUDPSocket->NonAcked.begin();
            myUDPSocket->NonAckedMtx.unlock();

            for(auto x: myUDPSocket->NonAcked)
            {
                myUDPSocket->NonAckedMtx.lock();
                //auto x = myUDPSocket->NonAcked.begin();
                Message *toBeResent = x.second.second;
                unsigned int trials = x.second.first;
                string fragID = x.first;
                myUDPSocket->NonAckedMtx.unlock();
                if( trials >0)
                {    
                    if(now - (toBeResent->getMessageTimestamp() + (myUDPSocket->faultTrials) - trials) >=1)
                    {

                        cout << "Resending  frag ID " << fragID << " trials " << trials << endl;
                        struct sockaddr_in destAddr;
                        memset((char*)&destAddr, 0, sizeof(destAddr));
                        string destIP = (toBeResent->getDestinationIP());
                        char *meh = new char [destIP.size()+1];
                        strcpy(meh, destIP.c_str());
                        struct hostent *host;
                        destAddr.sin_family  =  AF_INET;
                        if((host = gethostbyname(meh))== (void*)(0))
                        {
                            printf("Unknown host name\n");
                            exit(-1);
                        }
                        destAddr.sin_addr = *(struct in_addr *) (host->h_addr_list[0]);
                        destAddr.sin_port = htons(toBeResent->getDestinationPort());
                        string msgStr = toBeResent->marshal();
                        char *msgPtr = new char [msgStr.size()+1];
                        strcpy(msgPtr, msgStr.c_str());

                        myUDPSocket->sockMtx.lock();
                        int n = sendto(myUDPSocket->sock, msgPtr, strlen(msgPtr), 0,(sockaddr*) &destAddr,sizeof(destAddr));
                        myUDPSocket->sockMtx.unlock();

                        myUDPSocket->NonAckedMtx.lock();
                        myUDPSocket->NonAcked[fragID].first--;
                        myUDPSocket->NonAckedMtx.unlock();

                    }
                }
                else
                {
                    myUDPSocket->NonAckedMtx.lock();
                    myUDPSocket->NonAcked.erase(fragID);
                    myUDPSocket->NonAckedMtx.unlock();

                }
            }
            //usleep(10000);
            
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

            seconds ms = duration_cast< seconds >(system_clock::now().time_since_epoch());  

            if(topMsg->getMessageType() != MessageType::Ack)
                topMsg->setMessageTimestamp(ms.count());

            topMsg->setSourceIP(string(myUDPSocket->myAddress_str));
            topMsg->setSourcePort(myUDPSocket->getMyPort());

            fragmentMsg(topMsg, fragments);            

            struct sockaddr_in destAddr;
            memset((char*)&destAddr, 0, sizeof(destAddr));
            string destIP = (topMsg->getDestinationIP());
            char *meh = new char [destIP.size()+1];
            strcpy(meh, destIP.c_str());
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
                //cout << "sending frag i " << fragments[i]->getFragmentCount() << endl;
                string msgStr = fragments[i]->marshal();
                char *msgPtr = new char [msgStr.size()+1];
                strcpy(msgPtr, msgStr.c_str());

                myUDPSocket->sockMtx.lock();
                int n = sendto(myUDPSocket->sock, msgPtr, strlen(msgPtr), 0,(sockaddr*) &destAddr,sizeof(destAddr));
                myUDPSocket->sockMtx.unlock();

                if(fragments[i]->getMessageType() != MessageType::Ack)
                {

                    Message * temp = new Message;
                    temp->setDestinationIP(fragments[i]->getDestinationIP());
                    temp->setFragState(i+1, fragments.size());
                    temp->setMessage(fragments[i]->getMessage());
                    temp->setMessageSize(fragments[i]->getMessageSize());        
                    temp->setSourceIP(fragments[i]->getSourceIP());
                    temp->setSourcePort(fragments[i]->getSourcePort());
                    temp->setDestinationIP(fragments[i]->getDestinationIP());
                    temp->setMessageTimestamp(fragments[i]->getMessageTimestamp());
                    temp->setDestinationPort(fragments[i]->getDestinationPort());
                    temp->setRPCID(fragments[i]->getRPCId());
                    temp->setMessageType(fragments[i]->getMessageType());
                    temp->setOperation(fragments[i]->getOperation());
                    string fragID = (myUDPSocket->getFragmentID(temp));
                    cout << "Non acked frag id " << fragID << endl;

                    NonAckedMtx.lock();
                    myUDPSocket->NonAcked[fragID] = {myUDPSocket->faultTrials, temp};
                    NonAckedMtx.unlock();
                }

                usleep(100);
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
    FaultThread->join();
    close(sock);
}