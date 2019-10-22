#ifndef UDPSERVERSOCKET_H
#define UDPSERVERSOCKET_H
#include "UDPSocket.h"

class UDPServerSocket : public UDPSocket
{

public:
UDPServerSocket ();
bool initializeServer (char * _myAddr, int _myPort);
~UDPServerSocket ( );
};

#include "UDPServerSocket.cpp"
#endif // UDPSERVERSOCKET_H