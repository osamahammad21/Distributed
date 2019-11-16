#ifndef DIRECTORYSERVER_H
#define DIRECTORYSERVER_H

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>
#include "UDPSocket.h"
#include "Message.h"
#include "rapidcsv.h"


using namespace std;

class directoryServer
{
    private:
        string usersFile = "./users.csv";
        //everything here is stored in users.csv + username
		struct data
		{
			string password;
			int online = 0;	//bool causes issues
			string ip = "";
			unsigned int port;
			string token;
			vector <string> imageName;
			vector <string> imageID;
			int imageCount = imageID.size();

		};
		unordered_map<string, data> usersDict;

        void login(string&, string&, Message* , directoryServer*);
        void logout(string&, Message*, directoryServer*);
        void signup(string&, string&, Message* , directoryServer*);
        void uploadimage(string&, string&, Message* , directoryServer*);
        //void requestImages(const string&, Message* , directoryServer*);//todo??
		bool authenticate(string& username, string& password);
		bool usernameExists(string&);


    public:
        directoryServer();
        ~directoryServer();
        //void listen();//todo
        //void doOperation(Message *request);//todo???
};

#endif