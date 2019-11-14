#include "Peer.h"

//Peer constructor
Peer :: Peer(char * _listen_hostname, int _listen_port, char * _peerAddr, int _peerPort){
    //create client socket
    udpSocket.initializeClient(_peerAddr, _peerPort);

    //create server socket and port
    bool success = udpPeerSocket.initializeServer(_listen_hostname, _listen_port);
    
    //server socket waiting for request
    if(success){
        std::thread* m_thread=new std::thread(&Peer::getRequest,this);
    }
}

//peer getRequest
Message * Peer :: getRequest(){
    const int SIZE = 1024;
    char message[SIZE];  
    bool stop;
    do{
        udpServerSocket.readFromSocketWithBlock (message,  SIZE);
        printf("Received message from client: %s\n",message);
        Message receivedMessage(message);
        inputMessageMtx.lock();
        inputMessageQueue.push(receivedMessage);
        inputMessageMtx.unlock();
        struct sockaddr_in targetAddr = udpServerSocket.peerAddr;

        //NEEDS ADJUSTMENT!!!
        std::thread* m_thread=new std::thread(&Peer::sendReply,this,targetAddr);

        //WHEN WILL WE STOP NOW???
        stop = (message[0] == 'q' && strlen(message) == 1);
    }while(! stop);
} 

//TODO
void Peer :: doOperation()
{
    
}

//Peer send reply
void Peer :: sendReply (struct sockaddr_in targetAddr, Message * reply)
{
    usleep(9000);
    const int SIZE = 1024;
    inputMessageMtx.lock();
    Message* message; 
    message = inputMessageQueue.front();
    inputMessageQueue.pop();
    inputMessageMtx.unlock();

    char * marshaledReply = new char [1024]; 
    marshaledReply = reply.marshal();
    //FRAGMENT!!!!!!!!!!!!!!!!!!!!!!!!


    //using udpServerSocket ???
    udpServerSocket.writeToSocketToAddr(marshaledReply, (unsigned)strlen(marshaledReply), targetAddr);
}

//Peer sendMessage (instead of Client's execute)
int Peer :: sendMessage(Message * _message, bool activateTimeout, int requestNum)
{
    char request[1024] = {0};
    if(!(_message[0] == 'q' && strlen(_message) == 1))
    snprintf(request, sizeof request, "%s. ID = %d.\0", _message, requestNum);
    else
    {
    snprintf(request, sizeof request, "%s", _message);
    }
    

    int n = udpSocket.writeToSocket(request, (unsigned)strlen(request));
    if(n<0)
        return -1;//error send
    const int SIZE = 1024;
    char message[SIZE]; 
    if(activateTimeout)
        n = udpSocket.readFromSocketWithTimeout(message, SIZE, clientReadTimeout);
    else
        n = udpSocket.readFromSocketWithBlock(message, SIZE);
    
    if(n<0)
        return -2;//error no reply
    else
    {

        std::string reply = message,requestMsg=request;
        int pos = reply.find(requestMsg);
        if(pos<0)
        {
            return -3;//error irrelevant reply
        }else
        {
            printf("Received reply from server %s.\n",message);
        }
        
    }
    
    return 1;
}

//Peer setTimeout
void Peer::setTimeout(int timeoutSec, int timeoutMicro)
{
    clientReadTimeout.tv_sec = timeoutSec;
    clientReadTimeout.tv_usec = timeoutMicro;
}


Server :: ~Server(){

}