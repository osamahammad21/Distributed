#include "Message.h"

using namespace std;

int main()
{
    Message m (0, Request, "hellooo", 7, 2, 1, 1, 55, "10.9.8.10", "10.7.6.5");
    char * s = new char [1024]; 
    s = m.marshal();

    Message newM(s);

    return 0;
}