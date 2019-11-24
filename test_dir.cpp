#include <iostream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <vector>
using namespace std;
int main()
{
    int err = system("mkdir osos");
    cout<<err<<endl;
    // string username = "test_dir";
    // DIR* dirp;
    // dirp = opendir(username.c_str());
    // if(dirp==NULL)
    //     cout<<-1;
    // else
    //     cout<<0;
    // const int dir_err2 = mkdir(username.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    // cout<<dir_err2<<endl;
    // return 0;
}