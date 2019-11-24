#ifndef VIEWMYPHOTOS_H
#define VIEWMYPHOTOS_H

#include <QWidget>
#include "user.h"
#include "myphoto.h"
#include <fstream>
namespace Ui {
class ViewMyPhotos;
}

class ViewMyPhotos : public QWidget
{
    Q_OBJECT

public:
    explicit ViewMyPhotos(User * user, QWidget *parent = nullptr);
    void addSample(string imageName, string preview, Image image);
    ~ViewMyPhotos();

private:
    Ui::ViewMyPhotos *ui;
    User * user;
    QWidget * parent = parent;
};

#endif // VIEWMYPHOTOS_H
