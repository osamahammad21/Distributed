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
    cout << "finished Contrustion " << endl;
    time_t meeh = 90;
    int i=0;
   
    int j = 0;
    string extract;
    while(true)
    {
        Message * newM = sockobj.receiveMsg();
        cout << "Received Msg of size " << newM->getMessageSize() << endl;
        cout << "time stamp = " << newM->getMessageTimestamp();
        
        if(newM != NULL)
        {
        int size = newM->getMessageSize();
        char *c = new char[size];
        c = newM->getMessage();
        int i=0;
        string img;
        while(size--)
        img+=c[i++];
        img = base64_decode(img);

        ofstream out;
        string path = "out_img" + to_string(j) + ".jpg";
        out.open(path, ios_base::out | ios_base::binary);
        out << img;
        out.close();
        
        }

    }

    return 0;
}