#ifndef PEER_H
#define PEER_H
#define MSG_SIZE 1024
#include "UDPServerSocket.h"
class Server
{

    private:
        UDPServerSocket udpServerSocket;
        UDPClientSocket udpSocket;

        std::queue<Message *> inputMessageQueue;
        std::mutex inputMessageMtx;
        Message * getRequest();
        // Message * doOperation();
        // void sendReply (Message * _message);
        void sendReply (struct sockaddr_in targetAddr);
        void doOperation();
    public:
        Peer(char * _listen_hostname, int _listen_port, char * _peerAddr, int _peerPort);
        // void serveRequest();
        ~Server();



private:
    UDPClientSocket udpSocket;
    struct timeval clientReadTimeout;

public:
Client(char * _hostname, int _port);
// Message * execute(Message * _message);
int execute(char * _message, bool activateTimeout, int requestNum);
void setTimeout(int timeoutSec, int timeoutMicro);
~Client();






};
#endif // PEER_H