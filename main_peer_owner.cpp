#include "Peer.h"
using namespace std;
int main()
{
    Peer peer("127.0.0.1",1);
    peer.setUserName("osos");
    peer.setDS("127.0.0.1",50);
    while(true);
    // cout<<"Waiting for upload"<<endl;
    // cout<<peer.uploadImage("token","osos")<<endl;
    // cout<<"Upload Done"<<endl;
}