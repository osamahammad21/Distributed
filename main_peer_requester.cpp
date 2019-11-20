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
    int p;
    // list<boost::thread*> threads;
    // for(int i = 50;i<150;i++)
    // {
    //     boost::thread* name = new boost::thread(DOS,i);
    //     threads.push_back(name);
    // }
    for(int i = 50;i<150;i++)
    {
        // cout<<"waiting"<<endl;
        // cin>>p;
        // DOS(p);
        if(i!=88)
        {
            std::thread *second=new thread(DOS,i);
            usleep(100);
            // second->join();
        }
        // sleep(1);
    }
    // Peer peer("127.0.0.1",2);
    // peer.setDS("127.0.0.1",88);
    
    // cout<<<<endl;
    // cout<<peer.signup("osama","123")<<endl;
    // cout<<peer.uploadImage("osama","img1");
    // cout<<"Requesting Image\n";
    // string s = peer.getImage("osama","osos","127.0.0.1",1,"image123");
    // ofstream out;
    // out.open("trial.jpg",ios_base::out | ios_base::binary);
    // out<<s;
    // out.close();
}