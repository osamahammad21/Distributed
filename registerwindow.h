#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QMainWindow>
#include "user.h"
#include "homewindow.h"
namespace Ui {
class RegisterWindow;
}

class RegisterWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RegisterWindow(User * user, QWidget *parent = nullptr);
    ~RegisterWindow();

private slots:

    void on_pushButton_signup_clicked();

    void on_pushButton_back_clicked();

private:
    Ui::RegisterWindow *ui;
    User * user;
};

#endif // REGISTERWINDOW_H
