#include "directoryServer.h"
#include <thread>

using namespace std;

directoryServer::directoryServer(string ip, int port)
{
	rapidcsv::Document doc(usersFile);
	int totalUsers = doc.GetRowCount();
	int n = ip.length();
    char *char_array=new char[n+1];
	strcpy(char_array, ip.c_str());
	udpObj.initializeSocket(char_array, port);
	listen_thread = new thread(&directoryServer::listen,this);




	//constructing dictionary from csv
	for (int i = 0; i<totalUsers;i++)
	{
		string username = doc.GetRowName(i);
		usersDict[username].password = doc.GetCell<string>(0,i);
		if (doc.GetCell<string>(1, i) != "")
			usersDict[username].online = doc.GetCell<int>(1, i);
		usersDict[username].ip = doc.GetCell<string>(2, i);
		if (doc.GetCell<string>(3, i)!="")
			usersDict[username].port = doc.GetCell<unsigned int>(3, i);
		usersDict[username].token = doc.GetCell<string>(4, i);
		if (doc.GetCell<string>(5, i) != "")
			usersDict[username].imageCount = doc.GetCell<int>(5, i);

		for (int j = 6; j < 6 + usersDict[username].imageCount * 2; j += 2)
		{
			usersDict[username].imageName.push_back(doc.GetCell<string>(j, i));
			usersDict[username].imageID.push_back(doc.GetCell<string>(j + 1, i));
		}
	}
}

directoryServer::~directoryServer()
{
}

void directoryServer::login(string &username, string &password, Message* msg, directoryServer* ds)
{
    bool isAuthenticated = false;
	//UDPSocket sockobj;
    if (ds->authenticate(username, password))
    {
		rapidcsv::Document doc(usersFile);
        data onlineUser;
		//update struct
		usersDict[username].online = true;
		usersDict[username].port = msg->getSourcePort();
		usersDict[username].ip = msg->getSourceIP();
		//usersDict[username].port = 8080;
		//usersDict[username].ip = "1.1.1.1";
		usersDict[username].token = username + password;
        
		//update file
		doc.SetCell<int>("online", username, usersDict[username].online);
		doc.SetCell<unsigned int>("port", username, usersDict[username].port);
		doc.SetCell<string>("ip", username, usersDict[username].ip);
		doc.SetCell<string>("token", username, usersDict[username].token);
		mtx.lock();
		doc.Save();
		mtx.unlock();

		isAuthenticated = true;


    }
	//send appropriate reply
	int n = usersDict[username].token.length(); 
    char *char_array=new char[n+1]; 
    strcpy(char_array, usersDict[username].token.c_str()); 

    Message *message = new Message();
	message->setSourceIP(udpObj.getMyIP());
    message->setSourcePort(udpObj.getMyPort());
    message->setRPCID(msg->getRPCId());
    message->setDestinationIP(msg->getSourceIP());
    message->setDestinationPort(msg->getSourcePort());
    message->setOperation(Operation::login);
	message->setMessageType(MessageType::Reply);
	message->setMessage(char_array,n);
	udpObj.sendMessage(message);
	
}

bool directoryServer::authenticate(string &username, string &password)
{
    if (usersDict.find(username) != usersDict.end())
        return usersDict[username].password == password;
    return false;
}

void directoryServer::logout(string& username, Message* msg, directoryServer* ds)
{
	rapidcsv::Document doc(usersFile);

	//update struct
	ds->usersDict[username].online = 0;

	//update file
	doc.SetCell<int>("online", username, usersDict[username].online);
	mtx.lock();
	doc.Save();
	mtx.unlock();
	//send appropriate reply
	string ok ="ok";
	int n = ok.length(); 
    char *char_array=new char[n+1]; 
    strcpy(char_array, ok.c_str()); 

    Message *message = new Message();
	message->setSourceIP(udpObj.getMyIP());
    message->setSourcePort(udpObj.getMyPort());
    message->setRPCID(msg->getRPCId());
    message->setDestinationIP(msg->getSourceIP());
    message->setDestinationPort(msg->getSourcePort());
    message->setOperation(Operation::logout);
	message->setMessageType(MessageType::Reply);
	message->setMessage(char_array,n);
	udpObj.sendMessage(message);
	
}
void directoryServer::signup(string& username, string& password, Message* msg, directoryServer* ds)
{
	bool exists = false;
	if (ds->usernameExists(username))
		exists = true;
	//update struct and file with new user
	if (!exists) {
		rapidcsv::Document doc(usersFile);
		usersDict[username].password = password;
		doc.SetCell<string>(0, doc.GetRowCount(), password);
		doc.SetRowName(doc.GetRowCount() - 1, username);
		mtx.lock();
		doc.Save();
		mtx.unlock();
		//login after signup
		login(username,password,msg,ds);
	}
}

bool directoryServer::usernameExists(string& username)
{
	return usersDict.find(username) != usersDict.end();
}

void directoryServer::uploadimage(string& username, string& imagename, Message* msg, directoryServer* ds)
{
	rapidcsv::Document doc(usersFile);

	//imageID unique per user assumed
	string imageID = to_string(usersDict[username].imageCount+1);
	ds->usersDict[username].imageCount += 1;
	ds->usersDict[username].imageID.push_back(imageID);
	ds->usersDict[username].imageName.push_back(imagename);
	
	//add to users.csv
	doc.SetCell<int>("imageCount", username, usersDict[username].imageCount);
	vector<string>temp = doc.GetRowNames();
	int row = 0;
	for (int i = 0; i < temp.size(); i++)
		if (temp[i] == username)
			row = i;

	doc.SetCell<string>(4 + usersDict[username].imageCount*2, row, usersDict[username].imageName.back());
	doc.SetCell<string>(4 + usersDict[username].imageCount*2 + 1, row, usersDict[username].imageID.back());
	mtx.lock();
	doc.Save();
	mtx.unlock();

	string ok ="ok";
	int n = ok.length(); 
    char *char_array=new char[n+1]; 
    strcpy(char_array, ok.c_str()); 

	Message *message = new Message();
	message->setSourceIP(udpObj.getMyIP());
    message->setSourcePort(udpObj.getMyPort());
    message->setRPCID(msg->getRPCId());
    message->setDestinationIP(msg->getSourceIP());
    message->setDestinationPort(msg->getSourcePort());
    message->setOperation(Operation::uploadImage);
	message->setMessageType(MessageType::Reply);
	message->setMessage(char_array,n);
	udpObj.sendMessage(message);
	


}

string directoryServer::getPortnIP(string& username, Message* msg, directoryServer* ds)
{
	rapidcsv::Document doc(usersFile);

	string pandip =doc.GetCell<string>("port", username) + "," + doc.GetCell<string>("ip",username);
	int n = pandip.length(); 
    char *char_array=new char[n+1]; 
    strcpy(char_array, pandip.c_str()); 

	Message *message = new Message();
	message->setSourceIP(udpObj.getMyIP());
    message->setSourcePort(udpObj.getMyPort());
    message->setRPCID(msg->getRPCId());
    message->setDestinationIP(msg->getSourceIP());
    message->setDestinationPort(msg->getSourcePort());
    message->setOperation(Operation::getPortnIP);
	message->setMessageType(MessageType::Reply);
	message->setMessage(char_array,n);
	udpObj.sendMessage(message);

	return (doc.GetCell<string>("port", username) + "," + doc.GetCell<string>("ip",username));
}

string directoryServer::getAllImages(Message*msg, directoryServer*ds)
{
	rapidcsv::Document doc(usersFile);
	string imagesNames = "";
	int userCount = doc.GetRowCount();
	for (int i = 0; i < userCount; i++)
	{
		if (doc.GetCell<string>("imageCount", i) == "")
			continue;
		int imageCount = doc.GetCell<int>("imageCount", i);
		for (int j = 0; j < imageCount*2; j+=2)
		{
			if (doc.GetCell<string>(j+6, i) != "")
				imagesNames += (doc.GetRowName(i) + "," + doc.GetCell<string>(j+6, i));
		}
	}

	string images =imagesNames.substr(0, imagesNames.size() - 1);
	int n = images.length(); 
    char *char_array=new char[n+1]; 
    strcpy(char_array, images.c_str()); 

	Message *message = new Message();
	message->setSourceIP(udpObj.getMyIP());
    message->setSourcePort(udpObj.getMyPort());
    message->setRPCID(msg->getRPCId());
    message->setDestinationIP(msg->getSourceIP());
    message->setDestinationPort(msg->getSourcePort());
    message->setOperation(Operation::getAllImages);
	message->setMessageType(MessageType::Reply);
	message->setMessage(char_array,n);
	udpObj.sendMessage(message);

	return imagesNames.substr(0, imagesNames.size() - 1);
}

void directoryServer::listen()
{
    while(true){
        Message *request=udpObj.receiveMsg();
		thread th* = new thread(&directoryServer::doOperation,this,request);
    }
}

void directoryServer::doOperation(Message* request)
{
	int operationID = request->getOperation();
	thread* receiverThread;
	string input = string((char*)request->getMessage());
	vector<string> args = request->getMessageArgs();
	
	if (operationID == Operation::login)
	{
		directoryServer::login(args[0],args[1],request,this);
	}
	else if (operationID == Operation::logout)
	{
		directoryServer::logout(args[0],request,this);
	}
	else if (operationID == Operation::signup)
	{
		directoryServer::signup(args[0],args[1],request,this);
	}
	else if (operationID == Operation::uploadImage)
	{
		directoryServer::uploadimage(args[0],args[0],request,this);
	}
	else if (operationID == Operation::getPortnIP)
	{
		directoryServer::getPortnIP(args[0],request,this);
	}
	else if (operationID == Operation::getAllImages)
	{
		directoryServer::getAllImages(request,this);
	}
}
