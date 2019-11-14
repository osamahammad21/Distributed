#include "Message.h"
#include <string>
Message::Message(){}
Message :: Message(MessageType _message_type,  unsigned int _fragmentCount, unsigned int  _fragmentTotal, string _sourceIP, string _destIP, unsigned int _port, unsigned int _rpc_id, unsigned int _operation, long long _message_size,  char * _message)
{    
    this->message_type = _message_type;
    this->fragmentCount = _fragmentCount;
    this->fragmentTotal = _fragmentTotal;
    this->sourceIP = _sourceIP;
    this->destIP = _destIP;
    this->port = _port;
    this->rpc_id = _rpc_id;
    this->operation = _operation; //Which function to call on server side
    this->message_size = _message_size;
    this->message = _message;
}
Message::Message(MessageType _message_type, unsigned int _rpc_id, unsigned int _operation ,char *_message)
{
    this->message_type = _message_type;
    this->rpc_id = _rpc_id;
    this->operation = _operation;
    this->message = _message;
}

Message :: Message(char * marshalled_base64)
{
    string serialized_msg(marshalled_base64);
    string decoded_serialized_msg = base64_decode(serialized_msg);
    unsigned int source_IP_size, dest_IP_size;

    //Unmarshal
    this->message_type = (MessageType)(decoded_serialized_msg[0] - 48);
    hex_to_T(decoded_serialized_msg.substr(1, 8), this->fragmentCount);
    hex_to_T(decoded_serialized_msg.substr(9, 8), this->fragmentTotal);
    hex_to_T(decoded_serialized_msg.substr(17, 8), source_IP_size); 
    this->sourceIP = decoded_serialized_msg.substr(25,source_IP_size);
    int shift = 25 + source_IP_size-1;
    hex_to_T(decoded_serialized_msg.substr(shift + 1, 8), dest_IP_size); 
    this->destIP = decoded_serialized_msg.substr(shift+ 9, dest_IP_size);
    shift = shift + 9 + dest_IP_size-1;
    hex_to_T(decoded_serialized_msg.substr(shift + 1, 8), this->port); 
    hex_to_T(decoded_serialized_msg.substr(shift + 9, 8), this->rpc_id); 
    hex_to_T(decoded_serialized_msg.substr(shift + 17,8), this->operation); 
    hex_to_T(decoded_serialized_msg.substr(shift + 25, 16), this->message_size); 
    
    this->message = new char[this->message_size+1];
    std::strcpy((char *)this->message , decoded_serialized_msg.substr(shift+25+16).c_str());
}

char * Message ::marshal()
{
//1 digit ->    4 bits -> 0.5 byte -> 1 byte as character in string
//             msg_type     frag_count_s    frag_total_s    source_IP_size_s    sourceIP  dest_IP_size_s    destIP  port_s  rpc_id_s    op_s    msg_size_s
//hex_digits=   1               8               8                  8                x            8            y      8        8        8        16(64bits)
    char msg_type           = char ((int)this->message_type + 48);      //0 request, 1 reply, 2 Ack     
    string frag_count_s     =  int_to_hex((int)this->fragmentCount);       
    string frag_total_s     =  int_to_hex((int)this->fragmentTotal);        
    string source_IP_size_s = int_to_hex((int)this->sourceIP.size());
    string dest_IP_size_s =  int_to_hex((int)this->destIP.size());
    string port_s       = int_to_hex(this->port);
    string rpc_id_s     =  int_to_hex((int)this->rpc_id);  
    string op_s         =  int_to_hex((int)this->operation);                  
    string msg_size_s   = int_to_hex((long long) this->message_size); 
    string message((char * ) this->message); 
    string packet               = msg_type + frag_count_s + frag_total_s + source_IP_size_s + this->sourceIP + dest_IP_size_s  + this->destIP + port_s + rpc_id_s + op_s + msg_size_s + message;
    string packet_encoded       = base64_encode(reinterpret_cast<const unsigned char*>(reinterpret_cast<const unsigned char*> (packet.c_str())), packet.size() + 1);
    char * encoded_string_ptr   = new char[packet_encoded.size()+1];
    strcpy(encoded_string_ptr, packet_encoded.c_str());

    return encoded_string_ptr;
}
MessageType Message :: getMessageType()
{
    return this->message_type;
}
void Message :: setMessageType(MessageType type)
{
    this->message_type = type;
}
unsigned int Message :: getFragmentCount()
{
    return this->fragmentCount;
}
unsigned int Message :: getFragmentTotal()
{
    return this->fragmentTotal;
}
void setFragState(unsigned int curr_frag, unsigned int totalFrags)
{
    this->fragmentCount = curr_frag;
    this->fragmentTotal = totalFrags;
}

string Message :: getSourceIP()
{
    return this->sourceIP;
}
void Message :: setSourceIP(string ip)
{
    this->sourceIP = ip;
}
string Message :: getDestinationIP()
{
    return this->destIP;
}
void Message :: setDestinationIP(string ip)
{
    this->destIP = ip;
}
unsigned int Message :: getPort()
{
    return this->port;
}
void Message :: setPort(unsigned int port)
{
    this->port = port;
}
unsigned int Message :: getRPCId()
{
    return this->rpc_id;
}
void Message :: setRPCID(unsigned int RPCID)
{
    this->rpc_id = RPCID;
}
unsigned int Message :: getOperation ()
{
    return this->operation;
}
void Message :: setOperation (unsigned int op)
{
    this->operation = op;
}
unsigned int Message :: getMessageSize()
{
    return this->message_size;
}
char * Message :: getMessage()
{
    return this->message;
}
void Message :: setMessage(char * p)
{
    this->message = p;
}

void Message :: setMessage (char * message, unsigned int message_size)
{
    this->message = message;
    this->message_size = message_size;
}

Message::~Message()
{
}
