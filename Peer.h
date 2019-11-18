#ifndef PEER_Hclass
#define PEER_Hclass

#include "UDPSocket.h"
#include <map>
#include <pthread.h>
#include "Image.h"
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#define DELIM ","
using namespace std; 
class Peer
{
    private:
        UDPSocket sock;
        string dsaddr;
        int dsport;
        int rpccount=1;
        map<unsigned int,Message*> replyMessages;
        vector<Message*> serveMessages;
        std::thread* read_thread;
        std::thread* serve_thread;
        string myusername;
        bool dest = false;
    public:
        Peer(string ip, int port); 
        void setDS(string dsaddr,int dsport);
        string login(string username,string password);
        string signup(string username,string password);
        string uploadImage(string token,string imagename);
        string logout(string token);
        string getPortnIP(string token,string targetusername);
        string getAllImages(string token);
        string getImage(string myusername,string ownerusername,string targetadd,unsigned int targetport,string imagename);
        void listen();//thread
        void serve();//thread
        void setUserName(string username);
        ~Peer();

};
#include "Peer.cpp"
#endif // PEER_H
