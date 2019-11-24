#ifndef IMAGESAMPLEWIDGET_H
#define IMAGESAMPLEWIDGET_H

#include <QWidget>
#include <string>
#include "user.h"
#include "viewphotowindow.h"
using namespace std;
namespace Ui {
class imageSampleWidget;
}

class imageSampleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit imageSampleWidget(string imageName ="", string preview="", string username="", User * user = nullptr, QWidget *parent = nullptr, QWidget * grandparent = nullptr);
    ~imageSampleWidget();

private slots:

    void on_pushButton_imageName_clicked();

    void on_pushButton_username_clicked();

private:
    Ui::imageSampleWidget *ui;
    string ownerUsername;
    string imageName;
    User * user;
    QWidget * grandparent;
};

#endif // IMAGESAMPLEWIDGET_H
