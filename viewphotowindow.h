#ifndef VIEWPHOTOWINDOW_H
#define VIEWPHOTOWINDOW_H

#include <QMainWindow>
#include "ImageModel.h"
#include "user.h"
#include "homewindow.h"
#include "mainwindow.h"
namespace Ui {
class ViewPhotoWindow;
}

class ViewPhotoWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ViewPhotoWindow(User * user = nullptr, string ownerUsername="", string imageName="", QWidget *parent = nullptr);
    void setImage(ImageModel image);
    ~ViewPhotoWindow();

private slots:
    void on_pushButton_home_clicked();

    void on_pushButton_logout_clicked();

    void on_pushButton_more_views_clicked();

private:
    ImageModel image;
    Ui::ViewPhotoWindow *ui;
    User * user;
    string ownerUsername;
    string imageName;
};

#endif // VIEWPHOTOWINDOW_H
