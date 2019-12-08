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
       if (reply == "user already logged in")
           return LOGIN_FAIL;
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
    thread * acessRequestThread = new std::thread(& User::serveRequestViews, this);
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

int User :: getImage(string ownerUsername, string imageName, string & image){
    cout << "Message sent from user to DS\n";
    string reply = peer->getPortnIP(token, ownerUsername);
    cout << "Reply received by user from DS\n";
    if (reply == CONN_TIMEOUT)
        return CONN_FAILURE;
    vector <string> args;
    split(reply, args, ',');
    cout << "Message sent to peer\n";
    image = peer->getImage(username, ownerUsername, args[1], stoi(args[0]), imageName);
    cout << "Reply received from peer\n";
    if (image == CONN_TIMEOUT)
        return CONN_FAILURE;
    return MSG_SUCCESS;
}

int User:: getUsersSamples(map<string, vector<imageSample>> & samples){
    int status = getAllImages();
    if (status == MSG_SUCCESS)
        samples =  usersImageSamples;
    return status;
}

int User :: getAllOwnerImages(string ownerUsername, vector <imageSample> &allOwnerImages){
    cout << "Message sent from user to DS\n";
    string reply = peer->getPortnIP(token, ownerUsername);
    if (reply == CONN_TIMEOUT)
        return CONN_FAILURE;
    cout << "Reply received by user from DS\n";
    vector <string> args;
    split(reply, args, ',');
    peer->setTimeOut(60);
    cout << "Message sent to peer\n";
    reply = peer->getAllImagesFromPeer(username, ownerUsername, args[1], stoi(args[0]));
    if (reply == CONN_TIMEOUT)
        return CONN_FAILURE;
    cout << "Reply received from peer\n";
    peer->setTimeOut(20);
    split(reply, args, ',');
    for (int i=0; i < args.size(); i+=2){
        imageSample temp;
        temp.preview = args[i+1];
        temp.imageName = args[i];
        allOwnerImages.push_back(temp);
    }
    return MSG_SUCCESS;
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
        cout << "Listening for access requests" << endl;
        string reply = peer->getImageUpdates();
        emit requestAccessPopUp(reply);
}

int User :: requestImageAccess(string ownerUsername, string imageName){
    cout << "Message sent to peer\n";
    peer->requestImageAccess(token, ownerUsername, imageName);
    return MSG_SUCCESS;
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

int User :: removeImage(string imageName){
    cout << "Message sent to DS\n";
    string reply = peer->removeImage(token, imageName);
    cout << "Reply received from DS\n";

    cout << "Removing image locally\n";
    peer->removeImageLocally(imageName);
    cout << "Removed image locally\n";

    if (reply == CONN_TIMEOUT)
        return CONN_FAILURE;
    if (reply == "image not found")
        return PARAM_ERROR;
    return MSG_SUCCESS;
}

int User:: sendImageAccess(string targetUsername, string imageName, int views){
    cout << "Message sent to peer\n";
    peer->sendImageAccess(token, targetUsername, imageName, views);
    return MSG_SUCCESS;
}

int User :: getOnlineUsers(vector <pair<string, int>> &onlineUsers){
    string reply = peer->getOnlineUsers(token);
    if (reply == "invalid token")
        return PARAM_ERROR;
    vector <string> args;
    split(reply, args, ',');
    for (int i = 0; i < args.size(); i+=2){
        onlineUsers.push_back(pair<string, int>(args[i], stoi(args[i+1])));
    }
    return MSG_SUCCESS;
}
