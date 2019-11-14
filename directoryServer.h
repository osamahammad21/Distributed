#ifndef DIRECTORYSERVER_H
#define DIRECTORYSERVER_H

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include "string.h"
#include "UDPSocket.h"
#include "Message.h"

using namespace std;

class directoryServer
{
    private:
        unordered_map <string,data> usersDict;
        string usersFile = "./users.txt";
        ifstream infile;
        ofstream outfile;
        //everything here is stored in users.txt + username
        struct data 
        {
            string password;
            bool online = false;
            string ip = "";
            unsigned int port;
            string token;
            vector <string> imageName;
            vector <string> imageID;
            int imageCount = imageID.size()
     
        }
        void login(str&, str&, Message* ,CentralServer*);
        void logout(const str&, Message*, CentralServer*);
        void signup(const str&, const str&, Message* ,CentralServer*);
        void uploadimage(const str&, const str&, Message* ,CentralServer*);
        void requestImages(const str&, Message* ,CentralServer*);

    public:
        directoryServer();
        ~directoryServer();
        void listen();
        void doOperation(Message *request);
};

#endif