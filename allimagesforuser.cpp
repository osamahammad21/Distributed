#include "allimagesforuser.h"
#include "ui_allimagesforuser.h"

AllImagesForUser::AllImagesForUser(User * user, string ownerusername, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AllImagesForUser)
{
    ui->setupUi(this);
    this->user = user;
    this->parent = parent;
    this->ownerusername = ownerusername;
    ui->pushButton_username->setText(QString::fromStdString(ownerusername));


}

AllImagesForUser::~AllImagesForUser()
{
    delete ui;
}

void AllImagesForUser::on_pushButton_imageName_clicked()
{
    ViewPhotoWindow * viewPhotoWindow = new ViewPhotoWindow(user, ownerUsername, imageName);
    viewPhotoWindow->show();
    parent->close();
}
