#include "onlineuserslist.h"
#include "ui_onlineuserslist.h"

onlineUsersList::onlineUsersList(User * user, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::onlineUsersList)
{
    ui->setupUi(this);
    this->user = user;
    this->parent = parent;

     ui->listWidget->setStyleSheet("background-color: transparent");

    //call getOnlineUsers
}


void onlineUsersList :: addUser(string username, int status)
{
    onlineUser* iWidget = new onlineUser(user, parent, username, status, this);
    QListWidgetItem* lwi = new QListWidgetItem("");
    lwi->setSizeHint(QSize(0,110));
    lwi->setFlags(lwi->flags() & ~Qt::ItemIsSelectable);
    ui->listWidget->addItem(lwi);
    ui->listWidget->setItemWidget(lwi, iWidget);
}

onlineUsersList::~onlineUsersList()
{
    delete ui;
}
