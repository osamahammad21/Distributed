#include "Peer.h"

//Peer constructor
Peer :: Peer(char * _listen_hostname, int _listen_port, char * _peerAddr, int _peerPort){
    //create client socket
    udpClientSocket.initializeClient(_peerAddr, _peerPort);

    //create server socket and port
    bool success = udpServerSocket.initializeServer(_listen_hostname, _listen_port);
    
    //server socket waiting for request
    if(success){
        std::thread* m_thread=new std::thread(&Peer::getRequest,this);
    }
}

//peer getRequest
Message * Peer :: getRequest(){
    char message[MSG_SIZE];  
    bool stop;
    do{
        udpServerSocket.readFromSocketWithBlock (message,  SIZE);
        printf("Received message from client: %s\n",message);
        Message receivedMessage(message);
        inputMessageMtx.lock();
        inputMessageQueue.push(receivedMessage);
        inputMessageMtx.unlock();
        struct sockaddr_in targetAddr = udpServerSocket.peerAddr;

        std::thread* m_thread=new std::thread(&Peer::sendReply,this,targetAddr);

        //TODO
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
    UDPClientSocket udpSocket;
    
    //TODO
    //How the struct is accessed is taken from UDPSocket.cpp. Check its correctness
    udpSocket.initializeClient(targetAddr->sin_addr.s_addr, targetAddr->sin_port);

    usleep(9000);
    const int SIZE = 1024;
    inputMessageMtx.lock();
    Message* message; 
    message = inputMessageQueue.front();
    inputMessageQueue.pop();
    inputMessageMtx.unlock();

    //TODO 
    //doOperation 

    udpSocket.sendMessage(FullMessage);
}

//Peer sendMessage (instead of Client's execute)
int Peer :: sendMessage(Message * FullMessage, bool activateTimeout, int requestNum)
{
    
    bool success = udpClientSocket.sendMessage(FullMessage);
    if (! success)
        return -1; 

    Message * fullReply;  

    //TODO
    //Get reply into message object (WAIT FOR ALAA'S PROTOTYPE)
    if(activateTimeout)
        n = udpSocket.readFromSocketWithTimeout(message, SIZE, clientReadTimeout);
    else
        n = udpSocket.readFromSocketWithBlock(message, SIZE);
    if(n<0)
        return -2;//error no reply
    else
    {
        //TODO
        //check rpcID ????????
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
