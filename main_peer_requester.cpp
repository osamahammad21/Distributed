#include "Peer.h"
#include <list>
using namespace std;
int main()
{
    
    string start;
    cin>>start;
    Peer peer(3);
    cout<<"I am here"<<endl;
    peer.setDS("10.40.34.123",88);
    // cout<<peer.login("user1","pass1")<<endl;
    string token1 = peer.signup("test3","123");
    cout<<token1<<endl;
    string token2 = peer.signup("test4","123");
    cout<<token2<<endl;
    // peer.setTimeOut(2);
    cout<<peer.getPortnIP(token2,"test3")<<endl;
    cout<<peer.uploadImage(token1,"img1", "imggg1");
    cout<<peer.uploadImage(token1,"img2", "imggg2");
    cout<<peer.uploadImage(token1,"img3", "imggg3");
    usleep(2000000);

    cout << peer.removeImage(token1, "img2")<< endl;
        usleep(2000000);

        cout << peer.removeImage(token1, "img1")<< endl;
            usleep(2000000);

    cout << "get all imgs " << peer.getAllImagesFromDS( token1) << endl;



    /*int i = 0;
    while(1)
    {
    cout << "image upload number i " << i++;
    string imgname = "meh" + to_string(i);
    string imtnnname = "mehh" + to_string(i);
    cout << "upload " << i << endl;
    cout<<peer.uploadImage(token1,imgname,imtnnname);
    usleep(2000000);
    cout << peer.removeImage(token1, imgname);
    cout << "removed " << i << endl;
    }*/
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
