#ifndef UDPCLIENTSOCKET_H
#define UDPCLIENTSOCKET_H
#include"UDPSocket.h"
class UDPClientSocket : public UDPSocket
{

public:
UDPClientSocket ();
bool initializeSocket (char * _peerAddr, int _peerPort);

~UDPClientSocket ( );
};

#include "UDPClientSocket.cpp"
#endif // UDPCLIENTSOCKET_H