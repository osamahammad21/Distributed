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

struct imageSample { string imageName; string preview; Image im;};

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
    string getImage(string ownerUsername, string imageName);
    void getUsersSamples(map<string, vector<imageSample>> & samples);
    void getAllOwnerImages(string ownerUsername, vector <imageSample> &allOwnerImages);
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
