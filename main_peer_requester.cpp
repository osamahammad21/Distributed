#include "Peer.h"
#include <list>
using namespace std;
void DOS(int port)
{
    Peer peer(port);
    peer.setDS("10.7.57.207",88);
    string username = "osaos"+to_string(port);
    cout<<peer.signup(username,"1234")<<endl;
}
int main()
{
    
    string start;
    cin>>start;
    // for(int i = 50;i<150;i++)
    // {
        
    //     if(i!=88)
    //     {
    //         std::thread *second=new thread(DOS,i);
    //         usleep(100);
    //     }
    // }
    Peer peer(3);
    Image img1;
    img1.setImageDir("osama");
    string imgstr = peer.getImage("osama","habiba","10.7.57.221",1,"flower");
    img1.writeImage(imgstr , "habiba" ,"flower");
    // cout<<peer.requestImageAccess("osama","habiba","10.7.57.221",1,"flower")<<endl;
    // cin>>start;
    Image img;
    if(img.setImageDir("osama")<0)
        return 0;
    if(!img.findImage("habiba","flower"))
        return 0;
    img.readProperties();
    for(int i = 0;i<img.properties.size();i++)
        cout<<img.properties[i].user_name<<" "<<img.properties[i].views<<endl;
    // img.removeMiddleFiles();
    // peer.setDS("10.7.57.207",88);
    // cout<<peer.signup("user1","pass1")<<endl;
    // string token = peer.signup("user2","pass2");
    // cout<<token<<endl;
    // cout<<peer.getImage("koko","osos","127.0.0.1",1,"864820");
    // peer.setTimeOut(2);
    // cout<<peer.getPortnIP(token,"user1")<<endl;
    // peer.removeTimeOut();
    // cout<<peer.login("ss","123")<<endl;

//     int i =55;
//     Peer peer(i);
//     peer.setDS("127.0.0.1",88);
//     string username = "osaos"+to_string(i);
//     cout<<peer.signup(username,"1234")<<endl;
//     sleep(1);
//     peer.startStatusUpdates("osaos"+to_string(i)+"1234");
//     sleep(1);
//     cout<<peer.uploadImage("osaos"+to_string(i)+"1234","IMAGENAME","BASE64")<<endl;
//     sleep(1);
//     cin>>start;
// }
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
}
