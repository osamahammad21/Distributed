#ifndef ONLINEUSERSLIST_H
#define ONLINEUSERSLIST_H

#include <QWidget>
#include <string>
#include "onlineuser.h"
namespace Ui {
class onlineUsersList;
}

class onlineUsersList : public QWidget
{
    Q_OBJECT

public:
    explicit onlineUsersList(User * user, QWidget *parent = nullptr);
    void addUser(string username, int status);
    ~onlineUsersList();

private:
    Ui::onlineUsersList *ui;
    User * user;
    QWidget *parent;
};

#endif // ONLINEUSERSLIST_H
