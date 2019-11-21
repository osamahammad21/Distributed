#ifndef ALLPHOTOSWINDOW_H
#define ALLPHOTOSWINDOW_H

#include <QMainWindow>
#include "ImageModel.h"
#include "viewphotowindow.h"
#include <vector>
namespace Ui {
class AllPhotosWindow;
}

class AllPhotosWindow : public QMainWindow
{
    Q_OBJECT

public:
    vector<ImageModel> images;
    explicit AllPhotosWindow(QWidget *parent = nullptr);
    void addImageToList(ImageModel image);
    void removeImageIfFound(string image_id);
    ~AllPhotosWindow();

private slots:
    void on_pushButton_show_clicked();

private:
    Ui::AllPhotosWindow *ui;
    ViewPhotoWindow *window=nullptr;

};

#endif // ALLPHOTOSWINDOW_H
