#ifndef MESSAGE_H
#define MESSAGE_H
#include <string>
#include "base64.h"
#include <stdio.h>
#include "base64.h"
#include <iomanip>
#include <sstream>
#include <iostream>
#include <string>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <cstring> 
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> 
#include <sys/types.h>
#include <netdb.h>
using namespace std;
enum MessageType { Request, Reply, Ack};

class Message
{
private:

MessageType message_type;
unsigned int fragmentCount; 
unsigned int fragmentTotal;
string sourceIP; 
string destIP; 
int port;
unsigned int rpc_id;
unsigned int operation; //Which function to call on server side
unsigned int message_size;    
char * message;         

public:

Message(int operation, MessageType message_type,  char * message, unsigned int message_size, int rpc_id, unsigned int fragmentTotal, unsigned int fragmentCount, int port, string destIP, string sourceIP);
Message(char * marshalled_base64);
char * marshal ();
unsigned int getOperation ();
unsigned int getRPCId();
char * getMessage();
unsigned int getMessageSize();
MessageType getMessageType();
void setOperation (unsigned int _operation);
void setMessage (char * message, unsigned int message_size);
void setMessageType (MessageType message_type);
~Message();

};
#include "Message.cpp"
#endif // MESSAGE_H
