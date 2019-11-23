#ifndef IMAGESAMPLEWIDGET_H
#define IMAGESAMPLEWIDGET_H

#include <QWidget>
#include <string>
#include "user.h"
using namespace std;
namespace Ui {
class imageSampleWidget;
}

class imageSampleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit imageSampleWidget(QWidget *parent = nullptr, string imageName ="", string preview="", string username="", User * user = nullptr);
    ~imageSampleWidget();

private slots:

    void on_pushButton_imageName_clicked();

    void on_pushButton_username_clicked();

private:
    Ui::imageSampleWidget *ui;
    string ownerUsername;
    string imageName;
    User * user;
};

#endif // IMAGESAMPLEWIDGET_H
