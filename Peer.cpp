#include "Peer.h"
//Peer constructor
Peer :: Peer(int port){
    //create client socket
    sock.initializeSocket(port);
    read_thread = new std::thread(&Peer::listen,this);
    serve_thread = new std::thread(&Peer::serve,this);
}
void Peer::setDS(string ip,int port)
{
    dsaddr=ip;
    dsport=port;
}
//Before Auth
string Peer::login(string username,string password)
{
    int rpcId = rpccount++;
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
string Peer::signup(string username,string password)
{
    int rpcId = rpccount++;
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
//After Auth: Use token
string Peer::uploadImage(string token,string imagename)
{
    int rpcId = rpccount++;
    Message *message = new Message();
    message->setSourceIP(sock.getMyIP());
    message->setSourcePort(sock.getMyPort());
    message->setRPCID(rpcId);
    message->setDestinationIP(dsaddr);
    message->setDestinationPort(dsport);
    message->setOperation(Operation::uploadImage);
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
string Peer::getAllImages(string token)
{
    int rpcId = rpccount++;
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
string Peer::getPortnIP(string token,string targetusername)
{
    int rpcId = rpccount++;
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
    int rpcId = rpccount++;
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
void Peer::setUserName(string username)
{
    myusername=username;
}
void Peer::serve()
{
    Message *message;
    while(!dest)
    {
        if(!serveMessages.empty())
        {
            for(int i =0;i<serveMessages.size();i++)
            {
                cout<<serveMessages.size()<<endl;
                vector<string> fields;
                split(serveMessages[i]->getMessage(),fields,',');
                if(fields.size()<3)
                    continue;
                Image img;
                if(!(img.findImage(fields[1],fields[2])))
                    continue;
                img.readProperties();
                for(int i = 0;i<img.properties.size();i++)
                    if(img.properties[i].user_name==fields[0]&&img.properties[i].views>0)
                        img.properties[i].views--;
                img.updateProperties(); 
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
            }
            serveMessages.clear();
        }
        usleep(100);
    }
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
            serveMessages.push_back(reply);
        }
        
    }
}
Peer :: ~Peer()
{
    dest=true;
    serve_thread->join();
    read_thread->join();
}
//peer getRequest
// Message * Peer :: getRequest(){
//     char message[MSG_SIZE];  
//     bool stop;
//     do{
//         udpServerSocket.readFromSocketWithBlock (message,  SIZE);
//         printf("Received message from client: %s\n",message);
//         Message receivedMessage(message);
//         inputMessageMtx.lock();
//         inputMessageQueue.push(receivedMessage);
//         inputMessageMtx.unlock();

//         std::thread* m_thread=new std::thread(&Peer::sendReply, this);
//     }while(true);
// } 

// //TODO
// void Peer :: doOperation()
// {
    
// }

// //Peer send reply
// // HABDDDD
// void Peer :: sendReply ()
// {
//     usleep(9000);
//     const int SIZE = 1024;
//     inputMessageMtx.lock();
//     Message* message; 
//     message = inputMessageQueue.front();
//     inputMessageQueue.pop();
//     inputMessageMtx.unlock();

//     switch(message.getOperation()){
//         case Operation:: uploadImage: 
//             viewImage();
//         break; 
//         case Operation:: requestImageAccess: 
//         break;
//     }
//     //do operations
    
// }

// //Peer sendMessage (instead of Client's execute)
// int Peer :: sendMessage(Message * FullMessage, Message * ReplyMessage, bool activateTimeout)
// {
//     bool success = udpClientSocket.sendMessage(FullMessage);
//     if (! success)
//         return -1; 

//     //TODO
//     //Get reply into message object (WAIT FOR ALAA'S PROTOTYPE)
//     if(activateTimeout)
//         n = udpSocket.readFromSocketWithTimeout(ReplyMessage, MSG_SIZE, clientReadTimeout);
//     else
//         n = udpSocket.readFromSocketWithBlock(ReplyMessage, MSG_SIZE);
//     if(n<0)
//         return -2;//error no reply
//     else
//     {
//         //TODO
//         //check rpcID ????????
//         // std::string reply = message,requestMsg=request;
//         // int pos = reply.find(requestMsg);
//         // if(pos<0)
//         // {
//         //     return -3;//error irrelevant reply
//         // }else
//         // {
//         //     printf("Received reply from server %s.\n",message);
//         // }
        
//     }
    
//     return 1;
// }

// //Peer setTimeout
// void Peer::setTimeout(int timeoutSec, int timeoutMicro)
// {
//     clientReadTimeout.tv_sec = timeoutSec;
//     clientReadTimeout.tv_usec = timeoutMicro;
// }



