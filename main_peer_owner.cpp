#include "Peer.h"
using namespace std;
inline void split(string str, vector<string>& cont, char delim = ' ')
{
    stringstream ss(str);
    string token;
    while (getline(ss, token, delim)) {
        cont.push_back(token);
    }
}
int main()
{
    Peer peer(1);
    peer.setDS("127.0.0.1",50);
    Image img;
    img.setImageDir("habiba");
    img.setownerUsername("habiba");
    img.setImageId("flower");
    img.chooseImage("/home/admin/Desktop/osama/DistGUI/Distributed/small.jpg");
    struct userProperty prop;
    prop.user_name="osama";
    prop.views=2;
    img.properties.push_back(prop);
    img.writeProperties();
    img.steg();
    peer.addImageLocally("flower");
    while(true){
        string req = peer.getImageUpdates();
        vector<string>fields;
        split(req,fields,',');
        cout<<req<<endl;
        peer.sendImageAccess(fields[1],fields[0],fields[3],stoi(fields[4]),fields[2],10);
    }
    return 0;
    // cout<<"Waiting for upload"<<endl;
    // cout<<peer.uploadImage("token","osos")<<endl;
    // cout<<"Upload Done"<<endl;
}