
#include <sys/time.h>
#include <iostream>
using namespace std;

int main()
{

struct timeval tp;
gettimeofday(&tp, NULL);
unsigned long long ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
cout << ms << endl;
}