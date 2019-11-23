#ifndef VIEWIMAGESAMPLES_H
#define VIEWIMAGESAMPLES_H

#include <QMainWindow>
#include "homewindow.h"
#include "imagesamplewidget.h"

namespace Ui {
class ViewImageSamples;
}

class ViewImageSamples : public QMainWindow
{
    Q_OBJECT

public:
    explicit ViewImageSamples(User * user, vector <imageSample> samples, string username,  QWidget *parent = nullptr);
    ~ViewImageSamples();

private slots:

    void on_pushButton_logout_clicked();
    void addSample(string imageName, string preview);

    void on_pushButton_goBack_clicked();

private:
    Ui::ViewImageSamples *ui;
    User * user;
    QWidget *parent;
    string username;
};

#endif // VIEWIMAGESAMPLES_H
