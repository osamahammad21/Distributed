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
#include <vector>
#define DELIM ","
#define STATUS_UPDATE_TIME 4
#define IMAGES_DB_PATH "myimagesdb.txt"
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
        vector<string> myImages;
        std::thread* read_thread;
        std::thread* serve_thread;
        std::thread* status_thread;
        mutex rpcidmtx,vectorMtx;
        bool dest = false;
        void listen();//thread
        void serve();//thread
        void status(string input);//thread
    public:
        Peer(int port); 
        string getImageUpdates();
        void addImageLocally(string imageId);
        void removeImageLocally(string imageId);
        void setDS(string dsaddr,int dsport);
        void startStatusUpdates(string token);
        string login(string username,string password);
        string signup(string username,string password);
        string uploadImage(string token,string imagename,string image64);
        string removeImage(string token,string imagename);
        string logout(string token);
        string getPortnIP(string token,string targetusername);
        string getAllImagesFromDS(string token);
        string getAllImagesFromPeer(string myusername,string targetusername,string ip,int port);
        string getImage(string myusername,string ownerusername,string targetadd,unsigned int targetport,string imagename);
        ~Peer();

};
//#include "Peer.cpp"
#endif // PEER_H
