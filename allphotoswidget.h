#ifndef ALLPHOTOSWIDGET_H
#define ALLPHOTOSWIDGET_H

#include <QWidget>

namespace Ui {
class AllPhotosWidget;
}

class AllPhotosWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AllPhotosWidget(QWidget *parent = nullptr);
    ~AllPhotosWidget();

private:
    Ui::AllPhotosWidget *ui;
};

#endif // ALLPHOTOSWIDGET_H
