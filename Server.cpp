#include "Server.h"
Server :: Server(char * _listen_hostname, int _listen_port){
    bool success = udpServerSocket.initializeServer(_listen_hostname, _listen_port);
    if(success)
        getRequest();
}
void Server :: getRequest(){
    const int SIZE = 1024;
    char message[SIZE];  
    bool stop;
    do{
        udpServerSocket.readFromSocketWithBlock (message,  SIZE);
        printf("Received message from client: %s\n",message);
        inputMessageMtx.lock();
        inputMessageQueue.push(message);
        inputMessageMtx.unlock();
        struct sockaddr_in targetAddr = udpServerSocket.peerAddr;
        std::thread* m_thread=new std::thread(&Server::sendReply,this,targetAddr);
        stop = (message[0] == 'q' && strlen(message) == 1);
    }while(! stop);
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
void Server :: doOperation()
{
    
}
void Server :: sendReply (struct sockaddr_in targetAddr)
{
    usleep(9000);
    const int SIZE = 1024;
    inputMessageMtx.lock();
    char* message; 
    message = inputMessageQueue.front();
    inputMessageQueue.pop();
    inputMessageMtx.unlock();
    char replyBuffer[SIZE]={0};
    snprintf(replyBuffer, sizeof replyBuffer, "Reply from server: I received request %s.", message);
    udpServerSocket.writeToSocketToAddr(replyBuffer, (unsigned)strlen(replyBuffer),targetAddr);
}

Server :: ~Server(){

}