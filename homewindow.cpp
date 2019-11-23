#include "homewindow.h"
#include "ui_homewindow.h"
#include "allphotoswindow.h"
#include "photoswidget.h"
#include "QLabel"
#include "QMovie"
#include "userslist.h"
HomeWindow::HomeWindow(User * user, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HomeWindow)
{
    this->user = user;
    ui->setupUi(this);
    ui->tabWidget->addTab(new PhotosWidget(),"All Photos");
    ui->tabWidget->addTab(new PhotosWidget(),"My Photos");

    vector <string> usernames;
    user->getAllUsernames(usernames);
    ui->tabWidget->addTab(new usersList(user, usernames, nullptr),"All usernames");
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

}
