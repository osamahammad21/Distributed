#ifndef VIEWPHOTOWINDOW_H
#define VIEWPHOTOWINDOW_H

#include <QMainWindow>
#include "ImageModel.h"
namespace Ui {
class ViewPhotoWindow;
}

class ViewPhotoWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ViewPhotoWindow(QWidget *parent = nullptr);
    void setImage(ImageModel image);
    ~ViewPhotoWindow();

private:
    ImageModel image;
    Ui::ViewPhotoWindow *ui;
};

#endif // VIEWPHOTOWINDOW_H
