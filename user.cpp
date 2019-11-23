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
       cout << "HEYHEYHEY " << token << endl;
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

string User :: getUsername(){
    return username;
}

bool User :: uploadPhoto(Image image){
    string imageName;
    image.getImageId(imageName);
    peer->addImageLocally(imageName);
    string reply = peer->uploadImage(this->token, imageName, image.getSmallScaleImage());
    if (reply == "ok")
        return true;
    else
        return false;
}

bool User :: logout(){
//    string reply = peer -> logout(token);
//    if (reply == "ok")
//        return true;
//    return false;

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
    cout << reply << endl;
    vector <string> args;
    split(reply, args, ',');
    for (int i=0; i < args.size(); i+=3){
        imageSample temp;
        temp.preview = args[i+2];
        temp.imageName = args[i+1];
//        usersImageSamples.insert(pair<string, imageSample> (args[i], temp));
        usersImageSamples[args[i]].push_back(temp);
    }
    return true;
}

void User:: getAllUsernames(vector <string> & usernames){
    if(getAllImages()){

        map<string, vector<imageSample>>::iterator it;
        for ( it = usersImageSamples.begin(); it != usersImageSamples.end(); it++ )
        {
            usernames.push_back(it->first);
        }
    }
}

void User :: getSamples(string username, vector <imageSample> & samples){
    map<string, vector<imageSample>>::iterator it;
    for ( it = usersImageSamples.begin(); it != usersImageSamples.end(); it++ )
    {
        if(it->first == username){
            for (int i = 0; i < it->second.size(); i++){
                imageSample temp;
                temp.imageName = it->second[i].imageName;
                temp.preview = it->second[i].preview;
                cout << "preview1 " << temp.preview << endl;
                temp.preview = base64_decode(temp.preview);
                cout << "preview2 " << temp.preview << endl;
                temp.preview = base64_decode(temp.preview);
                cout << "preview3 " << temp.preview << endl;
                samples.push_back(temp);
            }
            return;
        }
    }
}

void User :: getImage(string ownerUsername, string imageName){
    string reply = peer->getPortnIP(token, ownerUsername);
    vector <string> args;
    split(reply, args, ',');
    cout << "reached here " << endl;
    reply = peer->getImage(username, ownerUsername, args[0], stoi(args[1]), imageName);
    cout << reply << endl;
}

