#include "Message.h"
#include <string>

template< typename T >
std::string int_to_hex( T i ){
    
    std::stringstream stream;
    stream<< std::setfill ('0') << std::setw(sizeof(T)*2)<< std::hex << i;
    return stream.str();
    
}

template <typename T>
void hex_to_T(std::string sz, T & pnt){
    sz = "0x" + sz;
    std::istringstream iss(sz);
    iss >> std::hex >> pnt;
}

Message :: Message(int operation, MessageType message_type,  char * message, unsigned int message_size, int rpc_id, unsigned int fragmentTotal, unsigned int fragmentCount, int port, string destIP, string sourceIP)
{
    
    this->message_type = message_type;
    this->operation = operation; //Which function to call on server side
    this->message = message;         
    this->message_size = message_size;    
    this->rpc_id = rpc_id;
    this->fragmentTotal = fragmentTotal;
    this->fragmentCount = fragmentCount;
    this->port = port;
    this->destIP = destIP;
    this->sourceIP = sourceIP;
}
Message :: Message(char * marshalled_base64)
{
    string serialized_msg(marshalled_base64);
    string decoded_serialized_msg = base64_decode(serialized_msg);
    unsigned int source_IP_size, dest_IP_size;
    //Unmarshel
    hex_to_T(decoded_serialized_msg.substr(0,0), this->message_type);
    hex_to_T(decoded_serialized_msg.substr(1, 8), this->fragmentCount);
    hex_to_T(decoded_serialized_msg.substr(9, 16), this->fragmentTotal); 
    hex_to_T(decoded_serialized_msg.substr(17, 24), source_IP_size); 
    hex_to_T(decoded_serialized_msg.substr(25,25 +source_IP_size-1), this->sourceIP); 
    int shift = 25 +source_IP_size-1;
    hex_to_T(decoded_serialized_msg.substr(shift + 1, shift+8), dest_IP_size); 
    hex_to_T(decoded_serialized_msg.substr(shift+8 + 1 , shift + 8 + 1  + dest_IP_size-1), this->destIP); 
    shift = (shift+8) + 1 + dest_IP_size-1;
    hex_to_T(decoded_serialized_msg.substr(shift + 1, shift +8), this->port); 
    hex_to_T(decoded_serialized_msg.substr(shift + 9, shift + 16), this->rpc_id); 
    hex_to_T(decoded_serialized_msg.substr(shift + 17,shift + 24), this->operation); 
    hex_to_T(decoded_serialized_msg.substr(shift + 25, shift + 32), this->message_size); 
    hex_to_T(decoded_serialized_msg.substr(shift + 33, shift + 33 + this->message_size-1), this->message); 

    cout << "Message type " << message_type << endl;
    cout << "fragmentCount " << fragmentCount << endl;
    cout << "sourceIP " << sourceIP << endl;
    cout << "destIP " << destIP << endl;
    cout << "port " << port << endl;
    cout << "rpc_id " << rpc_id << endl;
    cout << "operation " << operation << endl;
    cout << "message_size " << message_size << endl;
    printf("%s %s\n","Message ", message);
}

char * Message :: marshal ()
{
//1 digit ->    4 bits -> 0.5 byte 
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
unsigned int Message :: getOperation ()
{
    return this->operation;
}
unsigned int Message :: getRPCId()
{
    return this->rpc_id;
}

char * Message :: getMessage()
{
    return this->message;
}
unsigned int Message :: getMessageSize()
{
    return this->message_size;
}
MessageType Message :: getMessageType()
{
    return this->message_type;
}
void Message :: setOperation (unsigned int _operation)
{
    this->operation = _operation;
}
void Message :: setMessage (char * message, unsigned int message_size)
{
this->message = message;
this->message_size = message_size;
}
void Message :: setMessageType (MessageType message_type)
{
    this->message_type = message_type;
}
Message::~Message(){
    
}