#include "Message.h"
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
    string input = "";

    
    Message *m =new Message(Request, 1, 3, "10.1", "10.2", 6666, 9, 8, 10, "MEEEEEEEEH");
    // bool stop = false;

    // while(!stop)
    // {
    //     cout << "Client main: before send" << endl;
    //     cin >> input;     
    //     char * str = new char[input.size()];  
    //     strcpy(str, input.c_str());
    //     Message  * m;
    //     int status = c.Send(m);
    //     cout << "Client main: after send" << endl;
    // }

    return 0;
}