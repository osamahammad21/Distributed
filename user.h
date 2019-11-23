#ifndef USER_H
#define USER_H

using namespace std;
#include <string>
#include "Peer.h"
#include "ImageModel.h"

struct imageSample { string imageName; string preview;};

class User
{
public:
    User(Peer * peer);
    bool login(string username, string password);
    bool signup(string username, string password);
    string getUsername();
    bool uploadPhoto(Image image);
    bool logout();
    bool getAllImages();
    void getAllUsernames(vector <string> & usernames);
    void getSamples(string username, vector <imageSample> & samples);
    void getImage(string ownerUsername, string imageName);
    void getUsersSamples(map<string, vector<imageSample>> & samples);

private:
    string username;
    string token;
    Peer * peer;
    map <string, vector <imageSample>> usersImageSamples;
};

#endif // USER_H
