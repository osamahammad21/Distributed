#include "photosettingswindow.h"
#include "ui_photosettingswindow.h"
PhotoSettingsWindow::PhotoSettingsWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PhotoSettingsWindow)
{
    ui->setupUi(this);
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
    hide();
    //show grandparent (home)
    destroy();
}
