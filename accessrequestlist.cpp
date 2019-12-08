#include "accessrequestlist.h"
#include "ui_accessrequestlist.h"

accessRequestList::accessRequestList(User * user, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::accessRequestList)
{
    ui->setupUi(this);

    ui->listWidget->setStyleSheet("background-color: transparent");

    this->user = user;

    for (int i =0; i < user->requesters.size(); i++){
        addUser(user->requesters[i].username, user->requesters[i].imageName, user->requesters[i].id);
    }
}
void accessRequestList :: addUser(string imageName, string requesterUsername, int id)
{
    AccessRequest* iWidget = new AccessRequest(user, imageName, requesterUsername, id, this);
    QListWidgetItem* lwi = new QListWidgetItem("");
    lwi->setSizeHint(QSize(0,110));
    lwi->setFlags(lwi->flags() & ~Qt::ItemIsSelectable);
    ui->listWidget->addItem(lwi);
    ui->listWidget->setItemWidget(lwi, iWidget);
}

accessRequestList::~accessRequestList()
{
    delete ui;
}
