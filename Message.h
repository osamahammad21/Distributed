#ifndef MESSAGE_H
#define MESSAGE_H
#include <string>
#include "base64.h"
#include <stdio.h>
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

template< typename T >
std::string int_to_hex( T i ){
    
    stringstream stream;
    stream << (setfill ('0')) << (setw(sizeof(T)*2)) <<  (hex) << (i);
    return stream.str();
    
}

template <typename T>
void hex_to_T(string sz, T & pnt){
    sz = "0x" + sz;
    istringstream iss(sz);
    iss >> (hex) >> (pnt);
}
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

Message(MessageType _message_type,  unsigned int fragmentCount, unsigned int  fragmentTotal, string _sourceIP, string _destIP, unsigned int _port, unsigned int _rpc_id, unsigned int _operation, long long _message_size,  char * _message);
Message(char * marshalled_base64);
char * marshal ();

MessageType getMessageType();
void setMessageType(MessageType type);
unsigned int getFragmentCount();
unsigned int getFragmentTotal();
string getSourceIP();
void setSourceIP(string ip);
string getDestinationIP();
void setDestinationIP(string ip);
unsigned int getPort();
void setPort(unsigned int port);
unsigned int getRPCId();
void setRPCID(unsigned int RPCID);
unsigned int getOperation ();
void setOperation (unsigned int op);
unsigned int getMessageSize();
char * getMessage();
void setMessage(char * p);
void setMessage (char * message, unsigned int message_size);

~Message();

};
#endif // MESSAGE_H
