#include "user.h"

User::User(Peer * peer)
{
    this->peer = peer;
}


bool User :: login(string username, string password){
       cout << username << " " << password << endl;
       string reply = peer->login(username, password);
       cout << reply << endl;
       if(reply == "not a user")
            return false;
        this->username = username;
        this->token = reply;
        peer->startStatusUpdates(this->token);
        return true;
}

bool User :: signup(string username, string password){
    string reply = peer->signup(username, password);
    if(reply == "username already exists")
         return false;
     this->username = username;
     this->token = reply;
     peer->startStatusUpdates(this->token);
     return true;
}

string User :: getUsername(){
    return username;
}

bool User :: uploadPhoto(Image image){
    string imageName;
    image.getImageId(imageName);
    peer->addImageLocally(imageName);
    string photo = image.getSmallScaleImage();
    string reply = peer->uploadImage(this->token, imageName, photo);
    if (reply == "ok"){
        image.removeMiddleFiles();
        return true;
    }
    else
        return false;
}

bool User :: logout(){
    string reply = peer -> logout(token);
    if (reply == "ok"){
        this->username = "";
        this->token = "";
        this->usersImageSamples.clear();
        return true;
    }
    return false;
}

inline void split(string str, vector<string>& cont, char delim = ' ')
{
    stringstream ss(str);
    string token;
    while (getline(ss, token, delim)) {
        cont.push_back(token);
    }
}

bool User:: getAllImages(){
    string reply = peer->getAllImagesFromDS(token);
    vector <string> args;
    if(reply=="no images")
       return false;
    cout<<reply<<endl;
    split(reply, args, ',');
    for (int i=0; i < args.size(); i+=3){
        imageSample temp;
        temp.preview = args[i+2];
        temp.imageName = args[i+1];
        usersImageSamples[args[i]].push_back(temp);
    }
    return true;
}

string User :: getImage(string ownerUsername, string imageName){
    string reply = peer->getPortnIP(token, ownerUsername);
    cout << reply << endl;
    vector <string> args;
    split(reply, args, ',');

    reply = peer->getImage(username, ownerUsername, args[1], stoi(args[0]), imageName);
    return reply;
    cout << reply << endl;
}

void User:: getUsersSamples(map<string, vector<imageSample>> & samples){
    if (getAllImages())
        samples =  usersImageSamples;
}

string User :: getAllOwnerImages(string ownerUsername){
    string reply = peer->getPortnIP(token, ownerUsername);
    vector <string> args;
    split(reply, args, ',');
    cout << username<< " " << ownerUsername << " " << args[1] << " " << args[0] << endl;
    reply = peer->getAllImagesFromPeer(username, ownerUsername, args[1], stoi(args[0]));
    cout << reply;
}

