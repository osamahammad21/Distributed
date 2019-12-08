#ifndef ACCESSREQUEST_H
#define ACCESSREQUEST_H

#include <QWidget>
#include "user.h"
#include "Image.h"
namespace Ui {
class AccessRequest;
}

class AccessRequest : public QWidget
{
    Q_OBJECT

public:
    explicit AccessRequest(User * user, string imageName, string requesterUsername, int id, QWidget *parent);
    ~AccessRequest();

private slots:
    void on_pushButton_giveAccess_clicked();

    void on_pushButton_reject_clicked();

private:
    Ui::AccessRequest *ui;
    Image image;
    User *user;
    string requesterUsername;
    int id;
};

#endif // ACCESSREQUEST_H
