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

struct imageSample { string imageName; string preview; Image im;};

class User
{
public:
    User(Peer * peer);
    int login(string username, string password);
    int signup(string username, string password);
    string getUsername();
    int uploadPhoto(Image image);
    int logout();
    int getAllImages();
    string getImage(string ownerUsername, string imageName);
    int getUsersSamples(map<string, vector<imageSample>> & samples);
    int getAllOwnerImages(string ownerUsername, vector <imageSample> &allOwnerImages);
    void getMyImages(vector <imageSample> & myPhotos);
    void requestImageAccess(string ownerUsername, string imageName);
    void giveImageAccess(string targetUsername, string imageName, int views);
    void serveRequestViews();

private slots:
    void requestAccessPopUp(string);

signals:
    void requestAccessReceived();



private:
    string username;
    string token;
    Peer * peer;
    map <string, vector <imageSample>> usersImageSamples;
};

#endif // USER_H
