#ifndef MYPHOTO_H
#define MYPHOTO_H

#include <QWidget>
#include "user.h"
#include <string>
#include "viewphotowindow.h"
namespace Ui {
class myPhoto;
}

class myPhoto : public QWidget
{
    Q_OBJECT

public:
    explicit myPhoto(User * user, string imageName, string preview, Image image,  QWidget *parent = nullptr, QWidget * grandparent = nullptr);
    ~myPhoto();

private slots:
    void on_pushButton_changeSettings_clicked();

    void on_pushButton_delete_clicked();

    void on_pushButton_viewImage_clicked();

private:
    Ui::myPhoto *ui;
    User * user;
    string imageName;
    QWidget * grandparent;
    Image image;

};

#endif // MYPHOTO_H
