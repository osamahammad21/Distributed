#ifndef USER_H
#define USER_H

using namespace std;
#include <string>
#include "Peer.h"

class User
{
public:
    User(Peer * peer);
    bool login(string username, string password);
    bool signup(string username, string password);
    string getUsername();
    bool uploadPhoto(Image image);

private:
    string username;
    string password;
    string token;
    Peer * peer;
};

#endif // USER_H
