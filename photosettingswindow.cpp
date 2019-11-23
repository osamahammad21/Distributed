#include "photosettingswindow.h"
#include "ui_photosettingswindow.h"
#include "homewindow.h"

PhotoSettingsWindow::PhotoSettingsWindow(Image image, User * user, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PhotoSettingsWindow)
{
    ui->setupUi(this);
    this->user = user;
    this->image = image;
}

PhotoSettingsWindow::~PhotoSettingsWindow()
{
    delete ui;
}

void PhotoSettingsWindow::on_pushButton_add_clicked()
{
    std::string username = ui->lineEdit_username->text().toStdString();

    int views = ui->lineEdit_views->text().toInt();
    if (views!=0){
        users.insert(std::pair<std::string, int>(username, views));
        users[username] = views;
    }
    ui->lineEdit_username->clear();
    ui->lineEdit_views->clear();
    viewItemsInListWidget();
}

void PhotoSettingsWindow::on_pushButton_delete_clicked()
{
    std::string username = ui->lineEdit_usernameDEL->text().toStdString();
    users.erase(username);
    ui->lineEdit_usernameDEL->clear();
    viewItemsInListWidget();
}

void PhotoSettingsWindow::viewItemsInListWidget()
{
    QList<QListWidgetItem*> items = ui->listWidget->selectedItems();
    qDeleteAll(ui->listWidget->selectedItems());
    std::map<std::string, int>::iterator it;
    ui->listWidget->clear();
    for ( it = users.begin(); it != users.end(); it++ )
    {
        ui->listWidget -> addItem(QString::fromStdString(it->first + " " + std::to_string(it->second)));
    }
}

void PhotoSettingsWindow::on_pushButton_upload_clicked()
{
    struct userProperty prop;//this struct represent the setting for each user for the image

    std::map<std::string, int>::iterator it;
    ui->listWidget->clear();
    for ( it = users.begin(); it != users.end(); it++ )
    {
        prop.user_name = it->first;
        prop.views = it->second;
        image.properties.push_back(prop);
    }
    image.writeProperties();
    image.steg();
    image.removeMiddleFiles();

    if (user->uploadPhoto(image)){
        hide();
        HomeWindow *homeWindow = new HomeWindow(user, this);
        homeWindow->show();
        destroy();
    }


}

void PhotoSettingsWindow::on_pushButton_logout_clicked()
{

}