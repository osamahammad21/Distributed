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
UploadPhotoWindow::UploadPhotoWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::UploadPhotoWindow)
{
    ui->setupUi(this);
}

UploadPhotoWindow::~UploadPhotoWindow()
{
    delete ui;
}


void UploadPhotoWindow::on_pushButton_choose_clicked()
{
    QString filter = "PNG files (*.png) ;; JPG files (*.JPG) ;; JPEG files (*.JPEG)";
    filename = QFileDialog :: getOpenFileName(this, "Choose file", QDir::homePath(), filter);
    QPixmap img(filename);
    ui->label->setPixmap(img);
    ui->label->setScaledContents(true);
}

void UploadPhotoWindow::on_pushButton_upload_clicked()
{
    if (filename != NULL){
        hide();
        newWindow = new PhotoSettingsWindow(this);
        newWindow->show();
        destroy();
    }
}
