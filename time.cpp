
#include <sys/time.h>
#include <iostream>
using namespace std;

int main()
{

struct timeval tp;
gettimeofday(&tp, NULL);
long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
}