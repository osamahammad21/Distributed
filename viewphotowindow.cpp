#include "viewphotowindow.h"
#include "ui_viewphotowindow.h"
#include "string"
using namespace std;
ViewPhotoWindow::ViewPhotoWindow(User * user, string ownerUsername, string imageName, Image * im, string photo,  QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ViewPhotoWindow)
{   
    ui->setupUi(this);
    this->user = user;
    this->ownerUsername = ownerUsername;
    this->imageName = imageName;

    string path;
    int i;
    if (user->getUsername() != ownerUsername){
        im->readProperties();
        bool existsInProperties = false;
        for (i=0; i< im->properties.size(); i++){
            if (im->properties[i].user_name == user->getUsername()){
                ui->label_no_views->setText(QString::fromStdString("Remaining Views = " + to_string(im->properties[i].views > 0 ? im->properties[i].views-1 : 0)));
                existsInProperties = true;

                if(im->properties[i].views <= 0){
                    path = im->getUnAuthorizedImagePath();
                }
                else{
                    path = im->getAuthorizedImagePath();
                    im->properties[i].views--;
                    ui->pushButton_more_views->setVisible(false);
                }
                break;
            }
        }

        im->updateProperties();
        im->desteg();

        if (!existsInProperties){
            ui->label_no_views->setText(QString::fromStdString("Remaining views = 0"));
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

    QPixmap bkgnd(BACKGROUND_PATH);
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);

    ui->pushButton_home->setStyleSheet("background-color: white");
    ui->pushButton_logout->setStyleSheet("background-color: white");
    ui->pushButton_more_views->setStyleSheet("background-color: white");
}

ViewPhotoWindow::~ViewPhotoWindow()
{
    delete ui;
}

void ViewPhotoWindow::on_pushButton_home_clicked()
{
    map<string, vector<imageSample>> samples;
    int status1 = user->getUsersSamples(samples);
    vector <pair<string, int>> onlineUsers;
    int status2 = user->getOnlineUsers(onlineUsers);
    if (status1 == status2 && status1 == MSG_SUCCESS){
        HomeWindow *homeWindow = new HomeWindow(user, -10, samples, onlineUsers, this);
        homeWindow->show();
        destroy();
    }
    else {
        ui->label_status->setText("Connection error. Try again later.");
    }
}

void ViewPhotoWindow::on_pushButton_logout_clicked()
{
    int status = user->logout();
    if(status == MSG_SUCCESS){
        hide();
        MainWindow * mainWindow = new MainWindow(user, nullptr);
        mainWindow->show();
        destroy();
    } else {
        ui->label_status->setText("Connection error. Try again later.");
        ui->label_status->setVisible(true);
    }
}

void ViewPhotoWindow::on_pushButton_more_views_clicked()
{
    int status = user->requestImageAccess(ownerUsername, imageName);
    if (status == CONN_FAILURE){
        ui->label_status->setText("Connection error. Try again later.");
        ui->label_status->setVisible(true);
    }
        
}
