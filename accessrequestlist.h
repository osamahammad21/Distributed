#ifndef ACCESSREQUESTLIST_H
#define ACCESSREQUESTLIST_H

#include <QWidget>
#include <string>
#include "accessrequest.h"
#include "user.h"

using namespace std;
namespace Ui {
class accessRequestList;
}

class accessRequestList : public QWidget
{
    Q_OBJECT

public:
    explicit accessRequestList(User * user, QWidget *parent = nullptr);
    void addUser(string imageName, string requesterUsername, int id);
    ~accessRequestList();

private:
    Ui::accessRequestList *ui;
    User * user;
};

#endif // ACCESSREQUESTLIST_H
