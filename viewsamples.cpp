#include "viewsamples.h"
#include "ui_viewsamples.h"

viewSamples::viewSamples(User * user, map <string, vector<imageSample>> samples, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::viewSamples)
{
    ui->setupUi(this);
    this->user = user;
    this->parent = parent;

    map<string, vector<imageSample>>::iterator it;
    for ( it = samples.begin(); it != samples.end(); it++ )
    {
        if(it->first != user->getUsername())
            for (int i = 0; i < it->second.size(); i++)
                addSample(it->first, it->second[i].imageName, it->second[i].preview);

    }
}

void viewSamples :: addSample(string username, string imageName, string preview)
{
    imageSampleWidget* iWidget = new imageSampleWidget(imageName, preview, username, user, this, this->parent);
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
