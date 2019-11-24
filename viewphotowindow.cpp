#include "viewphotowindow.h"
#include "ui_viewphotowindow.h"
#include "string"
using namespace std;
ViewPhotoWindow::ViewPhotoWindow(User * user, string ownerUsername, string imageName, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ViewPhotoWindow)
{   
    ui->setupUi(this);
    this->user = user;

    Image tempImage;
    string photo;
    Image * im;
    if (tempImage.findImage(ownerUsername, imageName)){
        photo = tempImage.extractImage();
        im = &tempImage;
    }
    else {
        string photo = user->getImage(ownerUsername, imageName);
        im = new Image();
        im->writeImage(photo, ownerUsername, imageName);
        im->desteg();
    }

    string path;
    int i;
    if (user->getUsername() != ownerUsername){
        im->readProperties();
        bool existsInProperties = false;
        for (i=0; i< im->properties.size(); i++){

            if (im->properties[i].user_name == user->getUsername()){
                ui->label_no_views->setText(QString::fromStdString(to_string( im->properties[i].views )));
                existsInProperties = true;

                if(im->properties[i].views <= 0){
                    path = im->getUnAuthorizedImagePath();
                }
                else{
                    path = im->getAuthorizedImagePath();
                    im->properties[i].views--;
                }
                break;
            }
        }

        im->writeProperties();

        if (!existsInProperties){
            ui->label_no_views->setText(QString::fromStdString("0"));
            path = im->getUnAuthorizedImagePath();
        }
    } else {
        path = im->getAuthorizedImagePath();
        ui->label_no_views->setText("Owner");
        ui->pushButton_more_views->setVisible(false);
    }
    this->setWindowTitle(QString::fromStdString(image.image_name));

    int n = path.length();
    char *char_array=new char[n+1];
    strcpy(char_array, path.c_str());
    QPixmap pixmap(char_array);
    ui->label_image->setPixmap(pixmap);
    ui->label_image->show();

    im->steg();
    im->removeMiddleFiles();
}

ViewPhotoWindow::~ViewPhotoWindow()
{
    delete ui;
}

void ViewPhotoWindow::on_pushButton_home_clicked()
{
    HomeWindow *homeWindow = new HomeWindow(user, this);
    homeWindow->show();
    destroy();
}

void ViewPhotoWindow::on_pushButton_logout_clicked()
{
    if( user->logout()){
        hide();
        MainWindow * mainWindow = new MainWindow(user, this);
        mainWindow->show();
        destroy();
    }
}
