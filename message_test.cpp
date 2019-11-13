#include "Message.h"

using namespace std;

int main()
{
    Message m (Request, 1, 3, "10.1", "10.2", 6666, 9, 8, 10, "MEEEEEEEEH");
    char * s = new char [1024]; 
    s = m.marshal();
    Message newM(s);
    cout << newM.getMessageType()<< endl;
    cout << newM.getFragmentCount()<< endl;
    cout << newM.getFragmentTotal()<< endl;
    cout << newM.getSourceIP()<< endl;
    cout << newM.getDestinationIP()<< endl;
    cout << newM.getPort()<< endl;
    cout << newM.getRPCId()<< endl;
    cout << newM.getOperation()<< endl;
    cout << newM.getMessageSize()<< endl;
    cout << newM.getMessage()<< endl;

    return 0;
}