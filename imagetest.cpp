#include "Image.h"
#include <iostream>
using namespace std;
int test =1;
int main()
{
    Image image;
    struct userProperty prop;
    switch (test)
    {
        case 0:
            image.setownerUsername("osos");
            image.setImageId("image123");
            image.chooseImage(string(IMAGE_DIR)+"small.jpg");
            for(int i = 0;i<image.properties.size();i++)
            {
                cout<<image.properties[i].user_name<<" "<<image.properties[i].views;
                if(i<image.properties.size()-1)
                    cout<<endl;
            }
            prop.user_name="osama";
            prop.views=5;
            image.properties.push_back(prop);
            image.writeProperties();
            image.steg();
            image.removeMiddleFiles();
            break;
        case 1:
            if(!image.findImage("osos","image123"))
                return 0;
            image.readProperties();
            for(int i = 0;i<image.properties.size();i++)
            {
                if(image.properties[i].user_name=="osama"&&image.properties[i].views>0)
                {
                    image.properties[i].views--;
                    cout<<image.properties[i].views;
                }
            }
            image.updateProperties(); 
            cout<<endl;
            cout<<image.getAuthorizedImagePath()<<" "<<image.getUnAuthorizedImagePath();
            break;
        default:
        
            break;
    }
    

    // image.chooseImage(string(IMAGE_DIR)+"small.jpg");
    // string img = image.extractImage();
    // ofstream out;
    // out.open("trial.jpg",ios_base::out | ios_base::binary);
    // out<<img;
    // out.close();
}