#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include <QMainWindow>
#include "uploadphotowindow.h"
namespace Ui {
class HomeWindow;
}

class HomeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit HomeWindow(QWidget *parent = nullptr);
    ~HomeWindow();

private slots:
    void on_pushButton_uploaphoto_clicked();

private:
    Ui::HomeWindow *ui;
    UploadPhotoWindow * uploadWindow;
};

#endif // HOMEWINDOW_H
