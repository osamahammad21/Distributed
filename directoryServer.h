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
//#define MAX_SAMPLES 10;
using namespace std;

class directoryServer
{
    private:
		string usersFile = "./users.csv";
		mutex mtx,mtxStatus;
		UDPSocket udpObj;
		thread* listen_thread = nullptr;
		thread* status_thread = nullptr;
        //everything here is stored in users.csv + username
		struct data
		{
			string password;
			int online = 0;	//bool causes issues
			string ip = "";
			unsigned int port;
			string token;
			int imageCount;
			vector <string> imageName;
			vector <string> image64;
		};
		unordered_map<string, data> usersDict;
		unordered_map<string, int> statusDict;
		unordered_map<string, vector<Message*>> buffer;

		void login(string&, string&, Message* , directoryServer*);
        void logout(string&, Message*, directoryServer*);
        void signup(string&, string&, Message* , directoryServer*);
        void uploadimage(string&, string&, string&, Message* , directoryServer*);
		string getPortnIP(string&, string&, Message*, directoryServer*);//string of "port,ip"
		string getAllImages(string&, Message*, directoryServer*);//string of username,imagename,image(base64),...
		bool authenticate(string&, string&);
		bool usernameExists(string&);
		void updateStatus(string& , directoryServer*);
		void removeImage(string&, string&, Message*, directoryServer*);
		string getOnlineUsers(string&, Message*, directoryServer*);
		bool checkOnline(string&);

		void requestImageAccess(string&, string&, string&, Message*, directoryServer*);
		void addImageAccess(string&, string&, string&, string&, Message*, directoryServer*);


		void decrementStatus();
		void clearUsers();




    public:
        directoryServer(unsigned int);
        ~directoryServer();
        void listen();
        void doOperation(Message *request);
};
#endif
