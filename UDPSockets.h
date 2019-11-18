#ifndef UDPSOCKETS_Hclass
#define UDPSOCKETS_Hclass 
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
enum Operation {login, signup, logout, uploadImage, getImage, getPortnIP, getAllImages, updateStatus,requestImageAccess};

class UDPSockets
{   
    public:
    bool dest=false;
    bool ask=false;
    UDPSockets()
    {};
    ~UDPSockets()
    {};
    Message * receiveMsg()
    {
        if(dest||ask){
            Message *m = new Message();
            m->setMessageType(MessageType::Reply);
            m->setRPCID(55);
            return m;
        }
        Message *m = new Message();
        string reply="osama,image123";
        char *arr = new char[reply.size()+1];
        m->setRPCID(20);
        strcpy(arr,reply.c_str());
        m->setMessageType(MessageType::Request);
        m->setOperation(Operation::getImage);
        m->setMessage(arr,reply.size());
        m->setSourceIP(getMyIP());
        m->setSourcePort(getMyPort());
        ask=true;
        return m;
    }

    bool sendMessage(Message * FullMessage)
    {
        cout<<"sending"<<endl;
        return true;
    }
    int getMyPort ()
    {
        return 123;
    }
    string getMyIP()
    {
        return "127.0.0.1";
    }

};
#endif 