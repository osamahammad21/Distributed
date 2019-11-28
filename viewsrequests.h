#ifndef VIEWSREQUESTS_H
#define VIEWSREQUESTS_H

#include <QMainWindow>
#include <string>
#include <QWidget>
#include "Image.h"
#include "user.h"
#define BACKGROUND_PATH "../Distributed/background.jpeg"
using namespace std;

namespace Ui {
class viewsRequests;
}

class viewsRequests : public QMainWindow
{
    Q_OBJECT

public:
     explicit viewsRequests(Peer * peer, string token, string ownerUsername, string requesterUsername = "", string imageName = "", QWidget *parent = nullptr);
    ~viewsRequests();
    int getStatus();

private slots:
    void on_pushButton_giveAccess_clicked();

    void on_pushButton_reject_clicked();

private:
    Ui::viewsRequests *ui;
    Peer *peer;
    string ownerUsername;
    string requesterUsername;
    string imageName;
    Image image;
    string token;
};

#endif // VIEWSREQUESTS_H
