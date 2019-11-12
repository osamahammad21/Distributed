#ifndef MESSAGE_H
#define MESSAGE_H
#include <string>
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

Message(int operation, void * p_message, size_t p_message_size,int p_rpc_id);
Message(char * marshalled_base64);
char * marshal ();
int getOperation ();
ing getRPCId();
char * getMessage();
unsigned int getMessageSize();
MessageType getMessageType();
void setOperation (int _operation);
void setMessage (void * message, size_t message_size);
void setMessageType (MessageType message_type);
~Message();

};
#include "Message.cpp"
#endif // MESSAGE_H

/*