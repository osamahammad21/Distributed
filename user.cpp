#include "user.h"

User::User(Peer * peer)
{
    this->peer = peer;
}


bool User :: login(string username, string password){
       string reply = peer->login(username, password);
       if(reply == "not a user")
            return false;
        this->username = username;
        this->password = password;
        this->token = reply;
        peer->startStatusUpdates(this->token);
        return true;
}

bool User :: signup(string username, string password){
    string reply = peer->signup(username, password);
    if(reply == "username already exists")
         return false;
     this->username = username;
     this->password = password;
     this->token = reply;
     peer->startStatusUpdates(this->token);
     return true;
}
