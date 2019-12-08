#include "directoryServer.h"
#include <thread>

using namespace std;

directoryServer::directoryServer(unsigned int port)
{
	string syscall = "sudo snap install avahi";
	system(syscall.c_str());
	syscall = "sudo avahi-set-host-name directoryGroupH";
	system(syscall.c_str());
	mtx.lock();
	rapidcsv::Document doc(usersFile);
	int totalUsers = doc.GetRowCount();
	udpObj.initializeSocket(port);
	//cout<<udpObj.getMyIP()<<endl;
	listen_thread = new thread(&directoryServer::listen, this);
	status_thread = new thread(&directoryServer::decrementStatus, this);

	for (int i = 0; i < totalUsers; i++)
	{
		string username = doc.GetRowName(i);
		usersDict[username].password = doc.GetCell<string>(0, i);
		if (doc.GetCell<string>(1, i) != "")
			usersDict[username].online = 0;
		doc.SetCell<string>(1, i, "0");
		usersDict[username].ip = doc.GetCell<string>(2, i);
		if (doc.GetCell<string>(3, i) != "")
			usersDict[username].port = doc.GetCell<unsigned int>(3, i);
		doc.SetCell<string>(4, i, "");
		usersDict[username].token = "";
		doc.Save();
		if (doc.GetCell<string>(5, i) != "")
			usersDict[username].imageCount = doc.GetCell<int>(5, i);

		for (int j = 6; j < 6 + usersDict[username].imageCount * 2; j += 2)
		{
			usersDict[username].imageName.push_back(doc.GetCell<string>(j, i));
			usersDict[username].image64.push_back(doc.GetCell<string>(j + 1, i));
		}
	}
	mtx.unlock();
}

directoryServer::~directoryServer()
{
}

void directoryServer::login(string &username, string &password, Message *msg, directoryServer *ds)
{
	bool isAuthenticated = false;
	bool bufferFull = false;
	string reply;

	if (ds->authenticate(username, password))
	{
		if (buffer.find(username) != buffer.end())
		{
			bufferFull = true;
		}
		if (ds->checkOnline(username))
		{
			reply = "user already logged in";
		}
		else
		{
			mtx.lock();
			rapidcsv::Document doc(usersFile);
			data onlineUser;
			//update struct
			usersDict[username].online = true;
			usersDict[username].port = msg->getSourcePort();
			usersDict[username].ip = msg->getSourceIP();
			usersDict[username].token = username + password;

			//update file
			doc.SetCell<int>("online", username, usersDict[username].online);
			doc.SetCell<unsigned int>("port", username, usersDict[username].port);
			doc.SetCell<string>("ip", username, usersDict[username].ip);
			doc.SetCell<string>("token", username, usersDict[username].token);
			doc.Save();
			mtx.unlock();
			isAuthenticated = true;
			reply = usersDict[username].token;
		}
	}
	else
	{
		reply = "not a user";
	}

	//send appropriate reply
	int n = reply.length();
	char *char_array = new char[n + 1];
	strcpy(char_array, reply.c_str());

	Message *message = new Message();
	message->setSourceIP(udpObj.getMyIP());
	message->setSourcePort(udpObj.getMyPort());
	message->setRPCID(msg->getRPCId());
	message->setDestinationIP(msg->getSourceIP());
	message->setDestinationPort(msg->getSourcePort());
	message->setOperation(Operation::login);
	message->setMessageType(MessageType::Reply);
	message->setMessage(char_array, n);
	udpObj.sendMessage(message);

	if (bufferFull)
	{
		int buffSize = buffer[username].size();
		for (int i =0;i<buffSize;i++)
		{
			Message *message = new Message();
			if (buffer[username][i]->getOperation() == Operation::requestImageAccess)
			{
				message->setOperation(Operation::requestImageAccess);
			}
			else
			{
				message->setOperation(Operation::addImageAccess);
			}

			int nn = strlen(buffer[username][i]->getMessage());
			message->setSourceIP(buffer[username][i]->getSourceIP());
			message->setSourcePort(buffer[username][i]->getSourcePort());
			message->setRPCID(buffer[username][i]->getRPCId());
			message->setDestinationIP(msg->getSourceIP());
			message->setDestinationPort(msg->getSourcePort());
			message->setMessageType(MessageType::Request);
			message->setMessage(buffer[username][i]->getMessage(), nn);
			udpObj.sendMessage(message);
			cout<<"buffered message sent"<<endl;
			sleep(1);
		}
		buffer.erase(username);
	}
}

bool directoryServer::checkOnline(string &username)
{
	rapidcsv::Document doc(usersFile);
	if (doc.GetCell<string>("online", username) == "1")
		return true;
	return false;
}

bool directoryServer::authenticate(string &username, string &password)
{

	rapidcsv::Document doc(usersFile);
	vector<string> temp = doc.GetRowNames();
	int row = 0;
	for (int i = 0; i < temp.size(); i++)
		if (temp[i] == username)
		{
			if (doc.GetCell<string>("password", temp[i]) == password)
				return true;
		}
	return false;
}

void directoryServer::logout(string &token, Message *msg, directoryServer *ds)
{
	rapidcsv::Document doc(usersFile);
	vector<string> alltokens = doc.GetColumn<string>("token");
	std::vector<string>::iterator it = find(alltokens.begin(), alltokens.end(), token);
	string reply;

	if (it == alltokens.end())
	{
		reply = "invalid token";
		int n = reply.length();
		char *char_array = new char[n + 1];
		strcpy(char_array, reply.c_str());
		Message *message = new Message();
		message->setSourceIP(udpObj.getMyIP());
		message->setSourcePort(udpObj.getMyPort());
		message->setRPCID(msg->getRPCId());
		message->setDestinationIP(msg->getSourceIP());
		message->setDestinationPort(msg->getSourcePort());
		message->setOperation(Operation::logout);
		message->setMessageType(MessageType::Reply);
		message->setMessage(char_array, n);
		udpObj.sendMessage(message);

		return;
	}
	else
	{
		vector<string> temp = doc.GetRowNames();
		string username; //user with the token sent
		int row = 0;
		for (int i = 0; i < temp.size(); i++)
			if (usersDict[temp[i]].token == token)
				username = temp[i];

		if (ds->usernameExists(username))
		{
			mtx.lock();
			ds->usersDict[username].online = 0;
			ds->usersDict[username].token = "";
			//update file
			doc.SetCell<int>("online", username, usersDict[username].online);
			doc.SetCell<string>("token", username, "");

			doc.Save();
			mtx.unlock();
			reply = "ok";
		}
		else
		{
			reply = "not a user";
		}
		int n = reply.length();
		char *char_array = new char[n + 1];
		strcpy(char_array, reply.c_str());
		Message *message = new Message();
		message->setSourceIP(udpObj.getMyIP());
		message->setSourcePort(udpObj.getMyPort());
		message->setRPCID(msg->getRPCId());
		message->setDestinationIP(msg->getSourceIP());
		message->setDestinationPort(msg->getSourcePort());
		message->setOperation(Operation::logout);
		message->setMessageType(MessageType::Reply);
		message->setMessage(char_array, n);
		udpObj.sendMessage(message);
	}
}
void directoryServer::signup(string &username, string &password, Message *msg, directoryServer *ds)
{

	//update struct and file with new user
	if (!(usernameExists(username)))
	{
		mtx.lock();
		rapidcsv::Document doc(usersFile);
		usersDict[username].password = password;
		doc.SetCell<string>(0, doc.GetRowCount(), password);
		doc.SetRowName(doc.GetRowCount() - 1, username);
		doc.Save();
		mtx.unlock();
		//login after signup
		login(username, password, msg, ds);
	}
	else
	{
		string reply = "username already exists";
		int n = reply.length();
		char *char_array = new char[n + 1];
		strcpy(char_array, reply.c_str());
		Message *message = new Message();
		message->setSourceIP(udpObj.getMyIP());
		message->setSourcePort(udpObj.getMyPort());
		message->setRPCID(msg->getRPCId());
		message->setDestinationIP(msg->getSourceIP());
		message->setDestinationPort(msg->getSourcePort());
		message->setOperation(Operation::signup);
		message->setMessageType(MessageType::Reply);
		message->setMessage(char_array, n);
		udpObj.sendMessage(message);
	}
}

bool directoryServer::usernameExists(string &username)
{
	return (usersDict.find(username) != usersDict.end());
}

void directoryServer::uploadimage(string &token, string &imagename, string &image64, Message *msg, directoryServer *ds)
{

	mtx.lock();
	string username; //user with the token sent
	rapidcsv::Document doc(usersFile);
	vector<string> alltokens = doc.GetColumn<string>("token");
	std::vector<string>::iterator it = find(alltokens.begin(), alltokens.end(), token);
	mtx.unlock();
	if (it == alltokens.end())
	{
		string ok = "invalid token";
		int n = ok.length();
		char *char_array = new char[n + 1];
		strcpy(char_array, ok.c_str());

		Message *message = new Message();
		message->setSourceIP(udpObj.getMyIP());
		message->setSourcePort(udpObj.getMyPort());
		message->setRPCID(msg->getRPCId());
		message->setDestinationIP(msg->getSourceIP());
		message->setDestinationPort(msg->getSourcePort());
		message->setOperation(Operation::uploadImage);
		message->setMessageType(MessageType::Reply);
		message->setMessage(char_array, n);
		udpObj.sendMessage(message);

		return;
	}
	else
	{
		mtx.lock();
		vector<string> temp = doc.GetRowNames();
		int row = 0;
		for (int i = 0; i < temp.size(); i++)
			if (usersDict[temp[i]].token == token)
				username = temp[i];
		mtx.unlock();

		string count = doc.GetCell<string>("imageCount", username);
		if (count != "")
		{
			int count_int = doc.GetCell<int>("imageCount", username);
			if (count_int >= 3)
			{
				string ok = "maximum samples reached";
				cout << "maxmimum samples reached" << endl;
				int n = ok.length();
				char *char_array = new char[n + 1];
				strcpy(char_array, ok.c_str());

				Message *message = new Message();
				message->setSourceIP(udpObj.getMyIP());
				message->setSourcePort(udpObj.getMyPort());
				message->setRPCID(msg->getRPCId());
				message->setDestinationIP(msg->getSourceIP());
				message->setDestinationPort(msg->getSourcePort());
				message->setOperation(Operation::uploadImage);
				message->setMessageType(MessageType::Reply);
				message->setMessage(char_array, n);
				udpObj.sendMessage(message);

				return;
			}
		}

		mtx.lock();
		ds->usersDict[username].imageCount += 1;
		ds->usersDict[username].image64.push_back(image64);
		ds->usersDict[username].imageName.push_back(imagename);

		//add to users.csv
		doc.SetCell<int>("imageCount", username, usersDict[username].imageCount);
		temp = doc.GetRowNames();
		row = 0;
		for (int i = 0; i < temp.size(); i++)
			if (temp[i] == username)
				row = i;

		doc.SetCell<string>(4 + usersDict[username].imageCount * 2, row, usersDict[username].imageName.back());
		doc.SetCell<string>(4 + usersDict[username].imageCount * 2 + 1, row, usersDict[username].image64.back());
		doc.Save();
		mtx.unlock();

		string ok = "ok";
		int n = ok.length();
		char *char_array = new char[n + 1];
		strcpy(char_array, ok.c_str());

		Message *message = new Message();
		message->setSourceIP(udpObj.getMyIP());
		message->setSourcePort(udpObj.getMyPort());
		message->setRPCID(msg->getRPCId());
		message->setDestinationIP(msg->getSourceIP());
		message->setDestinationPort(msg->getSourcePort());
		message->setOperation(Operation::uploadImage);
		message->setMessageType(MessageType::Reply);
		message->setMessage(char_array, n);
		udpObj.sendMessage(message);
		return;
	}
}
void directoryServer::removeImage(string &token, string &imagename, Message *msg, directoryServer *ds)
{
	mtx.lock();
	string username = ""; //user with the token sent
	bool found = false;
	rapidcsv::Document doc(usersFile);
	vector<string> alltokens = doc.GetColumn<string>("token");
	std::vector<string>::iterator it = find(alltokens.begin(), alltokens.end(), token);
	mtx.unlock();
	if (it == alltokens.end())
	{
		string ok = "invalid token";
		int n = ok.length();
		char *char_array = new char[n + 1];
		strcpy(char_array, ok.c_str());

		Message *message = new Message();
		message->setSourceIP(udpObj.getMyIP());
		message->setSourcePort(udpObj.getMyPort());
		message->setRPCID(msg->getRPCId());
		message->setDestinationIP(msg->getSourceIP());
		message->setDestinationPort(msg->getSourcePort());
		message->setOperation(Operation::removeImage);
		message->setMessageType(MessageType::Reply);
		message->setMessage(char_array, n);
		udpObj.sendMessage(message);
		return;
	}
	else
	{
		mtx.lock();
		vector<string> temp = doc.GetRowNames();
		for (int i = 0; i < temp.size(); i++)
			if (usersDict[temp[i]].token == token)
				username = temp[i];
		mtx.unlock();
		if (username == "")
			return;
		if (usersDict[username].imageCount == NULL)
		{
			string ok = "image not found";
			int n = ok.length();
			char *char_array = new char[n + 1];
			strcpy(char_array, ok.c_str());

			Message *message = new Message();
			message->setSourceIP(udpObj.getMyIP());
			message->setSourcePort(udpObj.getMyPort());
			message->setRPCID(msg->getRPCId());
			message->setDestinationIP(msg->getSourceIP());
			message->setDestinationPort(msg->getSourcePort());
			message->setOperation(Operation::removeImage);
			message->setMessageType(MessageType::Reply);
			message->setMessage(char_array, n);
			udpObj.sendMessage(message);
			return;
		}
		int index = 0;
		for (int j = 6; j < 6 + usersDict[username].imageCount * 2; j += 2)
		{
			if (doc.GetCell<string>(j, username) == imagename)
			{
				for (int i = 0; i < temp.size(); i++)
				{
					if (doc.GetRowName(i) == username)
					{
						index = i;
					}
				}
				mtx.lock();
				doc.SetCell<string>(j, index, "");
				doc.SetCell<string>(j + 1, index, "");
				doc.SetCell<int>("imageCount", username, usersDict[username].imageCount - 1);
				usersDict[username].imageName.erase(usersDict[username].imageName.begin() + j);
				usersDict[username].image64.erase(usersDict[username].image64.begin() + j);
				usersDict[username].imageCount--;
				found = true;
				doc.Save();
				mtx.unlock();
			}
			if (found)
				break;
		}
		if (!found)
		{
			string ok = "image not found";
			int n = ok.length();
			char *char_array = new char[n + 1];
			strcpy(char_array, ok.c_str());

			Message *message = new Message();
			message->setSourceIP(udpObj.getMyIP());
			message->setSourcePort(udpObj.getMyPort());
			message->setRPCID(msg->getRPCId());
			message->setDestinationIP(msg->getSourceIP());
			message->setDestinationPort(msg->getSourcePort());
			message->setOperation(Operation::removeImage);
			message->setMessageType(MessageType::Reply);
			message->setMessage(char_array, n);
			udpObj.sendMessage(message);
			return;
		}

		string ok = "image removed";
		int n = ok.length();
		char *char_array = new char[n + 1];
		strcpy(char_array, ok.c_str());

		Message *message = new Message();
		message->setSourceIP(udpObj.getMyIP());
		message->setSourcePort(udpObj.getMyPort());
		message->setRPCID(msg->getRPCId());
		message->setDestinationIP(msg->getSourceIP());
		message->setDestinationPort(msg->getSourcePort());
		message->setOperation(Operation::removeImage);
		message->setMessageType(MessageType::Reply);
		message->setMessage(char_array, n);
		udpObj.sendMessage(message);
		return;
	}
}
string directoryServer::getPortnIP(string &token, string &username, Message *msg, directoryServer *ds)
{
	rapidcsv::Document doc(usersFile);
	string pandip = "";
	vector<string> alltokens = doc.GetColumn<string>("token");
	std::vector<string>::iterator it = find(alltokens.begin(), alltokens.end(), token);
	if (it == alltokens.end())
	{
		pandip = "invalid token";
	}
	else
	{
		if (usernameExists(username))
		{
			pandip = doc.GetCell<string>("port", username) + "," + doc.GetCell<string>("ip", username);	
		}
		else
		{
			pandip = "not a user";
		}
	}
	int n = pandip.length();
	char *char_array = new char[n + 1];
	strcpy(char_array, pandip.c_str());

	Message *message = new Message();
	message->setSourceIP(udpObj.getMyIP());
	message->setSourcePort(udpObj.getMyPort());
	message->setRPCID(msg->getRPCId());
	message->setDestinationIP(msg->getSourceIP());
	message->setDestinationPort(msg->getSourcePort());
	message->setOperation(Operation::getPortnIP);
	message->setMessageType(MessageType::Reply);
	message->setMessage(char_array, n);
	udpObj.sendMessage(message);

	return pandip;
}

string directoryServer::getAllImages(string &token, Message *msg, directoryServer *ds)
{

	rapidcsv::Document doc(usersFile);
	string images = "";
	vector<string> alltokens = doc.GetColumn<string>("token");
	std::vector<string>::iterator it = find(alltokens.begin(), alltokens.end(), token);
	if (it == alltokens.end())
	{
		images = "invalid token";
	}
	else
	{
		if (doc.GetColumnCount() <= 7)
		{
			images = "no images";
		}
		else
		{
			string imagesNames = "";
			int userCount = doc.GetRowCount();
			for (int i = 0; i < userCount; i++)
			{
				if (doc.GetCell<string>("imageCount", i) == "")
					continue;
				int imageCount = doc.GetCell<int>("imageCount", i);
				for (int j = 0; j < int(doc.GetColumnCount() - 6); j += 2)
				{
					if (doc.GetCell<string>(j + 6, i) != "")
						imagesNames += (doc.GetRowName(i) + "," + doc.GetCell<string>(j + 6, i) + "," + doc.GetCell<string>(j + 7, i)) + ",";
				}
			}
			images = imagesNames.substr(0, imagesNames.size() - 1);
		}
	}
	int n = images.length();
	char *char_array = new char[n + 1];
	strcpy(char_array, images.c_str());

	Message *message = new Message();
	message->setSourceIP(udpObj.getMyIP());
	message->setSourcePort(udpObj.getMyPort());
	message->setRPCID(msg->getRPCId());
	message->setDestinationIP(msg->getSourceIP());
	message->setDestinationPort(msg->getSourcePort());
	message->setOperation(Operation::getAllImages);
	message->setMessageType(MessageType::Reply);
	message->setMessage(char_array, n);
	udpObj.sendMessage(message);

	return images;
}

string directoryServer::getOnlineUsers(string &token, Message *msg, directoryServer *ds)
{
	rapidcsv::Document doc(usersFile);
	string onlineUsers = "";
	string userString = "";
	vector<string> alltokens = doc.GetColumn<string>("token");
	std::vector<string>::iterator it = find(alltokens.begin(), alltokens.end(), token);
	if (it == alltokens.end())
	{
		userString = "invalid token";
	}
	else
	{
		int userCount = doc.GetRowCount();
		for (int i = 0; i < userCount; i++)
		{
			onlineUsers += doc.GetRowName(i) + "," + doc.GetCell<string>("online", i) + ",";
		}
		userString = onlineUsers.substr(0, onlineUsers.size() - 1);
	}

	int n = userString.length();
	char *char_array = new char[n + 1];
	strcpy(char_array, userString.c_str());

	Message *message = new Message();
	message->setSourceIP(udpObj.getMyIP());
	message->setSourcePort(udpObj.getMyPort());
	message->setRPCID(msg->getRPCId());
	message->setDestinationIP(msg->getSourceIP());
	message->setDestinationPort(msg->getSourcePort());
	message->setOperation(Operation::getOnlineUsers);
	message->setMessageType(MessageType::Reply);
	message->setMessage(char_array, n);
	udpObj.sendMessage(message);

	return userString;
}

void directoryServer::listen()
{
	while (true)
	{
		Message *request = udpObj.receiveMsg();
		//cout<< request->getMessageArgs()[0]<<endl;
		thread *th = new thread(&directoryServer::doOperation, this, request);
	}
}

void directoryServer::updateStatus(string &token, directoryServer *ds)
{
	rapidcsv::Document doc(usersFile);
	vector<string> temp = doc.GetRowNames();
	string username; //user with the token sent
	int row = 0;
	for (int i = 0; i < temp.size(); i++)
		if (usersDict[temp[i]].token == token)
			username = temp[i];

	mtxStatus.lock();
	statusDict[username] = TIMEOUT; //TIMEOUT defined in directoryServer.h (30 seconds)
	mtxStatus.unlock();
	//cout<<"status for user:"<<username<<"updated"<<endl;
}

void directoryServer::decrementStatus()
{
	while (true)
	{
		for (auto &x : statusDict)
		{
			mtxStatus.lock();
			x.second--;		   //decrement TIMEOUT
			if (x.second == 0) //user no longer online
			{
				//remove entry from SatusDict
				usersDict.erase(x.first);
				//set as offline in users.csv
				mtx.lock();
				rapidcsv::Document doc(usersFile);
				usersDict[x.first].online = 0;
				doc.SetCell<string>("online", x.first, "0");
				doc.SetCell<string>("token", x.first, "");

				doc.Save();
				mtx.unlock();
			}
			mtxStatus.unlock();
		}
		usleep(1000000); //usleep takes microseconds
	}
}

void directoryServer::clearUsers()
{
}

void directoryServer::requestImageAccess(string &token, string &ownerusername, string &imagename, Message *msg, directoryServer *ds)
{
	rapidcsv::Document doc(usersFile);
	string username; //user with the token sent
	if (ds->usernameExists(ownerusername))
	{
		if (ds->checkOnline(ownerusername))
		{
			vector<string> alltokens = doc.GetColumn<string>("token");
			std::vector<string>::iterator it = find(alltokens.begin(), alltokens.end(), token);
			if (it != alltokens.end())
			{
				string forward = "";
				vector<string> temp = doc.GetRowNames();
				int row = 0;
				for (int i = 0; i < temp.size(); i++)
					if (usersDict[temp[i]].token == token)
						username = temp[i];
				forward = username + "," + ownerusername + "," + imagename;

				int n = forward.length();
				char *char_array = new char[n + 1];
				strcpy(char_array, forward.c_str());
				Message *message = new Message();
				message->setSourceIP(msg->getSourceIP());
				message->setSourcePort(msg->getSourcePort());
				message->setRPCID(msg->getRPCId());
				message->setDestinationIP(doc.GetCell<string>("ip", ownerusername));
				message->setDestinationPort(stoi(doc.GetCell<string>("port", ownerusername)));
				message->setOperation(Operation::requestImageAccess);
				message->setMessageType(MessageType::Request);
				message->setMessage(char_array, n);
				udpObj.sendMessage(message);
				cout<<"message sent"<<endl;
				return;
			}
			else
			{
				return;
			}
		}
		else
		{
			//buffer message
			mtx.lock();
			buffer[ownerusername].push_back(msg);
			mtx.unlock();
		}
	}
}

void directoryServer::addImageAccess(string &token, string &target, string &imagename, string &addedViews, Message *msg, directoryServer *ds)
{
	rapidcsv::Document doc(usersFile);
	string username; //user with the token sent
	if (ds->usernameExists(target))
	{
		if (ds->checkOnline(target))
		{
			vector<string> alltokens = doc.GetColumn<string>("token");
			std::vector<string>::iterator it = find(alltokens.begin(), alltokens.end(), token);
			if (it != alltokens.end())
			{
				string forward = "";
				vector<string> temp = doc.GetRowNames();
				int row = 0;
				for (int i = 0; i < temp.size(); i++)
					if (usersDict[temp[i]].token == token)
						username = temp[i];

				forward = username + "," + target + "," + imagename + "," + addedViews;
				int n = forward.length();
				char *char_array = new char[n + 1];
				strcpy(char_array, forward.c_str());
				Message *message = new Message();
				message->setSourceIP(msg->getSourceIP());
				message->setSourcePort(msg->getSourcePort());
				message->setRPCID(msg->getRPCId());
				message->setDestinationIP(doc.GetCell<string>("ip", target));
				message->setDestinationPort(stoi(doc.GetCell<string>("port", target)));
				message->setOperation(Operation::addImageAccess);
				message->setMessageType(MessageType::Request);
				message->setMessage(char_array, n);
				udpObj.sendMessage(message);
				cout<<"message sent"<<endl;
				return;
			}
			else
			{
				return;
			}
		}
		else
		{
			//buffer message
			mtx.lock();
			buffer[target].push_back(msg);
			mtx.unlock();
		}
	}
}
void directoryServer::doOperation(Message *request)
{
	int operationID = request->getOperation();
	thread *receiverThread;
	string input = string((char *)request->getMessage());
	vector<string> args = request->getMessageArgs();

	if (operationID == Operation::login)
	{
		directoryServer::login(args[0], args[1], request, this);
	}
	else if (operationID == Operation::logout)
	{
		directoryServer::logout(args[0], request, this);
	}
	else if (operationID == Operation::signup)
	{
		directoryServer::signup(args[0], args[1], request, this);
	}
	else if (operationID == Operation::uploadImage)
	{
		directoryServer::uploadimage(args[0], args[1], args[2], request, this);
	}
	else if (operationID == Operation::getPortnIP)
	{
		directoryServer::getPortnIP(args[0], args[1], request, this);
	}
	else if (operationID == Operation::getAllImages)
	{
		directoryServer::getAllImages(args[0], request, this);
	}
	else if (operationID == Operation::updateStatus)
	{
		directoryServer::updateStatus(args[0], this);
	}
	else if (operationID == Operation::removeImage)
	{
		directoryServer::removeImage(args[0], args[1], request, this);
	}
	else if (operationID == Operation::getOnlineUsers)
	{
		directoryServer::getOnlineUsers(args[0], request, this);
	}
	else if (operationID == Operation::requestImageAccess)
	{
		directoryServer::requestImageAccess(args[0], args[1], args[2],request, this);
	}
	else if (operationID == Operation::addImageAccess)
	{
		directoryServer::addImageAccess(args[0], args[1], args[2], args[3],request, this);
	}
}
