#ifndef PEER_H
#define PEER_H
#define MSG_SIZE 1024

#include "UDPServerSocket.h"
using namespace std; 

enum Operation {login, signup, logout, uploadImage, viewImage, requestImageAccess, getPortnIP, getAllImages, updateStatus};

class Server
{

    private:
        UDPServerSocket udpServerSocket;
        UDPClientSocket udpClientSocket;
        struct timeval clientReadTimeout;
        queue<Message *> inputMessageQueue;
        mutex inputMessageMtx;

        Message * getRequest();
        void doOperation();
        void sendReply (struct sockaddr_in targetAddr, Message * reply)

    public:
        Peer(char * ip, int _listen_port, int _clientPort); 
        void sendMessage(Message * FullMessage, Message * ReplyMessage, bool activateTimeout);
        void setTimeout(int timeoutSec, int timeoutMicro);
        ~Server();

};
#endif // PEER_H

//TODO
// 1) Handle the different operations 
// 2) Form the message object based on the operation, sender and receiver
// 3) Check that received reply is for the same message 
// 4) Increment and use RPC_ID
// 5) Ip, port to directory
