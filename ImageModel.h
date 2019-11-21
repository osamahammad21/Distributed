#ifndef IMAGEMODEL_H
#define IMAGEMODEL_H
#include<string>
using namespace std;
class ImageModel
{
public:
    ImageModel(){}
    ~ImageModel(){}
    int views;
    string imagePath;
    string owner_username;
    string image_name;
    string image_id;
};
#endif // IMAGEMODEL_H
