#include "Message.h"

Message::Message(){}
Message ::Message(MessageType _message_type,  unsigned int _fragmentCount, unsigned int  _fragmentTotal, string _sourceIP, unsigned int _sourcePort, string _destIP, unsigned int _destPort, unsigned int _rpc_id, unsigned int _operation, long long _message_size,  char * _message)
{
    this->message_type = _message_type;
    this->fragmentCount = _fragmentCount;
    this->fragmentTotal = _fragmentTotal;
    this->sourceIP = _sourceIP;
    this->sourcePort = _sourcePort;
    this->destIP = _destIP;
    this->destPort = _destPort;
    this->rpc_id = _rpc_id;
    this->operation = _operation; //Which function to call on server side
    this->message_size = _message_size;
    this->message = _message;

}

Message :: Message(char * marshalled_base64)
{
    cout << "demarshalling" << endl;
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
    hex_to_T(decoded_serialized_msg.substr(shift + 1, 8), this->sourcePort);
    hex_to_T(decoded_serialized_msg.substr(shift + 9, 8), dest_IP_size);
    this->destIP = decoded_serialized_msg.substr(shift+ 17, dest_IP_size);
    shift = shift + 17 + dest_IP_size-1;
    hex_to_T(decoded_serialized_msg.substr(shift + 1, 8), this->destPort);
    hex_to_T(decoded_serialized_msg.substr(shift + 9, 8), this->rpc_id);
    hex_to_T(decoded_serialized_msg.substr(shift + 17,8), this->operation);
    hex_to_T(decoded_serialized_msg.substr(shift + 25,16), this->timeStamp);
    hex_to_T(decoded_serialized_msg.substr(shift + 41, 16), this->message_size);

    this->message = new char[this->message_size+1];
    std::strcpy((char *)this->message , decoded_serialized_msg.substr(shift+41+16).c_str());
}
vector<string> Message :: getMessageArgs()
{
    string str(this->message);
    vector<string>result;
    string token = ",";
    while (str.size()) {
        int index = str.find(token);
        if (index != string::npos) {
            result.push_back(str.substr(0, index));
            str = str.substr(index + token.size());
            if (str.size() == 0)result.push_back(str);
        }
        else {
            result.push_back(str);
            str = "";
        }
    }
    return result;
}
char * Message :: marshal ()
{
//1 digit ->    4 bits -> 0.5 byte
//             msg_type     frag_count_s    frag_total_s    source_IP_size_s    sourceIP  sourcePort_s   dest_IP_size_s    destIP  destPort_s  rpc_id_s    op_s     timestamp        msg_size_s
//hex_digits=   1               8               8                  8                x            8             8               y      8            8        8           16            16(64bits)
    char msg_type           = char ((int)this->message_type + 48);      //0 request, 1 reply, 2 Ack
    string frag_count_s     =  int_to_hex((int)this->fragmentCount);
    string frag_total_s     =  int_to_hex((int)this->fragmentTotal);
    string source_IP_size_s = int_to_hex((int)this->sourceIP.size());
    string sourcePort_s       = int_to_hex((int)this->sourcePort);
    string dest_IP_size_s =  int_to_hex((int)this->destIP.size());
    string destPort_s       = int_to_hex((int)this->destPort);
    string rpc_id_s     =  int_to_hex((int)this->rpc_id);
    string op_s         =  int_to_hex((int)this->operation);
    string time_stamp_s =  int_to_hex((long long)this->timeStamp);
    string msg_size_s   = int_to_hex((long long) this->message_size);
    string message((char * ) this->message);

    string packet               = msg_type + frag_count_s + frag_total_s + source_IP_size_s + this->sourceIP + sourcePort_s + dest_IP_size_s  + this->destIP + destPort_s + rpc_id_s + op_s + time_stamp_s +  msg_size_s + message;
    string packet_encoded       = base64_encode(reinterpret_cast<const unsigned char*>(reinterpret_cast<const unsigned char*> (packet.c_str())), packet.size() + 1);
    //cout << "Cout packet encoded " <<  packet_encoded << endl;
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
void Message::setFragState(unsigned int curr_frag, unsigned int totalFrags)
{
    this->fragmentCount = curr_frag;
    this->fragmentTotal = totalFrags;
}

string Message :: getSourceIP()
{
    return this->sourceIP;
}
long long Message::getMessageTimestamp()
{
    return this->timeStamp;
}

void Message::setMessageTimestamp(long long _timeStamp)
{
    this->timeStamp = _timeStamp;
}

void Message :: setSourceIP(string ip)
{
    this->sourceIP = ip;
}
unsigned int Message :: getSourcePort(){
    return this->sourcePort;
}
void Message :: setSourcePort(unsigned int sourcePort){
    this->sourcePort = sourcePort;
}
string Message :: getDestinationIP()
{
    return this->destIP;
}
void Message :: setDestinationIP(string ip)
{
    this->destIP = ip;
}
unsigned int Message :: getDestinationPort(){
    return this->destPort;
}
void Message :: setDestinationPort(unsigned int destPort){
    this->destPort = destPort;
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
void Message :: setMessageSize(unsigned int _size)
{
    this->message_size = _size;
}

void Message :: setMessage (char * message, unsigned int message_size)
{
    this->message = message;
    this->message_size = message_size;
}

Message::~Message()
{
}
