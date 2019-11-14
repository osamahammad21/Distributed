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
    cout << "Before msg1 \n" << endl;

    while(!stop)
    {
        cout << "2\n" << endl;

        string input;
        cin >> input;                
        cout << "3\n" << endl;
        Message m (Request, 1, 3, "10.1", "10.2", 6666, 9, 8, 10, &input[0]);
        stop = input == "q";
        cout << "4 \n" << endl;
        cout << "before execute\n" << endl;
        status = c.execute(&m);

        if(status == -2)
        {
            cout<<"Lost request or no acknowledgement."<<endl;
            lostRequestsorAcks++;
        }
        else if(status==-3)
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