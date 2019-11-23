#ifndef USERSLIST_H
#define USERSLIST_H

#include <QWidget>
#include <string>
#include "userswidget.h"
#include <QListWidgetItem>
#include <vector>

using namespace std;
namespace Ui {
class usersList;
}

class usersList : public QWidget
{
    Q_OBJECT

public:
    explicit usersList(User * user, vector <string> usernames, QWidget *parent);
    void addUser(string username);
    ~usersList();

private:
    vector <string> usernames;
    Ui::usersList *ui;
    User * user;
    QWidget *parent;
};

#endif // USERSLIST_H
