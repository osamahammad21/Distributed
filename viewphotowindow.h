#ifndef VIEWPHOTOWINDOW_H
#define VIEWPHOTOWINDOW_H

#include <QMainWindow>
#include "ImageModel.h"
#include "user.h"
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

private:
    ImageModel image;
    Ui::ViewPhotoWindow *ui;
    User * user;
};

#endif // VIEWPHOTOWINDOW_H
