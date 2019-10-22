#include "Server.h"
using namespace std;
#define SIZE 1024

int main(int argc,char ** argv){
    if(argc>1)
    {
        Server s((char *)"unused", stoi(argv[1]));
    }
    return 0;
}