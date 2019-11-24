#ifndef VIEWSREQUESTS_H
#define VIEWSREQUESTS_H

#include <QMainWindow>
#include "user.h"
#include <string>
#include <QWidget>
using namespace std;

namespace Ui {
class viewsRequests;
}

class viewsRequests : public QMainWindow
{
    Q_OBJECT

public:
     explicit viewsRequests(User * user = nullptr, string requesterUsername = "", string imageName = "", QWidget *parent = nullptr);
    ~viewsRequests();

private slots:
    void on_pushButton_giveAccess_clicked();

    void on_pushButton_reject_clicked();

private:
    Ui::viewsRequests *ui;
    User * user;
    string requesterUsername;
    string imageName;
    Image image;
};

#endif // VIEWSREQUESTS_H
