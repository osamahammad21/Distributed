#include "Peer.h"
using namespace std;
int main()
{
    Peer peer(1);
    peer.setDS("127.0.0.1",50);
    Image img;
    img.setownerUsername("osos");
    img.setImageId("864820");
    img.chooseImage("small.jpg");
    struct userProperty prop;
    prop.user_name="koko";
    prop.views=5;
    img.properties.push_back(prop);
    img.writeProperties();
    img.steg();
    peer.addImageLocally("864820");
    peer.startStatusUpdates("osos123");
    while(true);
    // cout<<"Waiting for upload"<<endl;
    // cout<<peer.uploadImage("token","osos")<<endl;
    // cout<<"Upload Done"<<endl;
}