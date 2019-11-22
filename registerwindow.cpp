#include "registerwindow.h"
#include "ui_registerwindow.h"

RegisterWindow::RegisterWindow(User * user, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RegisterWindow)
{
    this->user = user;
    ui->setupUi(this);
}

RegisterWindow::~RegisterWindow()
{
    delete ui;
}

void RegisterWindow::on_pushButton_signup_clicked()
{
    QString username = ui->lineEdit_username->text();
    QString password = ui->lineEdit_password->text();

    if (user->signup(username.toStdString(), password.toStdString())){
        cout << "Sign Up Completed" << endl;
        HomeWindow *homeWindow = new HomeWindow(user, this);
        homeWindow->show();
        destroy();
    }
    else {
        ui->label_successMessage->setText("Username taken");
    }

    //insert in database
}
