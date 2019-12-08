#include "homewindow.h"
#include "ui_homewindow.h"
#include "allphotoswindow.h"
#include "QLabel"
#include "QMovie"
#include "viewsamples.h"
#include "viewmyphotos.h"

HomeWindow::HomeWindow(User * user, int uploadStatus, map<string, vector<imageSample>> samples, vector<pair<string, int>> onlineUsers, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HomeWindow)
{
    this->user = user;
    ui->setupUi(this);

    QPixmap bkgnd(BACKGROUND_PATH);
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);

    ui->tabWidget->addTab( new viewSamples(user, samples, this),"User's samples");
    ui->tabWidget->addTab( new ViewMyPhotos(user, this),"My Photos");
    ui->tabWidget->addTab( new onlineUsersList(user,  onlineUsers, this), "Online Users");
    ui->tabWidget->addTab( new accessRequestList(user, this), "Access requests");

    switch (uploadStatus){
        case CONN_FAILURE:
            ui->label_uploadStatus->setText("Connection error. Try again later.");
        break;
        case PARAM_ERROR:
             ui->label_uploadStatus->setText("Upload failed. Try again later.");
        break;
        case MSG_SUCCESS:
             ui->label_uploadStatus->setText("Upload succeeded");
        break;
        case 10:
            ui->label_uploadStatus->setText("Change settings succeeded");
        break;
        default:
            ui->label_uploadStatus->clear();

    }
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
    int status = user->logout();
    if( status == MSG_SUCCESS){
        hide();
        MainWindow * mainWindow = new MainWindow(user, this);
        mainWindow->show();
        destroy();
    } else {
        ui->label_uploadStatus->setText("Connection error. Try again later.");
        ui->label_uploadStatus->setVisible(true);
    }
}

void HomeWindow::on_pushButton_refresh_clicked()
{
    ui->tabWidget->clear();

    map<string, vector<imageSample>> samples;
    int status = user->getUsersSamples(samples);
    vector <pair<string, int>> onlineUsers;
    int status2 = user->getOnlineUsers(onlineUsers);
    if (status == MSG_SUCCESS && status == status2){
        ui->tabWidget->addTab( new viewSamples(user, samples, this),"User's samples");
        ui->tabWidget->addTab( new ViewMyPhotos(user, this),"My Photos");
        ui->tabWidget->addTab( new onlineUsersList(user, onlineUsers, this), "Online Users");
        ui->tabWidget->addTab( new accessRequestList(user, this), "Access requests");
    }
    else {
        ui->label_uploadStatus->setText("Connection error. Try again later.");
        ui->label_uploadStatus->setVisible(true);
    }
}
