#include "viewalluserphotoswin.h"
#include "ui_viewalluserphotoswin.h"

ViewAllUserPhotosWin::ViewAllUserPhotosWin(User * user, string ownerUsername, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ViewAllUserPhotosWin)
{
    ui->setupUi(this);
    this->user = user;
    this->ownerUsername = ownerUsername;

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
    HomeWindow *homeWindow = new HomeWindow(user, this);
    homeWindow->show();
    destroy();
}

void ViewAllUserPhotosWin::on_pushButton_logout_clicked()
{
    if( user->logout()){
        hide();
        MainWindow * mainWindow = new MainWindow(user, this);
        mainWindow->show();
        destroy();
    }
}
