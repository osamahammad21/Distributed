#include "Peer.h"
inline bool existf(string name) {
    ifstream f(name.c_str());
    if(f.good())
    {
        f.close();
        return true;
    }
    // f.close();
    return false;
}
//Peer constructor
Peer :: Peer(int port){
    //create client socket
    sock.initializeSocket(port);
    read_thread = new std::thread(&Peer::listen,this);
    serve_thread = new std::thread(&Peer::serve,this);
    if(existf(string(IMAGES_DB_PATH)))
    {
        ifstream imagesFile;
        imagesFile.open(IMAGES_DB_PATH);
        while(!imagesFile.eof())
        {
            string input;
            imagesFile>>input;
            myImages.push_back(input);
        }
        imagesFile.close();
    }else
    {
        ofstream imagesFile;
        imagesFile.open(IMAGES_DB_PATH);
        imagesFile.close();
    }
    
}
void Peer::startStatusUpdates(string token)
{
    status_thread = new std::thread(&Peer::status,this,token);
}
void Peer::setDS(string ip,int port)
{
    dsaddr=ip;
    dsport=port;
}
void Peer::addImageLocally(string imageId)
{
    for(int i = 0;i<myImages.size();i++)
    {
        if(myImages[i] == imageId){
            return;
        }
    }
    myImages.push_back(imageId);
    ofstream imagesFile;
    imagesFile.open(IMAGES_DB_PATH, ios::out | ios::trunc);
    for(int i = 0;i<myImages.size();i++)
    {
        if(i==myImages.size()-1)
        {
            imagesFile<<myImages[i];
        }else
        {
            imagesFile<<myImages[i]<<endl;
        }
        
    }
    imagesFile.close();
}
void Peer::removeImageLocally(string imageId)
{
    for(vector<string>::iterator it = myImages.begin();it!=myImages.end();it++)
    {
        if(*it == imageId){
            myImages.erase(it);
            break;
        }
    }
    ofstream imagesFile;
    imagesFile.open(IMAGES_DB_PATH, ios::out | ios::trunc);
    for(int i = 0;i<myImages.size();i++)
    {
        if(i==myImages.size()-1)
        {
            imagesFile<<myImages[i];
        }else
        {
            imagesFile<<myImages[i]<<endl;
        }
        
    }
    imagesFile.close();
}
//Before Auth
string Peer::login(string username,string password)
{
    rpcidmtx.lock();
    int rpcId = rpccount++;
    rpcidmtx.unlock();
    Message *message = new Message();
    message->setSourceIP(sock.getMyIP());
    message->setSourcePort(sock.getMyPort());
    message->setRPCID(rpcId);
    message->setDestinationIP(dsaddr);
    message->setDestinationPort(dsport);
    message->setOperation(Operation::login);
    string request=username+DELIM+password;
    int n = request.length(); 
    char *char_array=new char[n+1]; 
    strcpy(char_array, request.c_str()); 
    message->setMessage(char_array,n);
    message->setMessageType(MessageType::Request);

    cout << "Message is: " << message->getMessage() << endl;
    while(!sock.sendMessage(message)){}
    while(true)
    {
        if(replyMessages.find(rpcId)!=replyMessages.end())
        {
            if(replyMessages[rpcId]->getMessageType()==MessageType::Reply)
            {
                string s(replyMessages[rpcId]->getMessage());
                cout << "Reply is: " << s << endl;
                return s;
            }
        }
    }
}
string Peer::signup(string username,string password)
{
    rpcidmtx.lock();
    int rpcId = rpccount++;
    rpcidmtx.unlock();
    Message *message = new Message();
    message->setSourceIP(sock.getMyIP());
    message->setSourcePort(sock.getMyPort());
    message->setRPCID(rpcId);
    message->setDestinationIP(dsaddr);
    message->setDestinationPort(dsport);
    message->setOperation(Operation::signup);
    string request=username+DELIM+password;
    int n = request.length(); 
    char *char_array=new char[n+1]; 
    strcpy(char_array, request.c_str()); 
    message->setMessage(char_array,n);
    message->setMessageType(MessageType::Request);
    cout << "Message is: " << message->getMessage() << endl;
    while(!sock.sendMessage(message)){}
    while(true)
    {
        if(replyMessages.find(rpcId)!=replyMessages.end())
        {
            if(replyMessages[rpcId]->getMessageType()==MessageType::Reply)
            {
                string s(replyMessages[rpcId]->getMessage());
                cout << "Reply is: " << s << endl;
                return s;
            }
        }
    }
}
//After Auth: Use token
string Peer::uploadImage(string token,string imagename,string image64)
{
    rpcidmtx.lock();
    int rpcId = rpccount++;
    rpcidmtx.unlock();
    Message *message = new Message();
    message->setSourceIP(sock.getMyIP());
    message->setSourcePort(sock.getMyPort());
    message->setRPCID(rpcId);
    message->setDestinationIP(dsaddr);
    message->setDestinationPort(dsport);
    message->setOperation(Operation::uploadImage);
    string request=token+DELIM+imagename+DELIM+image64;
    int n = request.length(); 
    char *char_array=new char[n+1]; 
    strcpy(char_array, request.c_str()); 
    message->setMessage(char_array,n);
    message->setMessageType(MessageType::Request);

    cout << "Message is: " << message->getMessage() << endl;
    while(!sock.sendMessage(message)){}
    while(true)
    {
        if(replyMessages.find(rpcId)!=replyMessages.end())
        {
            if(replyMessages[rpcId]->getMessageType()==MessageType::Reply)
            {
                string s(replyMessages[rpcId]->getMessage());
                cout << "Reply is: " << s << endl;
                return s;
            }
        }
    }
}
string Peer::removeImage(string token,string imagename)
{
    rpcidmtx.lock();
    int rpcId = rpccount++;
    rpcidmtx.unlock();
    Message *message = new Message();
    message->setSourceIP(sock.getMyIP());
    message->setSourcePort(sock.getMyPort());
    message->setRPCID(rpcId);
    message->setDestinationIP(dsaddr);
    message->setDestinationPort(dsport);
    message->setOperation(Operation::removeImage);
    string request=token+DELIM+imagename;
    int n = request.length(); 
    char *char_array=new char[n+1]; 
    strcpy(char_array, request.c_str()); 
    message->setMessage(char_array,n);
    message->setMessageType(MessageType::Request);
    while(!sock.sendMessage(message)){}
    while(true)
    {
        if(replyMessages.find(rpcId)!=replyMessages.end())
        {
            if(replyMessages[rpcId]->getMessageType()==MessageType::Reply)
            {
                string s(replyMessages[rpcId]->getMessage());
                return s;
            }
        }
    }
}
//care that each image is encoded64 twice on the Peer level so you need to decode it twice to use it
string Peer::getAllImagesFromDS(string token)
{
    rpcidmtx.lock();
    int rpcId = rpccount++;
    rpcidmtx.unlock();
    Message *message = new Message();
    message->setSourceIP(sock.getMyIP());
    message->setSourcePort(sock.getMyPort());
    message->setRPCID(rpcId);
    message->setDestinationIP(dsaddr);
    message->setDestinationPort(dsport);
    message->setOperation(Operation::getAllImages);
    string request = token;
    int n = request.length(); 
    char *char_array=new char[n+1]; 
    strcpy(char_array, request.c_str()); 
    message->setMessage(char_array,n);
    message->setMessageType(MessageType::Request);
    while(!sock.sendMessage(message)){}
    while(true)
    {
        if(replyMessages.find(rpcId)!=replyMessages.end())
        {
            if(replyMessages[rpcId]->getMessageType()==MessageType::Reply)
            {
                string s(replyMessages[rpcId]->getMessage());
                return s;
            }
        }
    }
}
//care that each image is encoded64 twice on the Peer level so you need to decode it twice to use it
string Peer::getAllImagesFromPeer(string myusername,string targetusername,string ip,int port)
{
    rpcidmtx.lock();
    int rpcId = rpccount++;
    rpcidmtx.unlock();
    Message *message = new Message();
    message->setSourceIP(sock.getMyIP());
    message->setSourcePort(sock.getMyPort());
    message->setRPCID(rpcId);
    message->setDestinationIP(ip);
    message->setDestinationPort(port);
    message->setOperation(Operation::getThumbnails);
    string request = myusername+DELIM+targetusername;
    int n = request.length(); 
    char *char_array=new char[n+1]; 
    strcpy(char_array, request.c_str()); 
    message->setMessage(char_array,n);
    message->setMessageType(MessageType::Request);
    while(!sock.sendMessage(message)){}
    cout<<"Message Sent"<<endl;
    while(true)
    {
        if(replyMessages.find(rpcId)!=replyMessages.end())
        {
            if(replyMessages[rpcId]->getMessageType()==MessageType::Reply)
            {
                string s(replyMessages[rpcId]->getMessage());
                return s;
            }
        }
    }
}
string Peer::getPortnIP(string token,string targetusername)
{
    rpcidmtx.lock();
    int rpcId = rpccount++;
    rpcidmtx.unlock();
    Message *message = new Message();
    message->setSourceIP(sock.getMyIP());
    message->setSourcePort(sock.getMyPort());
    message->setRPCID(rpcId);
    message->setDestinationIP(dsaddr);
    message->setDestinationPort(dsport);
    message->setOperation(Operation::getPortnIP);
    string request = token+DELIM+targetusername;
    int n = request.length(); 
    char *char_array=new char[n+1]; 
    strcpy(char_array, request.c_str()); 
    message->setMessage(char_array,n);
    message->setMessageType(MessageType::Request);
    while(!sock.sendMessage(message)){}
    while(true)
    {
        if(replyMessages.find(rpcId)!=replyMessages.end())
        {
            if(replyMessages[rpcId]->getMessageType()==MessageType::Reply)
            {
                string s(replyMessages[rpcId]->getMessage());
                return s;
            }
        }
    }
}
inline void split(string str, vector<string>& cont, char delim = ' ')
{
    stringstream ss(str);
    string token;
    while (getline(ss, token, delim)) {
        cont.push_back(token);
    }
}
string Peer::getImage(string myusername,string ownerusername,string targetadd,unsigned int targetport,string imagename)
{
    rpcidmtx.lock();
    int rpcId = rpccount++;
    rpcidmtx.unlock();
    Message *message = new Message();
    message->setSourceIP(sock.getMyIP());
    message->setSourcePort(sock.getMyPort());
    message->setRPCID(rpcId);
    message->setDestinationIP(targetadd);
    message->setDestinationPort(targetport);
    message->setOperation(Operation::getImage);
    string request = myusername+DELIM+ownerusername+DELIM+imagename;
    int n = request.length(); 
    char *char_array=new char[n+1]; 
    strcpy(char_array, request.c_str()); 
    message->setMessage(char_array,n);
    message->setMessageType(MessageType::Request);
    while(!sock.sendMessage(message)){}
    while(true)
    {
        if(replyMessages.find(rpcId)!=replyMessages.end())
        {
            if(replyMessages[rpcId]->getMessageType()==MessageType::Reply)
            {
                string s(replyMessages[rpcId]->getMessage());
                return base64_decode(s);
            }
        }
    }
}
//working as a server
void Peer::serve()
{
    Message *message;
    while(!dest)
    {
        if(!serveMessages.empty())
        {
            cout<<serveMessages.size()<<endl;
            for(int i =0;i<serveMessages.size();i++)
            { 
                if(serveMessages[i]->getOperation()==Operation::getImage){
                    cout<<"Full Image"<<endl;
                    vector<string> fields;
                    split(serveMessages[i]->getMessage(),fields,',');
                    if(fields.size()<2)
                        continue;
                    Image img;
                    bool found = false;
                    for(int i =0;i<myImages.size();i++)
                    {
                        if(myImages[i]==fields[2])
                        {
                            found=true;
                            break;
                        }
                    }
                    if(!found||!(img.findImage(fields[1],fields[2])))
                        continue;
                    string request =img.extractImage();
                    request = base64_encode(reinterpret_cast<const unsigned char*>(reinterpret_cast<const unsigned char*> (request.c_str())), request.size() + 1);
                    img.removeMiddleFiles();
                    message = new Message();
                    message->setSourceIP(sock.getMyIP());
                    message->setSourcePort(sock.getMyPort());
                    message->setRPCID(serveMessages[i]->getRPCId());
                    message->setDestinationIP(serveMessages[i]->getSourceIP());
                    message->setDestinationPort(serveMessages[i]->getSourcePort());
                    message->setOperation(Operation::getImage);
                    message->setMessageType(MessageType::Reply);
                    int n = request.length(); 
                    char *char_array=new char[n+1]; 
                    strcpy(char_array, request.c_str()); 
                    message->setMessage(char_array,n);
                    while(!sock.sendMessage(message)){}
                    

                }else if(serveMessages[i]->getOperation()==Operation::getThumbnails)
                {
                    cout<<"Thumbnails"<<endl;
                    vector<string> fields;
                    split(serveMessages[i]->getMessage(),fields,',');
                    if(fields.size()<2)
                        continue;
                    Image img;
                    string images="";
                    bool first=true;
                    for(int i =0;i<myImages.size();i++)
                    {
                        Image img;
                        if(img.findImage(fields[1],myImages[i])){
                            img.desteg();
                            if(!first)
                                images+=DELIM;
                            else
                                first=false;
                            images += img.getSmallScaleImage();
                        }
                        img.removeMiddleFiles();
                    }
                    message = new Message();
                    if(images=="")
                        images="no photos";
                    message->setSourceIP(sock.getMyIP());
                    message->setSourcePort(sock.getMyPort());
                    message->setRPCID(serveMessages[i]->getRPCId());
                    message->setDestinationIP(serveMessages[i]->getSourceIP());
                    message->setDestinationPort(serveMessages[i]->getSourcePort());
                    message->setOperation(Operation::getThumbnails);
                    message->setMessageType(MessageType::Reply);
                    int n = images.length(); 
                    char *char_array=new char[n+1]; 
                    strcpy(char_array, images.c_str()); 
                    message->setMessage(char_array,n);
                    while(!sock.sendMessage(message)){}
                    
                }
            }
            vectorMtx.lock();
            serveMessages.clear();
            vectorMtx.unlock();
        }
        usleep(100);
    }
}
string Peer::getImageUpdates()
{
    string reply;
    while(!dest)
    {
        vector<Message*>::iterator it;
        for(it = serveMessages.begin();it!=serveMessages.end();it++)
        {
            if((*it)->getMessageType()==MessageType::Request &&
               (*it)->getOperation()==Operation::requestImageAccess)
            {
                vectorMtx.lock();
                reply = string((*it)->getMessage());
                serveMessages.erase(it);
                vectorMtx.unlock();
                return reply;
            }
        }
    }
    return "Finished Thread";
}
//listening to socket
void Peer::listen()
{
    while(!dest){
        if(sock.ReceiveBuffer.empty())
            continue;
        Message *reply=sock.receiveMsg();
        if(dest)
            break;
        if(reply->getMessageType()==MessageType::Reply){
            if(replyMessages.find(reply->getRPCId())==replyMessages.end())
            {
                replyMessages.insert({reply->getRPCId(),reply});
            }
        }else if(reply->getMessageType()==MessageType::Request)
        {
            vectorMtx.lock();
            serveMessages.push_back(reply);
            vectorMtx.unlock();
        }
        
    }
}
void Peer::status(string input)
{
    string token = input;
    while (!dest)
    {
        rpcidmtx.lock();
        int rpcId = rpccount++;
        rpcidmtx.unlock();
        Message *message = new Message();
        message->setSourceIP(sock.getMyIP());
        message->setSourcePort(sock.getMyPort());
        message->setRPCID(rpcId);
        message->setDestinationIP(dsaddr);
        message->setDestinationPort(dsport);
        message->setOperation(Operation::updateStatus);
        char *char_array=new char[token.length()+1]; 
        strcpy(char_array, token.c_str()); 
        message->setMessage(char_array,token.length());
        message->setMessageType(MessageType::Request);
        while(!sock.sendMessage(message)){}
        sleep(STATUS_UPDATE_TIME);
    }
    
}
Peer :: ~Peer()
{
    dest=true;
    serve_thread->join();
    read_thread->join();
}
