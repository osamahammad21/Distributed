#ifndef IMAGE_Hclass
#define IMAGE_Hclass
#include <string>
#include <fstream>
#include <iostream>
#include<vector>
#include "base64.h"
using namespace std;
#define USERNAME_ERROR -1
#define PATH_ERROR -2
#define IMAGE_ID_ERROR -3
#define PROPERTIES_ERROR -4
#define IMAGE_DIR "imagedir/"
#define COVER_IMAGE "imagedir/default.jpg"
struct userProperty
{
    string user_name;
    int views;
};
class Image
{
    private:
        string imageId;
        string ownerUsername;
        string stegImagePath;
        string destegImagePath;
        string propertiesPath;
        string originalImagePath;
    public:
        vector<struct userProperty> properties;
        Image();
        int readProperties();
        string extractImage();
        int updateProperties();
        string getAuthorizedImagePath();
        string getUnAuthorizedImagePath();
        int chooseImage(string path);
        bool findImage(string ownerusername,string imageId);
        int setImageId(string id);
        int getImageId(string& id);
        int setownerUsername(string username);
        int getownerUsername(string& username);
        int desteg();
        int steg();
        void writeImage(string image,string ownerusername,string imageId);
        void writeProperties();
        int steg(string secretfile,string coverfile,string stegofile);
        int desteg(string stegofile,string extractedfile);
        string getSmallScaleImage();
        void removeMiddleFiles();
        ~Image();
};
#endif 
