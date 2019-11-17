#include "Server.h"
#include "Message.h"
Server :: Server(char * _listen_hostname, int _listen_port)
{
    bool success = udpServerSocket.initializeSocket(_listen_hostname, _listen_port);
    printf("heren");
    if(success)
        getRequest();
}
void Server :: getRequest()
{
    const int SIZE = 1024;
    char message[SIZE];  
    bool stop = false;
    int fragsTotal;
    int fragsCount;
    do
    {
        
        printf("Received message from client");

            do
            {
                udpServerSocket.readFromSocketWithBlock (message,  SIZE);
                Message newM(message);
                fragsTotal = newM.getFragmentTotal();
                fragsCount = newM.getFragmentCount();
                cout << "Frags total = " << fragsTotal << endl;
                cout << "Frags count = " << fragsCount << endl;
                cout << newM.getMessageType()<< endl;
                cout << newM.getFragmentCount()<< endl;
                cout << newM.getFragmentTotal()<< endl;
                cout << newM.getSourceIP()<< endl;
                cout << newM.getDestinationIP()<< endl;
                cout << newM.getPort()<< endl;
                cout << newM.getRPCId()<< endl;
                cout << newM.getOperation()<< endl;
                cout << newM.getMessageSize()<< endl;
                cout << newM.getMessage()<< endl;
            }while(fragsCount < fragsTotal);
            
       // inputMessageMtx.lock();
       // inputMessageQueue.push(message);
       // inputMessageMtx.unlock();
       //struct sockaddr_in targetAddr = udpServerSocket.peerAddr;
       //std::thread* m_thread=new std::thread(&Server::sendReply,this,targetAddr);
       // stop = (message[0] == 'q' && strlen(message) == 1);
    }while(!stop);
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