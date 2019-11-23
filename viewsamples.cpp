#include "viewsamples.h"
#include "ui_viewsamples.h"

viewSamples::viewSamples(User * user, map <string, vector<imageSample>> samples, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::viewSamples)
{
    ui->setupUi(this);
    this->user = user;

    map<string, vector<imageSample>>::iterator it;
    for ( it = samples.begin(); it != samples.end(); it++ )
    {
         for (int i = 0; i < it->second.size(); i++)
            addSample(it->first, it->second[i].imageName, it->second[i].preview);
    }
}

void viewSamples :: addSample(string username, string imageName, string preview)
{
    imageSampleWidget* iWidget = new imageSampleWidget(nullptr, imageName, preview, username, user);
    QListWidgetItem* lwi = new QListWidgetItem("");
    lwi->setSizeHint(QSize(0,90));
    lwi->setFlags(lwi->flags() & ~Qt::ItemIsSelectable);
    ui->listWidget->addItem(lwi);
    ui->listWidget->setItemWidget(lwi, iWidget);
}

viewSamples::~viewSamples()
{
    delete ui;
}
