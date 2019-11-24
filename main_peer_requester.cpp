#include "Peer.h"
#include <list>
using namespace std;
int main()
{
    
    string start;
    cin>>start;
    Peer peer(3);
    cout<<"I am here"<<endl;
    peer.setDS("127.0.0.1",88);
    // cout<<peer.login("user1","pass1")<<endl;
    string token1 = peer.signup("test3","123");
    cout<<token1<<endl;
    string token2 = peer.signup("test4","123");
    cout<<token2<<endl;
    string token3 = peer.signup("test5","123");
    // peer.setTimeOut(2);
    cout<<peer.getPortnIP(token2,"test3")<<endl;
    cout<<peer.uploadImage(token1,"img1", "imggg1");
    cout<<peer.uploadImage(token1,"img2", "imggg2");
    cout<<peer.uploadImage(token1,"img3", "imggg3");
    cout<<peer.uploadImage(token2,"img1", "imggg1");
    cout<<peer.uploadImage(token2,"img2", "imggg2");
    cout<<peer.uploadImage(token2,"img3", "imggg3");
    cout<<peer.uploadImage(token3,"img1", "imggg1");
    cout<<peer.uploadImage(token3,"img2", "imggg2");
    cout<<peer.uploadImage(token3,"img3", "imggg3");
    

    cin>>start;
    peer.removeImage(token1, "img1");
    peer.removeImage(token2, "img2");
    peer.removeImage(token3, "img3");
    cin>>start;
    peer.removeImage(token3, "img2");
    cin>>start;
    cout<<peer.getAllImagesFromDS(token1)<<endl;
}
