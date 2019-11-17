#ifndef UDPSOCKET_H
#define UDPSOCKET_H
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
#include <thread>
#include <unordered_map>
#include <netinet/in.h>
#include <arpa/inet.h>
#define FRAG_MSG_SIZE 10
#define MAX_BUFFER_SIZE 1000000
class UDPSocket
{
    protected:
        int sock;
        struct sockaddr_in myAddr;
        string myAddress_str;
        thread * ReceiveThread;
        thread* SendThread;
        queue<Message *> ReceiveBuffer;
        queue<Message *> SendBuffer;
        mutex ReceiveBufferMtx;
        mutex SendBufferMtx;
        bool enabled = true;

    
    public:    
        struct sockaddr_in peerAddr;
        int myPort;
        UDPSocket ();    
        bool initializeSocket(char * _myAddr, unsigned int _myPort);
        int writeToSocket (char * buffer,  int maxBytes );
        int writeToSocketToAddr (char * buffer,  int maxBytes ,struct sockaddr_in targetAddr);
        void fragmentMsg(Message * FullMessage, vector<Message *> & frags);
        // int writeToSocketAndWait (char * buffer, int  maxBytes,int microSec ); 
        // int readFromSocketWithNoBlock (char * buffer, int  maxBytes );
        int readFromSocketWithTimeout (char * buffer, int maxBytes, struct timeval tv);
        int readFromSocketWithBlock (char * buffer,  int maxBytes ); 
        // int readSocketWithNoBlock (char * buffer, int  maxBytes );
        // int readSocketWithTimeout (char * buffer, int maxBytes, int timeoutSec, int timeoutMilli);
        // int readSocketWithBlock (char * buffer,  int maxBytes );
        int getMyPort(); 
        string getMyIP();
        //int getPeerPort ();
        // void enable();
        // void disable();
        // bool isEnabled();
        // void lock();
        // void unlock();
        // int getSocketHandler();
        string getMsgID(Message* message);
        bool sendMessage(Message * FullMessage);
        Message * receiveMsg();
        void sendingHandler(UDPSocket* myUDPSocket);
        void receiveHandler(UDPSocket* myUDPSocket);
        //Message * defragment(vector<Message *> frags);



        ~UDPSocket ( );
};
#include "UDPSocket.cpp"
#endif // UDPSOCKET_H