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

    QPixmap bkgnd(BACKGROUND_PATH);
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);
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
        return;
    }
    if (imageName == NULL){
        ui->label_successMessage->setText("Must enter a valid image name");
        return;
    }
    if (imageName.toStdString().find(" ")!=-1){
        ui->label_successMessage->setText("Must enter a valid image name. Remove space characters");
        return;
    }
    Image im;
    im.setImageDir(user->getUsername());
    if (im.findImage(user->getUsername(), imageName.toStdString())){
        ui->label_successMessage->setText("Image name already exists. Choose different name.");
        return;
    }
    if (filename != NULL && imageName != NULL){
        Image image;
        image.setImageDir(user->getUsername());
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
    int status = user->logout();
    if( status == MSG_SUCCESS){
        hide();
        MainWindow * mainWindow = new MainWindow(user, this);
        mainWindow->show();
        destroy();
    } else {
        ui->label_successMessage->setText("Connection error. Try again later.");
        ui->label_successMessage->setVisible(true);
    }
}

void UploadPhotoWindow::on_pushButton_home_clicked()
{
    map<string, vector<imageSample>> samples;
    user->getUsersSamples(samples);
    HomeWindow *homeWindow = new HomeWindow(user, -10, samples, this);
    homeWindow->show();
    destroy();
}
