#include "homewindow.h"
#include "ui_homewindow.h"
#include "allphotoswindow.h"
#include "photoswidget.h"
#include "QLabel"
#include "QMovie"
HomeWindow::HomeWindow(User * user, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HomeWindow)
{
    this->user = user;
    ui->setupUi(this);
    ui->tabWidget->addTab(new PhotosWidget(),"All Photos");
    ui->tabWidget->addTab(new PhotosWidget(),"My Photos");
}

HomeWindow::~HomeWindow()
{
    delete ui;
}

void HomeWindow::on_pushButton_uploaphoto_clicked()
{
    hide();
    uploadWindow = new UploadPhotoWindow(user, this);
    uploadWindow->show();
    destroy();
}
