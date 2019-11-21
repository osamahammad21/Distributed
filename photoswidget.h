#ifndef PHOTOSWIDGET_H
#define PHOTOSWIDGET_H

#include <QWidget>
#include "ImageModel.h"
#include "viewphotowindow.h"
namespace Ui {
class PhotosWidget;
}

class PhotosWidget : public QWidget
{
    Q_OBJECT

public:
    vector<ImageModel> images;
    explicit PhotosWidget(QWidget *parent = nullptr);
    void addImageToList(ImageModel image);
    void removeImageIfFound(string image_id);
    ~PhotosWidget();


private:
    Ui::PhotosWidget *ui;
    ViewPhotoWindow *window=nullptr;

};

#endif // PHOTOSWIDGET_H
