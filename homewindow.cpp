#include "homewindow.h"
#include "ui_homewindow.h"
#include "allphotoswindow.h"
#include "photoswidget.h"
#include "QLabel"
#include "QMovie"
#include "viewsamples.h"
#include "viewmyphotos.h"

HomeWindow::HomeWindow(User * user, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HomeWindow)
{
    this->user = user;
    ui->setupUi(this);

    map<string, vector<imageSample>> samples;
    user->getUsersSamples(samples);
    ui->tabWidget->addTab( new viewSamples(user, samples, this),"User's samples");
    ui->tabWidget->addTab( new ViewMyPhotos(user, this),"My Photos");

}

HomeWindow::~HomeWindow()
{
    delete ui;
}

void HomeWindow::on_pushButton_uploaphoto_clicked()
{
    hide();
    UploadPhotoWindow * uploadWindow = new UploadPhotoWindow(user, this);
    uploadWindow->show();
    destroy();
}

void HomeWindow::on_pushButton_logout_clicked()
{
    if( user->logout()){
        hide();
        MainWindow * mainWindow = new MainWindow(user, this);
        mainWindow->show();
        destroy();
    }
}
