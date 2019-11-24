#ifndef ALLIMAGESFORUSER_H
#define ALLIMAGESFORUSER_H

#include <QWidget>
#include "viewphotowindow.h"

namespace Ui {
class AllImagesForUser;
}

class AllImagesForUser : public QWidget
{
    Q_OBJECT

public:
    explicit AllImagesForUser(User * user, string ownerusername, QWidget *parent = nullptr);
    ~AllImagesForUser();

private slots:
    void on_pushButton_imageName_clicked();

private:
    Ui::AllImagesForUser *ui;
    User * user;
    QWidget * parent;
    string ownerusername;
};

#endif // ALLIMAGESFORUSER_H
