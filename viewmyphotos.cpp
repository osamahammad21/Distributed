#include "viewmyphotos.h"
#include "ui_viewmyphotos.h"

ViewMyPhotos::ViewMyPhotos(User * user, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ViewMyPhotos)
{
    ui->setupUi(this);
    this->user = user;
    this->parent = parent;

    vector <imageSample> myPhotos;
    user->getMyImages(myPhotos);
    cout << myPhotos.size() << endl;
    for (int i=0; i<myPhotos.size(); i++)
        addSample(myPhotos[i].imageName, myPhotos[i].preview, myPhotos[i].im);
}

void ViewMyPhotos :: addSample(string imageName, string preview, Image image)
{
    myPhoto* widget = new myPhoto(user, imageName, preview, image, this, parent);
    QListWidgetItem* lwi = new QListWidgetItem("");
    lwi->setSizeHint(QSize(0,90));
    lwi->setFlags(lwi->flags() & ~Qt::ItemIsSelectable);
    ui->listWidget->addItem(lwi);
    ui->listWidget->setItemWidget(lwi, widget);
}

ViewMyPhotos::~ViewMyPhotos()
{
    delete ui;
}
