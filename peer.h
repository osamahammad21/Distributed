#ifndef PEER_H
#define PEER_H
#define MSG_SIZE 1024
#include "UDPServerSocket.h"

enum Operation {login, signup, logout, uploadImage, changeSettings, viewImage, requestImage};

class Server
{

    private:
        UDPServerSocket udpServerSocket;
        UDPClientSocket udpClientSocket;
        struct timeval clientReadTimeout;
        std::queue<Message *> inputMessageQueue;
        std::mutex inputMessageMtx;
        
        Message * getRequest();
        void doOperation();

        (MessageType _message_type,  unsigned int _fragmentCount, unsigned int  _fragmentTotal, string _sourceIP, string _destIP, unsigned int _port, unsigned int _rpc_id, unsigned int _operation, long long _message_size,  char * _message)

        int sendMessage(Message * FullMessage, bool activateTimeout, int requestNum);
        void sendReply (struct sockaddr_in targetAddr, Message * reply)

    public:
        Peer(char * _listen_hostname, int _listen_port, char * _peerAddr, int _peerPort); 
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
