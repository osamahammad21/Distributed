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
    if (user->login(username.toStdString(), password.toStdString())){
        cout << "Login completed" << endl;
        HomeWindow *homeWindow = new HomeWindow(user, this);
        homeWindow->show();
        destroy();
    }
    else {
        ui->label_successMessage->setText("Wrong username or password");
    }

}

void MainWindow::on_pushButton_signup_clicked()
{
    RegisterWindow *registerwindow = new RegisterWindow(user, this);
    registerwindow->show();
    destroy();
}
