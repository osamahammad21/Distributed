User :: User(string username, string password){
    myInfo.username = username; 
    myInfo.password password; 
    FormMessage_signup();
    rpcId = 0;
}

void updateStatus(){
    string messageS = "0"; 
    char * messageC = new char[messageS.size()];
    strcpy(messageC, messageS.c_str());
    Message * replyMessage;
    rpcId++; 
    Message * message(MessageType:: Request, 0, 0, udpClientSocket.myAddress, DIRECTORY_IP, DIRECTORY_PORT , rpcId, Operation::login, messageS.size(), messageC);
    sendMessage(message, replyMessage ,False);
    //will I do anything with replyMessage?
}

//acting as a client
void Peer:: login (){
    string messageS = myInfo.username + "," + myInfo.password; 
    
    char * messageC = new char[messageS.size()];
    strcpy(messageC, messageS.c_str());
    Message * replyMessage;
    rpcId++; 
    Message * message(MessageType:: Request, 0, 0, udpClientSocket.myAddress, DIRECTORY_IP, DIRECTORY_PORT , rpcId, Operation::login, messageS.size(), messageC);
    sendMessage(message, replyMessage ,False);
    //will I do anything with replyMessage?
}

void Peer:: signup(){
    string messageS = myInfo.username + "," + myInfo.password; 
    
    char * messageC = new char[messageS.size()];
    strcpy(messageC, messageS.c_str());
    Message * replyMessage;
    rpcId++; 
    Message * message(MessageType:: Request, 0, 0, udpClientSocket.myAddress, DIRECTORY_IP, DIRECTORY_PORT , rpcId, Operation::signup, messageS.size(), messageC);
    sendMessage(message, replyMessage ,False);
    //will I do anything with replyMessage?
}
void Peer:: logout(){
    string messageS = myInfo.username + "," + myInfo.password; 
    
    char * messageC = new char[messageS.size()];
    strcpy(messageC, messageS.c_str());
    Message * replyMessage;
    rpcId++; 
    Message * message(MessageType:: Request, 0, 0, udpClientSocket.myAddress, DIRECTORY_IP, DIRECTORY_PORT , rpcId, Operation::logout, messageS.size(), messageC);
    sendMessage(message, replyMessage ,False);
    //will I do anything with replyMessage?
}
void Peer:: Client_uploadImage(string imageName){
    String messageS = myInfo.username +","+ imageName;
    char * messageC = new char[messageS.size()];
    strcpy(messageC, messageS.c_str());
    Message * replyMessage;
    rpcId++; 
    Message * message(MessageType:: Request, 0, 0, udpClientSocket.myAddress, DIRECTORY_IP, DIRECTORY_PORT, rpcId, Operation::uploadImage, messageS.size(), messageC);
    sendMessage(message, replyMessage ,False);
    //will I do anything with replyMessage?
}

void Peer:: Client_viewImage(string ownerUsername, string imageName){
    String messageS = ownerUsername;
    char * messageC = new char[messageS.size()];
    strcpy(messageC, messageS.c_str());
    Message * replyMessage;
    rpcId++; 
    Message * message(MessageType:: Request, 0, 0, udpClientSocket.myAddress, DIRECTORY_IP, DIRECTORY_PORT, rpcId, Operation::getPortnIP, messageS.size(), messageC);
    sendMessage(message, replyMessage, False);

    vector<string> args;
    args = replyMessage.getMessageArgs();

    messageS = myInfo.username + "," + imageName;
    messageC = new char[messageS.size()];
    strcpy(messageC, messageS.c_str());
    rpcId++; 
    Message * message(MessageType:: Request, 0, 0, udpClientSocket.myAddress, args[0], stoi(args[1]), rpcId, Operation::viewImage, messageS.size(), messageC);
    sendMessage(message, replyMessage, False);
    //TODO
    //View image in reply message
}
void Peer:: Client_requestImage(){
    String messageS = ownerUsername;
    char * messageC = new char[messageS.size()];
    strcpy(messageC, messageS.c_str());
    Message * replyMessage;
    rpcId++; 
    Message * message(MessageType:: Request, 0, 0, udpClientSocket.myAddress, DIRECTORY_IP, DIRECTORY_PORT, rpcId, Operation::getPortnIP, messageS.size(), messageC);
    sendMessage(message, replyMessage, False);

    vector<string> args;
    args = replyMessage.getMessageArgs();
    
    messageS = imageName;
    messageC = new char[messageS.size()];
    strcpy(messageC, messageS.c_str());
    rpcId++; 
    Message * message(MessageType:: Request, 0, 0, udpClientSocket.myAddress, args[0], stoi(args[1]), rpcId, Operation::requestImage, messageS.size(), messageC);
    sendMessage(message, replyMessage, False);
    //will I do anything with replyMessage?
}

void getAllImages(){
    String messageS = "-1";
    char * messageC = new char[messageS.size()];
    strcpy(messageC, messageS.c_str());
    Message * replyMessage;
    rpcId++; 
    Message * message(MessageType:: Request, 0, 0, udpClientSocket.myAddress, DIRECTORY_IP, DIRECTORY_PORT, rpcId, Operation::getAllImages, messageS.size(), messageC);
    sendMessage(message, replyMessage ,False);

    vector<string> args;
    args = replyMessage.getMessageArgs();

    for(int i=0; i<args.size(); i++){
        allImages.ownerUsername = args[i++];
        allImages.imageId = args[i];
    }
}

//acting as a server
void Peer:: Server_viewImage(Message * request, UDPSocket socket){
    vector<string> args;
    args = request.getMessageArgs();
    int imgIdx;
    for (imgIdx=0; imgIdx<myImages.size(); imgIdx++){
        if(myImages[imgIdx].imageId == args[1]){
            break;
        }
    }
    vector <struct userProperties> properties; 
    int userIdx; 
    for (userIdx=0; userIdx<properties.size(); userIdx++){
        if(properties[userIdx].username == args[0]){
            break;
        }
    }

    if (properties[userIdx].views > 0)
        properties[userIdx].views--; 
    
    myImages[imgIdx].setProperties(properties);
    
    String messageS = myImages[imgIdx].extractImage();
    char * messageC = new char[messageS.size()];
    strcpy(messageC, messageS.c_str());
        
    Message * message(MessageType:: Reply, 0, 0, socket.myAddress, request.getSourceIP(), request.getSourcePort(), rpcId, request.getOperation(), messageS.size(), messageC);
    sendReply(message);
}

void Peer:: Server_requestImage(){
    vector<string> args;
        args = request.getMessageArgs();
        int imgIdx;
        for (imgIdx=0; imgIdx<myImages.size(); imgIdx++){
            if(myImages[imgIdx].imageId == args[1]){
                break;
            }
        }
    vector <struct userProperties> properties; 
    int userIdx; 
    for (userIdx=0; userIdx<properties.size(); userIdx++){
        if(properties[userIdx].username == args[0]){
            break;
        }
    }

    if (properties[userIdx].views > 0)
        properties[userIdx].views = 1;
    else{
        struct userProperties userProp;
        userProp.username = args[0];
        userProp.views = stoi(args[1]);
        properties.push_back(userProp);
    }
        
    myImages[imgIdx].setProperties(properties);

    String messageS = "1";
    char * messageC = new char[messageS.size()];
    strcpy(messageC, messageS.c_str());
        
    Message * message(MessageType:: Ack, 0, 0, socket.myAddress, request.getSourceIP(), request.getSourcePort(), rpcId, request.getOperation(), messageS.size(), messageC);
    sendReply(message);
}