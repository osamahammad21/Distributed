#include "registerwindow.h"
#include "ui_registerwindow.h"

RegisterWindow::RegisterWindow(User * user, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RegisterWindow)
{
    this->user = user;
    ui->setupUi(this);

    QPixmap bkgnd(BACKGROUND_PATH);
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);
}

RegisterWindow::~RegisterWindow()
{
    delete ui;
}

void RegisterWindow::on_pushButton_signup_clicked()
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

void RegisterWindow::on_pushButton_back_clicked()
{
    MainWindow * mainWindow = new MainWindow(user, this);
    mainWindow->show();
    destroy();
}
