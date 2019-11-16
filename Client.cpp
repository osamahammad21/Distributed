#include "Client.h"


Client :: Client(char * _hostname, int _port){
    udpSocket.initializeClient(_hostname, _port);
}
int Client :: execute(char * _message, bool activateTimeout, int requestNum)
{
    char request[1024] = {0};
    if(!(_message[0] == 'q' && strlen(_message) == 1))
        snprintf(request, sizeof request, "%s. ID = %d.\0", _message, requestNum);
    else
    snprintf(request, sizeof request, "%s", _message);

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

int Client :: Send(Message * m)
{
    char request[1024] = {0};

    int n = udpSocket.sendMessage(m);
    if(n<0)
        return -1;//error send
    else
    {
        cout << "Client: Send msg success" << endl;
        return 1;
    }
    
}
void Client::setTimeout(int timeoutSec, int timeoutMicro)
{
    clientReadTimeout.tv_sec = timeoutSec;
    clientReadTimeout.tv_usec = timeoutMicro;
}

Client :: ~Client(){

}