#include "Client.h"
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char ** argv)
{
    char * ip= argv[1];
    
    int port = stoi(argv[2]);
    Client c(ip,port);
    c.setTimeout(0, 10000);
    string input;
    int status=1;
    bool stop = false;
    int lostRequestsorAcks = 0,  RequestsNum = 0;
    while(!stop)
    {
        cin>>input;
        stop = input == "q";
        status = c.execute(&input[0], true, ++RequestsNum);

        if(status == -2)
        {
            cout<<"Lost request or no acknowledgement."<<endl;
            lostRequestsorAcks++;
        }else if(status==-3)
        {
            cout<<"Received Irrelevant Reply"<<endl;
            lostRequestsorAcks++;
        }
    }
    cout <<"Total Requests = "<<RequestsNum<<" Total lost = "<<lostRequestsorAcks<<endl;
    cout << "Total loss percentage = " << ((float)lostRequestsorAcks/(float)RequestsNum)*100 << "%" << endl;
    //c.execute((char *)"hellooo\0");
    //c.execute((char *)"q\0");
    // UDPSocket udp;
    // udp.initializeClient(ip, 59);
    //  ip= (char * )"10.7.57.169\0";
    // udp.writeToSocket(ip, (int)strlen(ip));
    return 0;
}