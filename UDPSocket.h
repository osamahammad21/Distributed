#ifndef UDPSOCKET_Hclass
#define UDPSOCKET_Hclass 
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include "string.h"
#include <thread>         // std::thread
#include <mutex>  
#include <queue>
#include <unistd.h>
#include <stdio.h>      /* printf, fopen */
#include <stdlib.h>     /* exit, EXIT_FAILURE */
#include <netinet/in.h> 
#include "Message.h"
#include <algorithm>
#include <math.h>
#define FRAG_MSG_SIZE 3
class UDPSocket
{
    protected:
    int sock;
    struct sockaddr_in myAddr;
    
    char * myAddress;
    char * peerAddress;
    int myPort;
    int peerPort;
    bool enabled;
    pthread_mutex_t mutex;
    void setBroadcast(int s);
    void makeLocalSA(struct sockaddr_in *sa);
    void makeDestSA(struct sockaddr_in * sa, char *hostname, int port);
    void makeReceiverSA(struct sockaddr_in *sa, int port);
    
    public:    
    struct sockaddr_in peerAddr;
    UDPSocket ();   
    // void setFilterAddress (char * _filterAddress);   
    // char * getFilterAddress ();   
    bool initializeServer (char * _myAddr, int _myPort);    
    bool initializeClient (char * _peerAddr, int _peerPort);
    int writeToSocket (char * buffer,  int maxBytes );
    int writeToSocketToAddr (char * buffer,  int maxBytes ,struct sockaddr_in targetAddr);
    void fragmentMsg(Message * FullMessage, vector<Message *> & frags);
    bool sendMessage(Message * FullMessage);
    // int writeToSocketAndWait (char * buffer, int  maxBytes,int microSec ); 
    // int readFromSocketWithNoBlock (char * buffer, int  maxBytes );
    int readFromSocketWithTimeout (char * buffer, int maxBytes, struct timeval tv);
    int readFromSocketWithBlock (char * buffer,  int maxBytes ); 
    // int readSocketWithNoBlock (char * buffer, int  maxBytes );
    // int readSocketWithTimeout (char * buffer, int maxBytes, int timeoutSec, int timeoutMilli);
    // int readSocketWithBlock (char * buffer,  int maxBytes );
    int getMyPort (); 
    int getPeerPort ();
    // void enable();
    // void disable();
    // bool isEnabled();
    // void lock();
    // void unlock();
    // int getSocketHandler();
    ~UDPSocket ( );
};
#include "UDPSocket.cpp"

#endif // UDPSOCKET_H