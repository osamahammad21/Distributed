#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(User * user,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->user = user;
    ui->setupUi(this);
    QPixmap bkgnd(BACKGROUND_PATH);
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_login_clicked()
{
    QString username = ui->lineEdit_username->text();
    QString password = ui->lineEdit_password->text();
    int status = user->login(username.toStdString(), password.toStdString());
    if (status == MSG_SUCCESS){
        map<string, vector<imageSample>> samples;
        user->getUsersSamples(samples);
        HomeWindow *homeWindow = new HomeWindow(user, -10, samples, this);
        homeWindow->show();
        destroy();
    }
    else if (status == PARAM_ERROR)
    {
        ui->label_successMessage->setText("Wrong username or password");
    } else {
        ui->label_successMessage->setText("Connection error. Please try again later.");
    }
}

void MainWindow::on_pushButton_signup_clicked()
{
    QString username = ui->lineEdit_username->text();
    QString password = ui->lineEdit_password->text();
    int status = user->signup(username.toStdString(), password.toStdString());
    if (status == MSG_SUCCESS){
        map<string, vector<imageSample>> samples;
        user->getUsersSamples(samples);
        HomeWindow *homeWindow = new HomeWindow(user, -10, samples, this);
        homeWindow->show();
        destroy();
    }
    else if (status == PARAM_ERROR){
        ui->label_successMessage->setText("Username taken");
    } else {
        ui->label_successMessage->setText("Connection error. Try again later.");
    }
}
