#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "registerwindow.h"
MainWindow::MainWindow(User * user,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->user = user;
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_login_clicked()
{
    QString username = ui->lineEdit_username->text();
    QString password = ui->lineEdit_password->text();
    user->login(username.toStdString(), password.toStdString());

    //check database

}

void MainWindow::on_pushButton_signup_clicked()
{
    RegisterWindow *registerwindow = new RegisterWindow(this);
    registerwindow->show();
    destroy();
}
