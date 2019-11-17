#include "Message.h"
// #include "Client.h"
#include <iostream>
#include <string>
using namespace std;
// int passw(Message* m)
// {
//     cout<<m->getMessage()<<endl;
// }
// int pass(Message* m)
// {
//     passw(m);
    
//     cout<<"I am here1"<<endl;
// }
int main(int argc, char ** argv)
{
    // char * ip= argv[1];    
    // int port = stoi(argv[2]);
    //     Client c(ip,port);
    // bool stop = false;
    // string input;
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
    Message *m =new Message(Request, 1, 3, "10.1", 3, "10.2", 6666, 9, 8, 10, "MEEEEEEEEH");
    char * s = new char [1024]; 
    s = m->marshal();
    Message newM(s);
    cout << newM.getMessageType()<< endl;
    cout << newM.getFragmentCount()<< endl;
    cout << newM.getFragmentTotal()<< endl;
    cout << newM.getSourceIP()<< endl;    
    cout << newM.getSourcePort()<< endl;
    cout << newM.getDestinationIP()<< endl;
    cout << newM.getDestinationPort()<< endl;
    cout << newM.getRPCId()<< endl;
    cout << newM.getOperation()<< endl;
    cout << newM.getMessageSize()<< endl;
    cout << newM.getMessage()<< endl;
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