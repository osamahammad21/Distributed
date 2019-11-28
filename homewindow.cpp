#include "homewindow.h"
#include "ui_homewindow.h"
#include "allphotoswindow.h"
#include "QLabel"
#include "QMovie"
#include "viewsamples.h"
#include "viewmyphotos.h"

HomeWindow::HomeWindow(User * user, int uploadStatus, map<string, vector<imageSample>> samples, QWidget *parent) :
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
