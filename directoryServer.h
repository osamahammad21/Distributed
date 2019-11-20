#ifndef DIRECTORYSERVER_H
#define DIRECTORYSERVER_H

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <mutex>
#include "UDPSocket.h"
#include "Message.h"
#include "rapidcsv.h"

#define TIMEOUT 30;
using namespace std;

class directoryServer
{
    private:
		string usersFile = "./users.csv";
		mutex mtx,mtxStatus;
		UDPSocket udpObj,udpObjStatus;
		thread* listen_thread;
		thread* op_thread = nullptr;
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
		unordered_map<string, int> statusDict;

        void login(string&, string&, Message* , directoryServer*);
        void logout(string&, Message*, directoryServer*);
        void signup(string&, string&, Message* , directoryServer*);
        void uploadimage(string&, string&, Message* , directoryServer*);
		string getPortnIP(string&, Message*, directoryServer*);//string of "port,ip"
		string getAllImages(Message*, directoryServer*);//string of imageName,...
		bool authenticate(string& username, string& password);
		bool usernameExists(string&);
		void updateStatus(string& , directoryServer*);



    public:
        directoryServer(int,int);
        ~directoryServer();
        void listen();
		void listenStatus();//wip
        void doOperation(Message *request);
};
#include "directoryServer.cpp"
#endif
