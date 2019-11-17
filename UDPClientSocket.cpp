#include "UDPClientSocket.h"
UDPClientSocket :: UDPClientSocket (){
    UDPSocket();
}
bool UDPClientSocket :: initializeClient (char * _peerAddr, int _peerPort){
    UDPSocket::initializeSocket(_peerAddr, _peerPort);
}

UDPClientSocket :: ~UDPClientSocket ( ){
}