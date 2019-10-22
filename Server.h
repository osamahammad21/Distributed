#ifndef SERVER_H
#define SERVER_H
#include "UDPServerSocket.h"

class Server
{

private:
UDPServerSocket udpServerSocket;
std::queue<char *> inputMessageQueue;
std::mutex inputMessageMtx;
// Message * getRequest();
void getRequest();
// Message * doOperation();
// void sendReply (Message * _message);
void sendReply (struct sockaddr_in targetAddr);
void doOperation();
public:
Server(char * _listen_hostname, int _listen_port);
// void serveRequest();
~Server();
};
#include "Server.cpp"
#endif // SERVER_H