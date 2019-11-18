#include "Peer.h"
using namespace std;
int main()
{
    Peer peer("127.0.0.1",2);
    peer.setDS("127.0.0.1",50);
    string start;
    cin>>start;
    cout<<"Requesting Image\n";
    string s = peer.getImage("osama","osos","127.0.0.1",1,"image123");
    ofstream out;
    out.open("trial.jpg",ios_base::out | ios_base::binary);
    out<<s;
    out.close();
}