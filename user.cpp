#include "user.h"

User::User(Peer * peer)
{
    this->peer = peer;

    thread * acessRequestThread = new std::thread(& User::serveRequestViews, this);
}

bool User :: login(string username, string password){
       cout << "Message sent from user to DS\n";
       string reply = peer->login(username, password);
       cout << "Reply received by user from DS\n";
       if(reply == "not a user")
            return false;
        this->username = username;
        this->token = reply;
        peer->startStatusUpdates(this->token);
        return true;
}

bool User :: signup(string username, string password){
    cout << "Message sent from user to DS\n";
    string reply = peer->signup(username, password);
    cout << "Reply received by user from DS\n";
    if(reply == "username already exists")
         return false;
     this->username = username;
     this->token = reply;
     cout << "Peer started sending status updates\n";
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
    cout << "Message sent from user to DS\n";
    string reply = peer->uploadImage(this->token, imageName, photo);
    cout << "Reply received by user from DS\n";
    if (reply == "ok"){
        image.removeMiddleFiles();
        return true;
    }
    else
        return false;

}

bool User :: logout(){
    cout << "Message sent from user to DS\n";
    string reply = peer -> logout(token);
    cout << "Reply received by user from DS\n";
    if (reply == "ok"){
        this->username = "";
        this->token = "";
        this->usersImageSamples.clear();
        peer->stopStatusUpdates();
        return true;
    }
    return false;
}

inline void split(string str, vector<string>& cont, char delim = ' ')
{
    try {
      cont.clear();
    } catch (exception e) {

    }

    stringstream ss(str);
    string token;
    while (getline(ss, token, delim)) {
        cont.push_back(token);
    }
}

bool User:: getAllImages(){
    cout << "Message sent from user to DS\n";
    string reply = peer->getAllImagesFromDS(token);
    cout << "Reply received by user from DS\n";
    vector <string> args;
    if(reply=="no images")
       return false;
    split(reply, args, ',');
    usersImageSamples.clear();
    for (int i=0; i < args.size(); i+=3){
        imageSample temp;
        temp.preview = args[i+2];
        temp.imageName = args[i+1];
        usersImageSamples[args[i]].push_back(temp);
    }
    return true;
}

string User :: getImage(string ownerUsername, string imageName){
    cout << "Message sent from user to DS\n";
    string reply = peer->getPortnIP(token, ownerUsername);
    cout << "Reply received by user from DS\n";
    vector <string> args;
    split(reply, args, ',');
    cout << "Message sent to peer\n";
    reply = peer->getImage(username, ownerUsername, args[1], stoi(args[0]), imageName);
    cout << "Reply received from peer\n";
    return reply;
}

void User:: getUsersSamples(map<string, vector<imageSample>> & samples){
    if (getAllImages())
        samples =  usersImageSamples;
}

void User :: getAllOwnerImages(string ownerUsername, vector <imageSample> &allOwnerImages){
    cout << "Message sent from user to DS\n";
    string reply = peer->getPortnIP(token, ownerUsername);
    cout << "Reply received by user from DS\n";
    vector <string> args;
    split(reply, args, ',');
    cout << "Message sent to peer\n";
    reply = peer->getAllImagesFromPeer(username, ownerUsername, args[1], stoi(args[0]));
    cout << "Reply received from peer\n";

    split(reply, args, ',');
    for (int i=0; i < args.size(); i+=2){
        imageSample temp;
        temp.preview = args[i+1];
        temp.imageName = args[i];
        allOwnerImages.push_back(temp);
    }
}
void User :: getMyImages(vector <imageSample> & myPhotos){
    ifstream in (username+".txt");
    if (!in.is_open()){
        cout << "Error opening input file" << endl;
    }
    string imageName;
    do{
        in >> imageName;
        Image i;
        i.setImageDir(username);
        if (i.findImage(username, imageName)){
            imageSample temp;
            temp.imageName = imageName;
            i.desteg();
            temp.preview = i.getSmallScaleImage();
            temp.im = i;
            myPhotos.push_back(temp);
        }
    }while (!in.eof());
}
void User :: serveRequestViews(){
    while (1) {
        string reply = peer->getImageUpdates();
        vector <string> args;
        split(reply, args, ',');
        string requesterUsername, imageName;
        viewsRequests * popUp = new viewsRequests(peer, token, args[1], args[0], args[2], nullptr);
        popUp->show();
        QEventLoop loop;
        QObject :: connect(popUp, SIGNAL(destroyed()), & loop, SLOT(quit()));
        loop.exec();
        cout<<"destroyed\n";
    }
}

void User :: requestImageAccess(string ownerUsername, string imageName){
    cout << "Message sent from user to DS\n";
    string reply = peer->getPortnIP(token, ownerUsername);
    cout << "Reply received by user from DS\n";
    vector <string> args;
    split(reply, args, ',');
    cout << "Message sent to peer\n";
    peer->requestImageAccess(username, ownerUsername, args[1],stoi(args[0]),imageName);
}
