#include "Message.h"

using namespace std;

int main()
{

    Message m (10, Request, "HELLOOOOOOOOO", 14, 89, 120, 334, 8767, "10.9.8.1", "10.7.6.5");
    char * s = new char [1024]; 
    s = m.marshal();
    Message newM(s);

    return 0;
}