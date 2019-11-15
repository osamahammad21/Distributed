#include "UDPSocket.h"

UDPSocket :: UDPSocket (){
    
    
}
void UDPSocket :: makeReceiverSA(struct sockaddr_in *sa, int port)
{
    sa->sin_family  =  AF_INET;
    sa->sin_port = htons(port);
    sa->sin_addr.s_addr = htonl(INADDR_ANY);
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
void UDPSocket :: makeLocalSA(struct sockaddr_in *sa)
{
    sa->sin_family  =  AF_INET;
    sa->sin_port = htons(0);
    sa-> sin_addr.s_addr = htonl(INADDR_ANY);
}

void UDPSocket :: makeDestSA(struct sockaddr_in * sa, char *hostname, int port)
{   
    struct hostent *host;
    sa->sin_family  =  AF_INET;
    if((host = gethostbyname(hostname))== (void*)(0))
    {
        printf("Unknown host name\n");
        exit(-1);
    }
    sa->sin_addr = *(struct in_addr *) (host->h_addr_list[0]);
    sa->sin_port = htons(port);
}

bool UDPSocket ::initializeServer (char * _myAddr, int _myPort){

    struct sockaddr_in myAddr, peerAddr;
    //setting class variables
    myAddress = _myAddr;
    myPort = _myPort;

    if((sock = socket(AF_INET, SOCK_DGRAM, 0))<0)
    {
        perror("Initializing socket of server failed");

        return false;
    }
    int enableReuse = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &enableReuse, sizeof(enableReuse)) < 0)
        perror("setsockopt(SO_REUSEADDR) failed");
    makeReceiverSA(&myAddr, myPort);

    if( bind(sock, (struct sockaddr *)&myAddr, sizeof(struct sockaddr_in))!= 0)
    {
        perror("Bind of server socket failed\n");

        close(sock);
        return false;
    }
    peerAddr.sin_family = AF_INET; 
    return true; 

}
bool UDPSocket ::initializeClient (char * _peerAddr, int _peerPort){

    peerAddress = _peerAddr;
    peerPort = _peerPort;

    if(( sock = socket(AF_INET, SOCK_DGRAM, 0))<0) 
    {
        printf("Initialization of client socket failed");
        return false;
    }
    setBroadcast(sock);/*see Section 2.7 */
    makeLocalSA(&myAddr);
    if( bind(sock, (sockaddr*)&myAddr, sizeof(myAddr))!= 0)
    {
        printf("Bind of client socket failed\n");
        close (sock);
        return false;
    }
    makeDestSA(&peerAddr,_peerAddr, _peerPort);
    return true; 
}
int UDPSocket ::writeToSocket (char * buffer,  int maxBytes ){
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

int UDPSocket ::getMyPort (){
    return myPort;
}
int UDPSocket ::getPeerPort (){
    return peerPort;
}
void UDPSocket::fragmentMsg(Message FullMessage, vector<Message *> & frags)
{
    string MessageWithoutHeader = string(FullMessage.getMessage());
    unsigned int NumberOfFrags = ceil((float)FullMessage.getMessageSize()/FRAG_MSG_SIZE);

    vector<string> subMessagesWithoutHeader;

    for(int i=0;i<NumberOfFrags; i++)
        subMessagesWithoutHeader.push_back(MessageWithoutHeader.substr(i*FRAG_MSG_SIZE, FRAG_MSG_SIZE));

    for(unsigned int i=0; i<NumberOfFrags; i++)
    {
        Message fragi;
        fragi.setDestinationIP(FullMessage.getDestinationIP());
        fragi.setFragState(i, NumberOfFrags);
        fragi.setMessage((char *)(subMessagesWithoutHeader[i].c_str()));
        fragi.setSourceIP(FullMessage.getSourceIP());
        fragi.setRPCID(FullMessage.getRPCId());
        fragi.setMessageType(FullMessage.getMessageType());
        fragi.setPort(FullMessage.getPort());
        frags.push_back(&fragi);
    }
}
UDPSocket :: ~UDPSocket ( ){
    close(sock);
}