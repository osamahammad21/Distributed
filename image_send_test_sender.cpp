#include "UDPSocket.h"

#include <iostream>
#include <string>
#include<fstream>
using namespace std;

int main(int argc, char ** argv)
{
    char * myIP= argv[1]; 
    char * destIP = argv[2];
    unsigned int myPort = stoi(argv[3]);
    unsigned int destPort = stoi(argv[4]);

    bool stop = false;
    string input;

    UDPSocket sockobj;
    struct sockaddr_in peerAddr;

    bool meh = sockobj.initializeSocket(myPort);
    cout << "MY IP" << sockobj.getMachineIP();
    cout << "finished Contrustion" << endl;
int i;
    //cout << extract << endl;
    int j = 0;
    while(true)
    {
    cin >> input;

    ifstream fin("trial.jpg",ios::binary);
    string extract = string(istreambuf_iterator<char>(fin), istreambuf_iterator<char>());
    extract = base64_encode(reinterpret_cast<const unsigned char*>(reinterpret_cast<const unsigned char*> (extract.c_str())), extract.size() + 1);
    char * str = new char[extract.size()];  
    strcpy(str, extract.c_str());
    //cout << "extract size" << extract.size() << endl;
       // seconds ms = duration_cast< seconds >(system_clock::now().time_since_epoch());  

    //ong long timeStamp =  ms.count();

    Message *m =new Message(Request, 1, 3, sockobj.getMyIP(), sockobj.getMyPort(), destIP, destPort, ++i, 8, extract.size(), str);
    sockobj.sendMessage(m);
    }

    // while(!stop && input != "q")
    // {
    //     cin >> input;
    //     char * str = new char[input.size()];  
    //     strcpy(str, input.c_str());
    //     Message *m =new Message(Request, 1, 3, "10.1", "10.2", 6666, 9, 8, input.size(), str);
    //     int stat = pass(m);
    //     //c.setTimeout(0, 10000);
    //     c.Send(m);
    // }

    // Client c(ip,port);
    // c.setTimeout(0, 10000);
    // string input = "";

//while(!stop)
    // {
    //     cout << "Client main: before send" << endl;
    //     cin >> input;     
    //     char * str = new char[input.size()];  
    //     strcpy(str, input.c_str());
    //     Message  * m;
    //     int status = c.Send(m);
    //     cout << "Client main: after send" << endl;
    // }
    // Message *m =new Message(Request, 1, 3, "10.1", "10.2", 6666, 9, 8, 10, "MEEEEEEEEH");
    // char * s = new char [1024]; 
    // s = m->marshal();
    // Message newM(s);
    // cout << newM.getMessageType()<< endl;
    // cout << newM.getFragmentCount()<< endl;
    // cout << newM.getFragmentTotal()<< endl;
    // cout << newM.getSourceIP()<< endl;
    // cout << newM.getDestinationIP()<< endl;
    // cout << newM.getPort()<< endl;
    // cout << newM.getRPCId()<< endl;
    // cout << newM.getOperation()<< endl;
    // cout << newM.getMessageSize()<< endl;
    // cout << newM.getMessage()<< endl;
    //  bool stop = false;
    // Message *m =new Message(Request, 1, 3, "10.1", "10.2", 6666, 9, 8, 10, "MEEEEEEEEH");
    // // pass(m);
    // cout<<"I am out"<<endl;
    // cout << "Client main: before send" << endl;
    // cin >> input;     
    // // char * str = new char[input.size()];  
    // // strcpy(str, input.c_str());
    // cout<<"I am here";
    // int status = c.Send(m);
    // cout << "Client main: after send" << endl;

    return 0;
}