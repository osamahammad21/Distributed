#ifndef CLIENT_H
#define CLIENT_H
#include "UDPClientSocket.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <cstring>
#include <unistd.h>
#include <stdio.h>      /* printf, fopen */
#include <stdlib.h>     /* exit, EXIT_FAILURE */

class Client
{

private:
    UDPClientSocket udpSocket;
    struct timeval clientReadTimeout;

public:
Client(char * _hostname, int _port);
// Message * execute(Message * _message);
int execute(char * _message, bool activateTimeout, int requestNum);
int Send(Message * m);
void setTimeout(int timeoutSec, int timeoutMicro);
~Client();

};
#include "Client.cpp"
#endif // CLIENT_H