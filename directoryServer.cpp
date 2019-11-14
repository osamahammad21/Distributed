#include "drectoryServer.h"
#include "string.h"
using namespace std;

directoryServer::directoryServer()
{
    infile.open(usersFile);
    if (infile.fail())
    {
        cout<< "Failed to open input file\n";
        exit(1);
    }
    //constructing dictionary 
    infile.seekg(0, ios::beg);
    while (!input.eof())
    {

        infile >> username;
        if (username == "") continue;
            infile >> usersDict[username].password;
            infile >> usersDict[username].online;
            infile >> usersDict[username].ip;
            infile >> usersDict[username].port;
            infile >> usersDict[username].token;//malhash lazma?
            infile >> usersDict[username].imageCount;

            for (int i =0; i<usersDict[username].imageCount;i++)
            {
                infile >> usersDict[username].imageName;
                infile >> usersDict[username].imageID;
            }        
    }
    input.close();
}
void directoryServer::login(string &username, string &password, Message* msg, directoryServer* ds)
{
    bool isAuthenticated = false;
    if (myCS->authenticate(username, password))
    {
        data newUser;
        newUser.online = true;
        newUser.port = msg->getPort();
        newUser.ip = msg->getSourceIP();
        newUser.token = username + password;
        

        ds->usersDict[username] = newUser;
        isAuthenticated = true;
    }
}
bool directoryServer::authenticate(str &username, str &password)
{
    if (usersDict.find(username) != usersDict.end())
        return usersDict[username].password == password;
    return false;
}
