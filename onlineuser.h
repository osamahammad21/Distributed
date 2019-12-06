#ifndef ONLINEUSER_H
#define ONLINEUSER_H

#include <QWidget>
#include <vector>
#include "user.h"
#include "viewalluserphotoswin.h"
namespace Ui {
class onlineUser;
}

class onlineUser : public QWidget
{
    Q_OBJECT

public:
    explicit onlineUser( User * user, QWidget * grandparent, string ownerUsername, int status, QWidget *parent);
    ~onlineUser();

private slots:
    void on_pushButton_viewAllImages_clicked();

private:
    Ui::onlineUser *ui;
    User * user;
    QWidget * grandparent;
    string otherUsername;

};

#endif // ONLINEUSER_H
