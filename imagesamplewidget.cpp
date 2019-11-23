#include "imagesamplewidget.h"
#include "ui_imagesamplewidget.h"

imageSampleWidget::imageSampleWidget(QWidget *parent, string imageName, string preview, string username, User * user) :
    QWidget(parent),
    ui(new Ui::imageSampleWidget)
{
    ui->setupUi(this);
    ui->label_imageName->setText(QString::fromStdString(imageName));
    ui->label_preview->setText(QString::fromStdString(preview));
    this->username = username;
    this->user = user;
}

imageSampleWidget::~imageSampleWidget()
{
    delete ui;
}

void imageSampleWidget::on_pushButton_requestImage_clicked()
{
    string imageName = ui->label_imageName->text().toStdString();
    user->getImage(username, imageName);

}
