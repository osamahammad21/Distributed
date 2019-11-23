#include "userslist.h"
#include "ui_userslist.h"

usersList::usersList(User * user, vector <string> usernames, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::usersList)
{
    ui->setupUi(this);
    this->user = user;
    for ( int i = 0; i< usernames.size(); i++ )
    {
        addUser(usernames[i]);
    }
    this->parent = parent;
}

void usersList::addUser(string username)
{
    usernames.push_back(username);
    usersWidget* uWidget = new usersWidget(user, username, parent);
    QListWidgetItem* lwi = new QListWidgetItem("");
    lwi->setSizeHint(QSize(0,70));
    lwi->setFlags(lwi->flags() & ~Qt::ItemIsSelectable);
    ui->listWidget_users->addItem(lwi);
    ui->listWidget_users->setItemWidget(lwi, uWidget);
}

usersList::~usersList()
{
    delete ui;
}
