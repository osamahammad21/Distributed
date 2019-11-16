#ifndef USER_H
#define USER_H
#define DIRECTORY_IP "ay7aga"
#define DIRECTORY_PORT 10
#include <string> 
#include <vector>
using namespace std; 

struct userInfo {        
    String username;
    String password;  
}

class User : public Peer
{
    private: 
        userInfo myInfo; 
        vector <Image> myImages;
        vector <Image> allImages; 
        int rpcId; 
    public:
        User(); 

        void updateStatus();
        void login ();
        void signup();
        void logout();

        //acting as a client
        void Client_uploadImage(string imageName);
        void Client_viewImage(string ownerUsername, string imageName);
        void Client_requestImageAccess(); 
        void getAllImages();

        //acting as a server
        virtual void Server_viewImage(Message * request);
        virtual void Server_requestImageAccess();

        ~User();
};
#endif // USER_H