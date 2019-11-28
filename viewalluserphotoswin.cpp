#include "viewalluserphotoswin.h"
#include "ui_viewalluserphotoswin.h"

ViewAllUserPhotosWin::ViewAllUserPhotosWin(User * user, string ownerUsername, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ViewAllUserPhotosWin)
{
    ui->setupUi(this);
    this->user = user;
    this->ownerUsername = ownerUsername;

    QPixmap bkgnd(BACKGROUND_PATH);
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);

    ui->listWidget->setStyleSheet("background-color: transparent");

    vector <imageSample> allOwnerImages;
    user->getAllOwnerImages(ownerUsername, allOwnerImages);
        for (int i=0; i< allOwnerImages.size(); i++)
            addSample(ownerUsername, allOwnerImages[i].imageName, allOwnerImages[i].preview);
}

void ViewAllUserPhotosWin :: addSample(string username, string imageName, string preview)
{
    AllImagesForUser* Widget = new AllImagesForUser(user, ownerUsername, imageName, preview, this);
    QListWidgetItem* lwi = new QListWidgetItem("");
    lwi->setSizeHint(QSize(0,90));
    lwi->setFlags(lwi->flags() & ~Qt::ItemIsSelectable);
    ui->listWidget->addItem(lwi);
    ui->listWidget->setItemWidget(lwi, Widget);
}

ViewAllUserPhotosWin::~ViewAllUserPhotosWin()
{
    delete ui;
}

void ViewAllUserPhotosWin::on_pushButton_home_clicked()
{
    map<string, vector<imageSample>> samples;
    user->getUsersSamples(samples);
    HomeWindow *homeWindow = new HomeWindow(user, -10, samples, this);
    homeWindow->show();
    destroy();
}

void ViewAllUserPhotosWin::on_pushButton_logout_clicked()
{
    int status = user->logout();
    if(status == MSG_SUCCESS){
        hide();
        MainWindow * mainWindow = new MainWindow(user, this);
        mainWindow->show();
        destroy();
    } else {
        ui->label_status->setText("Connection error. Try again later.");
        ui->label_status->setVisible(true);
    }
}
