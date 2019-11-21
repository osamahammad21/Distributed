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
ViewPhotoWindow::ViewPhotoWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ViewPhotoWindow)
{
    ui->setupUi(this);
}

ViewPhotoWindow::~ViewPhotoWindow()
{
    delete ui;
}
