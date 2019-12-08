#include "onlineuser.h"
#include "ui_onlineuser.h"

onlineUser::onlineUser( User * user, QWidget * grandparent, string otherUsername, int status, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::onlineUser)
{
    ui->setupUi(this);
    this->user = user;
    this->grandparent = grandparent;
    this->otherUsername = otherUsername;

    ui->pushButton_viewAllImages->setStyleSheet("background-color: white");


    if (status == 1){
        QPixmap pixmap("green.png");
        ui->label_online->setPixmap(pixmap.scaled(10,10,Qt::KeepAspectRatio));
        ui->label_online->show();
    }
    else{
        QPixmap pixmap("red.png");
        ui->label_online->setPixmap(pixmap.scaled(10,10,Qt::KeepAspectRatio));
        ui->label_online->show();
    }
    ui->label_username->setText(QString::fromStdString(otherUsername));
}

onlineUser::~onlineUser()
{
    delete ui;
}

void onlineUser::on_pushButton_viewAllImages_clicked()
{
    vector <imageSample> allOwnerImages;
    int status = user->getAllOwnerImages(otherUsername, allOwnerImages);
    if (status == MSG_SUCCESS){
        ViewAllUserPhotosWin * allUserPhotos = new ViewAllUserPhotosWin(user, otherUsername, allOwnerImages, nullptr);
        allUserPhotos->show();
        grandparent->close();
    } else if (status == CONN_FAILURE){
        ui->label_status->setText("Connection error. Try again later");
    }
}
