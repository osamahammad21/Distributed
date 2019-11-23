#include "viewimagesamples.h"
#include "ui_viewimagesamples.h"

ViewImageSamples::ViewImageSamples(User * user, vector<imageSample> samples, string username, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ViewImageSamples)
{
    ui->setupUi(this);
    this->user = user;
    this->parent = parent;
    this->username = username;

    for ( int i = 0; i< samples.size(); i++ )
    {
        addSample(samples[i].imageName, samples[i].preview);
    }
}

void ViewImageSamples::addSample(string imageName, string preview)
{
    imageSampleWidget* iWidget = new imageSampleWidget(nullptr, imageName, preview, username, user);
    QListWidgetItem* lwi = new QListWidgetItem("");
    lwi->setSizeHint(QSize(0,70));
    lwi->setFlags(lwi->flags() & ~Qt::ItemIsSelectable);
    ui->listWidget->addItem(lwi);
    ui->listWidget->setItemWidget(lwi, iWidget);
}

ViewImageSamples::~ViewImageSamples()
{
    delete ui;
}

void ViewImageSamples::on_pushButton_logout_clicked()
{

}

void ViewImageSamples::on_pushButton_goBack_clicked()
{
    destroy();
}
