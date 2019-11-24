#include "photolistwidget.h"
#include "ui_photolistwidget.h"
#include "viewphotowindow.h"
PhotoListWidget::PhotoListWidget(QWidget *parent,ImageModel* image) :
    QWidget(parent),
    ui(new Ui::PhotoListWidget),
    image(*image)
{
    ui->setupUi(this);
    ui->label_image_name->setText(QString::fromStdString(image->image_name));
    ui->label_owner_name->setText(QString::fromStdString(image->owner_username));
}

PhotoListWidget::~PhotoListWidget()
{
    delete ui;
}

void PhotoListWidget::on_pushButton_view_clicked()
{
//   ViewPhotoWindow *window = new ViewPhotoWindow(this);
//   if(rand()%2)
//       image.imagePath="/home/admin/Downloads/instagram-logo-png-paint-brush-colour-1.png";
//   else
//       image.imagePath="/home/admin/Downloads/Download-Vector-PNG-File.png";
//   window->setImage(image);
//   window->show();
//   window->raise();
}
