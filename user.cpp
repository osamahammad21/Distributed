#include "user.h"

User::User(Peer * peer)
{
    this->peer = peer;
    peer->setTimeOut(20);
}

int User :: login(string username, string password){
       cout << "Message sent from user to DS\n";
       string reply = peer->login(username, password);
       cout << "Reply received by user from DS\n";
       if(reply == "not a user")
            return PARAM_ERROR;
       if (reply == CONN_TIMEOUT)
           return CONN_FAILURE;
        this->username = username;
        this->token = reply;
        peer->startStatusUpdates(this->token);
        thread * acessRequestThread = new std::thread(& User::serveRequestViews, this);
        return MSG_SUCCESS;
}

int User :: signup(string username, string password){
    cout << "Message sent from user to DS\n";
    string reply = peer->signup(username, password);
    cout << "Reply received by user from DS\n";
    if(reply == "username already exists")
         return PARAM_ERROR;
    if (reply == CONN_TIMEOUT)
        return CONN_FAILURE;
    this->username = username;
    this->token = reply;
    cout << "Peer started sending status updates\n";
    peer->startStatusUpdates(this->token);
    return MSG_SUCCESS;
}

string User :: getUsername(){
    return username;
}

int User :: uploadPhoto(Image image){
    string imageName;
    image.getImageId(imageName);
    peer->addImageLocally(imageName);
    string photo = image.getSmallScaleImage();
    cout << "Message sent from user to DS\n";
    string reply = peer->uploadImage(this->token, imageName, photo);
    cout << "Reply received by user from DS\n";
    if (reply == "ok"){
        image.removeMiddleFiles();
        return MSG_SUCCESS;
    }
    if (reply == CONN_TIMEOUT)
        return CONN_FAILURE;
    if (reply == "maximum samples reached"){
        cout << "Add image locally" << endl;
        peer->addImageLocally(imageName);
        return MSG_SUCCESS;
    }
    return PARAM_ERROR;

}

int User :: logout(){
    cout << "Message sent from user to DS\n";
    string reply = peer -> logout(token);
    cout << "Reply received by user from DS\n";
    if (reply == "ok"){
        this->username = "";
        this->token = "";
        this->usersImageSamples.clear();
        peer->stopStatusUpdates();
        return MSG_SUCCESS;
    }
    if (reply == CONN_TIMEOUT)
        return CONN_FAILURE;
    return PARAM_ERROR;
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

int User:: getAllImages(){
    cout << "Message sent from user to DS\n";
    string reply = peer->getAllImagesFromDS(token);
    cout << "Reply received by user from DS\n";
    vector <string> args;
    if(reply=="no images")
       return PARAM_ERROR;
    if (reply == CONN_TIMEOUT)
        return CONN_FAILURE;
    split(reply, args, ',');
    usersImageSamples.clear();
    for (int i=0; i < args.size(); i+=3){
        imageSample temp;
        temp.preview = args[i+2];
        temp.imageName = args[i+1];
        usersImageSamples[args[i]].push_back(temp);
    }
    return MSG_SUCCESS;
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

int User:: getUsersSamples(map<string, vector<imageSample>> & samples){
    int status = getAllImages();
    if (status == MSG_SUCCESS)
        samples =  usersImageSamples;
    return status;

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
        cout << "Pop up function" << endl;
        string reply = peer->getImageUpdates();
        emit requestAccessPopUp(reply);
//        vector <string> args;
//        split(reply, args, ',');
//        viewsRequests * popUp = new viewsRequests(peer, token, args[1], args[0], args[2], nullptr);
//        popUp->show();

//        QEventLoop loop;
//        QObject :: connect(popUp, SIGNAL(destroyed()), & loop, SLOT(serveRequestViews()));
//        loop.exec();
//        cout<<"destroyed\n";
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

void User :: requestAccessPopUp(string reply){
    vector <string> args;
    split(reply, args, ',');
    viewsRequests * popUp = new viewsRequests(peer, token, args[1], args[0], args[2], nullptr);
    popUp->show();

            QEventLoop loop;
            QObject :: connect(popUp, SIGNAL(destroyed()), & loop, SLOT(QUIT()));
            thread * acessRequestThread = new std::thread(& User::serveRequestViews, this);
            cout << "destroyed 0\n";
            loop.exec();
            cout << "destroyed\n";
}
