#ifndef USER_H
#define USER_H

using namespace std;
#include <string>
#include "Peer.h"
#include "ImageModel.h"
#include <fstream>
#include "viewsrequests.h"
#include <QEventLoop>
#include <QObject>

#define CONN_TIMEOUT "connection timeout"
#define CONN_FAILURE -1
#define MSG_SUCCESS 1
#define PARAM_ERROR 0
#define LOGIN_FAIL -2

struct imageSample { string imageName; string preview; Image im;};

class User
{
public:
    struct accessRequester{
        string username;
        string imageName;
        int id;
    };
    vector <accessRequester> requesters;
    User(Peer * peer);
    int login(string username, string password);
    int signup(string username, string password);
    string getUsername();
    int uploadPhoto(Image image);
    int logout();
    int getAllImages();
    int getImage(string ownerUsername, string imageName, string & image);
    int getUsersSamples(map<string, vector<imageSample>> & samples);
    int getAllOwnerImages(string ownerUsername, vector <imageSample> &allOwnerImages);
    void getMyImages(vector <imageSample> & myPhotos);
    int requestImageAccess(string ownerUsername, string imageName);
    void serveRequestViews();
    void requestAccessPopUp(string);
    int removeImage(string imageName);
    int sendImageAccess(string targetUsername, string imageName, int views);
    int getOnlineUsers(vector <pair<string, int>> &onlineUsers);

private:
    string username;
    string token;
    Peer * peer;
    map <string, vector <imageSample>> usersImageSamples;
    int accessRequests;
};

#endif // USER_H
