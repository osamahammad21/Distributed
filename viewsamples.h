#ifndef VIEWSAMPLES_H
#define VIEWSAMPLES_H

#include <QWidget>
#include "user.h"
#include "imagesamplewidget.h"

namespace Ui {
class viewSamples;
}

class viewSamples : public QWidget
{
    Q_OBJECT

public:
    explicit viewSamples(User * user, map <string, vector<imageSample>> samples, QWidget *parent = nullptr);
    void addSample(string username, string imageName, string preview);
    ~viewSamples();

private:
    Ui::viewSamples *ui;
    User * user;
    QWidget * parent;
};

#endif // VIEWSAMPLES_H
