#ifndef UDPSOCKET_H
#define UDPSOCKET_H
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include "string.h"
#include <thread>
#include <mutex>  
#include <queue>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h> 
#include "Message.h"
#include <algorithm>
#include <math.h>
#include <thread>
#include <unordered_map>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h> 
#include <chrono>
#include <sys/time.h>
#include <ctime>
#include <unistd.h>
using namespace std::chrono;
using namespace std;
//#define RECEIVE_OUTPUT_FILE_LOG
//#define DEBUG
class UDPSocket
{
    protected:
        int sock;
        struct sockaddr_in myAddr;
        int myPort;
        string myAddress_str;
        thread * ReceiveThread;
        thread* SendThread;
        thread * FaultThread;
        mutex ReceiveBufferMtx;
        mutex SendBufferMtx;
        mutex NonAckedMtx;
        mutex sockMtx;
        //<string(MSGID+fragc), <trialsLeft, Message *>>
        unordered_map<string, pair<unsigned int, Message *>> NonAcked;
        queue<Message *> ReceiveBuffer;
        queue<Message *> SendBuffer; 
        bool enabled = true;
        unsigned int FRAG_MSG_SIZE = 10000;
        unsigned int SOCK_MAX_BUFFER_SIZE = 100000;
        unsigned int faultTrials = 20;
        ofstream outFile;
        bool dest=false;
    public:   

        UDPSocket ();    
        bool initializeSocket(char * _myAddr, unsigned int _myPort);
        bool initializeSocket(unsigned int _myPort);
        char * getMachineIP();
        int getMyPort(); 
        string getMyIP();
        void setMyPort(unsigned int _myPort);
        Message * receiveMsg();
        bool sendMessage(Message * FullMessage);
        void fragmentMsg(Message * FullMessage, vector<Message *> & frags);
        string getMsgID(Message* message);
        string getFragmentID(Message* message);
        string getAckFragmentID(Message* message);
        void sendingHandler(UDPSocket * myUDPSocket);
        void receiveHandler(UDPSocket * myUDPSocket);
        void faultToleranceHandler(UDPSocket * myUDPSockeT);
        void setBroadcast(int s);
        ~UDPSocket ( );
};
#endif // UDPSOCKET_H
