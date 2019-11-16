#include "directoryServer.h"
using namespace std;

directoryServer::directoryServer()
{
	rapidcsv::Document doc(usersFile);
	int totalUsers = doc.GetRowCount();

	//constructing dictionary from csv
	for (int i = 0; i<totalUsers;i++)
	{
		string username = doc.GetRowName(i);
		usersDict[username].password = doc.GetCell<string>(i, 0);
		usersDict[username].online = doc.GetCell<bool>(i, 1);
		usersDict[username].ip = doc.GetCell<string>(i, 2);
		usersDict[username].port = doc.GetCell<unsigned int>(i, 3);
		usersDict[username].token = doc.GetCell<string>(i, 4);
		usersDict[username].imageCount = doc.GetCell<int>(i, 5);

		for (int j = 6; j < 6 + usersDict[username].imageCount * 2; j += 2)
		{
			usersDict[username].imageName.push_back(doc.GetCell<string>(i, j));
			usersDict[username].imageID.push_back(doc.GetCell<string>(i, j+1));
		}
	}
}

directoryServer::~directoryServer()
{
}

void directoryServer::login(string &username, string &password, Message* msg, directoryServer* ds)
{
    bool isAuthenticated = false;
    if (ds->authenticate(username, password))
    {
		rapidcsv::Document doc(usersFile);
        data onlineUser;
		//update struct
		usersDict[username].online = true;
		usersDict[username].port = msg->getPort();
		usersDict[username].ip = msg->getSourceIP();
		usersDict[username].token = username + password;
        
		//update file
		doc.SetCell<bool>("online", username, usersDict[username].online);
		doc.SetCell<unsigned int>("port", username, usersDict[username].port);
		doc.SetCell<string>("ip", username, usersDict[username].ip);
		doc.SetCell<string>("token", username, usersDict[username].token);
		doc.Save();

		isAuthenticated = true;

    }
	//send appropriate reply
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
	ds->usersDict[username].online = false;

	//update file
	doc.SetCell<bool>("online", username, usersDict[username].online);

	//send appropriate reply
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
		doc.Save();
	}
	//send appropriate reply
}

bool directoryServer::usernameExists(string& username)
{
	return usersDict.find(username) != usersDict.end();
}

void directoryServer::uploadimage(string& username, string& imagename, Message* req, directoryServer* ds)
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
	int row;
	for (int i = 0; i < temp.size(); i++)
		if (temp[i] == username)
			int row = i;
	doc.SetCell<string>(6+ usersDict[username].imageCount, row, usersDict[username].imageName.back());
	doc.SetCell<string>(6 + usersDict[username].imageCount+1, row, usersDict[username].imageID.back());
	doc.Save();


}


