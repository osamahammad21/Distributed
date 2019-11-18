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
#define FRAG_MSG_SIZE 10000
#define MAX_BUFFER_SIZE 100000
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
        void fragmentMsg(Message * FullMessage, vector<Message *> & frags);
        int getMyPort(); 
        string getMyIP();
        string getMsgID(Message* message);
        bool sendMessage(Message * FullMessage);
        Message * receiveMsg();
        void sendingHandler(UDPSocket * myUDPSocket);
        void receiveHandler(UDPSocket * myUDPSocket);
        ~UDPSocket ( );
};
#include "UDPSocket.cpp"
#endif // UDPSOCKET_H