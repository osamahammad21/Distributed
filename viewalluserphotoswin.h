#ifndef VIEWALLUSERPHOTOSWIN_H
#define VIEWALLUSERPHOTOSWIN_H

#include <QMainWindow>
#include "homewindow.h"
#include "mainwindow.h"
#include "allimagesforuser.h"
namespace Ui {
class ViewAllUserPhotosWin;
}

class ViewAllUserPhotosWin : public QMainWindow
{
    Q_OBJECT

public:
    explicit ViewAllUserPhotosWin(User * user, string ownerUsername, QWidget *parent = nullptr);
    ~ViewAllUserPhotosWin();

private slots:
    void on_pushButton_home_clicked();

    void on_pushButton_logout_clicked();
    void addSample(string username, string imageName, string preview);

private:
    Ui::ViewAllUserPhotosWin *ui;
    User * user;
    string ownerUsername;
};

#endif // VIEWALLUSERPHOTOSWIN_H
