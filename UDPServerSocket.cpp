#include "UDPServerSocket.h"
UDPServerSocket::UDPServerSocket (){
    UDPSocket();
}
bool UDPServerSocket :: initializeServer (char * _myAddr, int _myPort){
    UDPSocket::initializeServer(_myAddr, _myPort);
}
UDPServerSocket :: ~UDPServerSocket ( ){
}