#include "uploadphotowindow.h"
#include "ui_uploadphotowindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QLineEdit>
#include <QDir>
#include <QPixmap>
#include <QLabel>
#include <QImageReader>
#include <QPainter>
UploadPhotoWindow::UploadPhotoWindow(User * user, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::UploadPhotoWindow)
{
    this->user = user;
    ui->setupUi(this);
}

UploadPhotoWindow::~UploadPhotoWindow()
{
    delete ui;
}


void UploadPhotoWindow::on_pushButton_choose_clicked()
{
    ui->label_successMessage->setText("");
    QString filter = "PNG files (*.png) ;; JPG files (*.JPG) ;; JPEG files (*.JPEG)";
    filename = QFileDialog :: getOpenFileName(this, "Choose file", QDir::homePath(), filter);
    QPixmap img(filename);
    ui->label->setPixmap(img);
    ui->label->setScaledContents(true);
}

void UploadPhotoWindow::on_pushButton_upload_clicked()
{
    QString imageName = ui->lineEdit_imageName->text();
    if (filename == NULL){
        ui->label_successMessage->setText("Must enter a valid image");
    }
    if (imageName == NULL){
        ui->label_successMessage->setText("Must enter a valid image name");
    }
    if (filename != NULL && imageName != NULL){
        Image image;
        image.setownerUsername(this->user->getUsername());
        image.setImageId(imageName.toStdString());
        image.chooseImage(filename.toStdString());

        hide();
        newWindow = new PhotoSettingsWindow(true, image, user, this);
        newWindow->show();
        destroy();
    }
}

void UploadPhotoWindow::on_pushButton_logout_clicked()
{
    if( user->logout()){
        hide();
        MainWindow * mainWindow = new MainWindow(user, this);
        mainWindow->show();
        destroy();
    }
}
