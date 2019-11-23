#include "viewphotowindow.h"
#include "ui_viewphotowindow.h"
#include "string"
using namespace std;
void ViewPhotoWindow::setImage(ImageModel image)
{
    this->image=image;
    const QString filename =QString::fromStdString(image.imagePath);
    const QString views = QString::fromStdString("Views: "+to_string(image.views));
    QPixmap img(filename);
    ui->label_image->setPixmap(img);
    ui->label_image->setScaledContents(true);
    ui->label_no_views->setText(views);
    this->setWindowTitle(QString::fromStdString(image.image_name));
}
ViewPhotoWindow::ViewPhotoWindow(User * user, string ownerUsername, string imageName, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ViewPhotoWindow)
{
    ui->setupUi(this);
    this->user = user;

    string photo = user->getImage(ownerUsername, imageName);

    Image * im = new Image();
    im->writeImage(photo, ownerUsername, imageName);
    im->desteg();
    vector <struct userProperty> prop = im->properties;

    string path;
    int i;
    for (i=0; i<prop.size(); i++){
        if (prop[i].user_name == user->getUsername()){
            ui->label_no_views->setText(QString::fromStdString(to_string( prop[i].views )));

            if(prop[i].views <= 0)
                path = im->getUnAuthorizedImagePath();
            else{
                path = im->getAuthorizedImagePath();
                prop[i].views--;
            }
            break;
        }
    }

    if (i >= prop.size()){
        path = im->getUnAuthorizedImagePath();
    }
    this->setWindowTitle(QString::fromStdString(image.image_name));
    ofstream out;
//    string path = "out2_img.jpg";
    out.open(path, ios_base::out | ios_base::binary);
    out << photo;
    out.close();

//    const string pathConst = path;
//    QPixmap pixmap();
//    pixmap.setImage(path);
//    ui->label_image->setPixmap(pixmap);
//    ui->label_image->show();
}

ViewPhotoWindow::~ViewPhotoWindow()
{
    delete ui;
}
