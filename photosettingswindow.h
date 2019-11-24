#ifndef PHOTOSETTINGSWINDOW_H
#define PHOTOSETTINGSWINDOW_H

#include <QMainWindow>
#include <map>
#include <string>
#include <QString>
#include "user.h"
namespace Ui {
class PhotoSettingsWindow;
}

class PhotoSettingsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PhotoSettingsWindow(bool upload, Image image, User * user, QWidget *parent = nullptr);
    ~PhotoSettingsWindow();

private slots:
    void on_pushButton_add_clicked();
    void on_pushButton_delete_clicked();
    void viewItemsInListWidget();

    void on_pushButton_upload_clicked();

    void on_pushButton_logout_clicked();

    void on_pushButton_home_clicked();

private:
    Ui::PhotoSettingsWindow *ui;
    std::map <std::string, int> users;
    Image image;
    User * user;
    bool upload;
};

#endif // PHOTOSETTINGSWINDOW_H
