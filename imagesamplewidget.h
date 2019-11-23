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

private:
    Ui::imageSampleWidget *ui;
    string username;
    User * user;
};

#endif // IMAGESAMPLEWIDGET_H
