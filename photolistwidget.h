#ifndef PHOTOLISTWIDGET_H
#define PHOTOLISTWIDGET_H

#include <QWidget>
#include <ImageModel.h>
namespace Ui {
class PhotoListWidget;
}

class PhotoListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PhotoListWidget(QWidget *parent = nullptr,ImageModel *image = nullptr);
    ~PhotoListWidget();

private slots:
    void on_pushButton_view_clicked();

private:
    Ui::PhotoListWidget *ui;
    ImageModel image;
};

#endif // PHOTOLISTWIDGET_H
