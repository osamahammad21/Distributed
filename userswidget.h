#ifndef USERSWIDGET_H
#define USERSWIDGET_H

#include <QWidget>
#include <string>
#include "viewimagesamples.h"
using namespace std;
namespace Ui {
class usersWidget;
}

class usersWidget : public QWidget
{
    Q_OBJECT

public:
    explicit usersWidget(User * user, string username, QWidget *parent);
    ~usersWidget();

private slots:
    void on_pushButton_viewSamples_clicked();

private:
    Ui::usersWidget *ui;
    User * user;
    QWidget *parent;
};

#endif // USERSWIDGET_H
