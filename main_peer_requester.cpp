#include "Peer.h"
#include <list>
using namespace std;
void DOS(int port)
{
    Peer peer(port);
    peer.setDS("127.0.0.1",88);
    string username = "osaos"+to_string(port);
    cout<<peer.signup(username,"1234")<<endl;
}
int main()
{
    
    string start;
    cin>>start;
    int i =55;
    Peer peer(i);
    peer.setDS("127.0.0.1",88);
    string username = "osaos"+to_string(i);
    cout<<peer.signup(username,"1234")<<endl;
    sleep(1);
    peer.startStatusUpdates("osaos"+to_string(i)+"1234");
    sleep(1);
    cout<<peer.uploadImage("osaos"+to_string(i)+"1234","IMAGENAME","BASE64")<<endl;
    sleep(1);
    cin>>start;
}
    //Peer peer(3);
    //cout<<peer.getImage("koko","osos","127.0.0.1",1,"864820");
    // cout<<<<endl;
    // cout<<peer.signup("osama","123")<<endl;
    // cout<<peer.uploadImage("osama","img1");
    // cout<<"Requesting Image\n";
    // string s = peer.getImage("osama","osos","127.0.0.1",1,"image123");
    // ofstream out;
    // out.open("trial.jpg",ios_base::out | ios_base::binary);
    // out<<s;
    // out.close();
