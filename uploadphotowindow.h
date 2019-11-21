#ifndef UPLOADPHOTOWINDOW_H
#define UPLOADPHOTOWINDOW_H

#include <QMainWindow>
#include "photosettingswindow.h"
QT_BEGIN_NAMESPACE
namespace Ui { class UploadPhotoWindow; }
QT_END_NAMESPACE

class UploadPhotoWindow : public QMainWindow
{
    Q_OBJECT

public:
    UploadPhotoWindow(QWidget *parent = nullptr);
    ~UploadPhotoWindow();

private slots:
    void on_pushButton_choose_clicked();

    void on_pushButton_upload_clicked();

private:
    Ui::UploadPhotoWindow *ui;
    PhotoSettingsWindow *newWindow;
    QString filename ="";
};
#endif // UPLOADPHOTOWINDOW_H
