#include "Image.h"
inline bool exist (string name) {
    ifstream f(name.c_str());
    if(f.good())
    {
        f.close();
        return true;
    }
    return false;
}
Image::Image(){}
int Image::desteg(string stegofile,string extractedfile)
{
    string command = "steghide extract -sf "+stegofile+" -p groupH -xf "+extractedfile+" -f";
    system(command.c_str());
}
int Image::steg(string secretfile,string coverfile,string stegofile)
{
    string command = "steghide embed -cf "+coverfile+" -ef "+ secretfile+" -p groupH -sf "+stegofile+" -f";
    system(command.c_str());
}
int Image::desteg()
{
    destegImagePath = string(IMAGE_DIR)+ownerUsername+"_"+imageId+"_desteg"+".jpg";
    propertiesPath = string(IMAGE_DIR)+ownerUsername+"_"+imageId+"_properties"+".txt";
    //extracting desteg from steg
    desteg(stegImagePath,destegImagePath);
    //extracting properties from desteg
    desteg(destegImagePath,propertiesPath);
    return 0;
}
//properties are written
int Image::steg()
{
    propertiesPath = IMAGE_DIR+ownerUsername+"_"+imageId+"_properties"+".txt";
    destegImagePath = IMAGE_DIR+ownerUsername+"_"+imageId+"_desteg"+".jpg";
    stegImagePath = IMAGE_DIR+ownerUsername+"_"+imageId+"_steg"+".jpg";
    //Embedding properties into original to desteg
    steg(propertiesPath,destegImagePath,destegImagePath);
    //Embedding desteg into default to steg
    steg(destegImagePath,string(COVER_IMAGE),stegImagePath);
    return 0;
}
void Image::removeMiddleFiles()
{
    string command="rm "+destegImagePath+" "+propertiesPath+" -f";
    system(command.c_str());
}
void Image::writeProperties()
{
    propertiesPath = IMAGE_DIR+ownerUsername+"_"+imageId+"_properties"+".txt";
    ofstream output;
    output.open(propertiesPath);
    for(int i = 0;i<properties.size();i++)
    {
        output<<properties[i].user_name<<" "<<properties[i].views;
        if(i<properties.size()-1)
            output<<endl;
    }
    output.close();
}
void Image::copyjpgfile(string source,string target)
{
    ifstream fin(source,ios::binary);
    string extract = string(istreambuf_iterator<char>(fin), istreambuf_iterator<char>());
    fin.close(); 
    ofstream out;
    out.open(target,ios_base::out | ios_base::binary);
    out<<extract;
    out.close();
}
bool Image::findImage(string ownerusername,string imageId)
{
    setownerUsername(ownerusername);
    setImageId(imageId);
    stegImagePath=string(IMAGE_DIR)+ownerUsername+"_"+imageId+"_steg.jpg";
    if(!exist(stegImagePath))
        return false;
    return true;    
}
int Image::updateProperties()
{
    desteg();
    writeProperties();
    steg();
    string command = "rm "+destegImagePath+" "+propertiesPath+" -f";
    system(command.c_str());
}
int Image::readProperties()
{
    desteg();
    ifstream input;
    input.open(propertiesPath);
    while (!input.eof())
    {
        userProperty prop;
        input>>prop.user_name;
        input>>prop.views;
        if(prop.user_name=="DoNotUse")
            break;
        properties.push_back(prop);
    }
    input.close();
    string command = "rm "+destegImagePath+" "+propertiesPath+" -f";
    system(command.c_str());
    return 0;
}

int Image::chooseImage(string path)
{
    // originalImagePath=path;
    destegImagePath = IMAGE_DIR+ownerUsername+"_"+imageId+"_desteg"+".jpg";
    copyjpgfile(path,destegImagePath);
    return 0;
}
int Image::setImageId(string id)
{
    imageId=id;
    return 0;
}
int Image::getImageId(string& id)
{
    if(imageId=="")
        return USERNAME_ERROR;
    id= imageId;
    return 0;
}
int Image::setownerUsername(string username)
{
    ownerUsername=username;
    return 0;
}
int Image::getownerUsername(string& username)
{
    if(ownerUsername=="")
        return USERNAME_ERROR;
    username= ownerUsername;
    return 0;
}
string Image::extractImage()
{
    //steg();
    ifstream fin(stegImagePath,ios::binary);
    string extract = string(istreambuf_iterator<char>(fin), istreambuf_iterator<char>());
    fin.close();
    return extract;
}
string Image::getAuthorizedImagePath()
{
    desteg();
    string command = "rm "+propertiesPath+" -f";
    system(command.c_str());
    return destegImagePath;
}
string Image::getUnAuthorizedImagePath()
{
    return stegImagePath;
}
Image::~Image()
{}