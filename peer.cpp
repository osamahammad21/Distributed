#include "Peer.h"

//Peer constructor
Peer :: Peer(char * ip, int _listen_port, int _clientPort){
    //create client socket
    udpClientSocket.initializeClient(ip, _clientPort);

    //create server socket and port
    bool success = udpServerSocket.initializeServer(ip, _listen_port);
    
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

        std::thread* m_thread=new std::thread(&Peer::sendReply, this);
    }while(true);
} 

//TODO
void Peer :: doOperation()
{
    
}

//Peer send reply
// HABDDDD
void Peer :: sendReply ()
{
    usleep(9000);
    const int SIZE = 1024;
    inputMessageMtx.lock();
    Message* message; 
    message = inputMessageQueue.front();
    inputMessageQueue.pop();
    inputMessageMtx.unlock();

    switch(message.getOperation()){
        case Operation:: uploadImage: 
            viewImage();
        break; 
        case Operation:: requestImageAccess: 
        break;
    }
    //do operations
    
}

//Peer sendMessage (instead of Client's execute)
int Peer :: sendMessage(Message * FullMessage, Message * ReplyMessage, bool activateTimeout)
{
    bool success = udpClientSocket.sendMessage(FullMessage);
    if (! success)
        return -1; 

    //TODO
    //Get reply into message object (WAIT FOR ALAA'S PROTOTYPE)
    if(activateTimeout)
        n = udpSocket.readFromSocketWithTimeout(ReplyMessage, MSG_SIZE, clientReadTimeout);
    else
        n = udpSocket.readFromSocketWithBlock(ReplyMessage, MSG_SIZE);
    if(n<0)
        return -2;//error no reply
    else
    {
        //TODO
        //check rpcID ????????
        // std::string reply = message,requestMsg=request;
        // int pos = reply.find(requestMsg);
        // if(pos<0)
        // {
        //     return -3;//error irrelevant reply
        // }else
        // {
        //     printf("Received reply from server %s.\n",message);
        // }
        
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


