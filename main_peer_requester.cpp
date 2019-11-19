#include "Peer.h"
using namespace std;
int main()
{
    Peer peer("127.0.0.1",2);
    peer.setDS("127.0.0.1",88);
    string start;
    cin>>start;
    cout<<peer.login("osama","123")<<endl;
    cout<<peer.signup("osama","123")<<endl;
    // cout<<"Requesting Image\n";
    // string s = peer.getImage("osama","osos","127.0.0.1",1,"image123");
    // ofstream out;
    // out.open("trial.jpg",ios_base::out | ios_base::binary);
    // out<<s;
    // out.close();
}