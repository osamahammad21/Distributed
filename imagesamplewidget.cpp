#include "imagesamplewidget.h"
#include "ui_imagesamplewidget.h"

imageSampleWidget::imageSampleWidget(QWidget *parent, string imageName, string preview, string username, User * user) :
    QWidget(parent),
    ui(new Ui::imageSampleWidget)
{
    ui->setupUi(this);
    ui->pushButton_imageName->setText(QString::fromStdString(imageName));
    ui->pushButton_username->setText(QString::fromStdString(username));
    this->user = user;

    preview = base64_decode(preview);
    preview = base64_decode(preview);

    ofstream out;
    string path = "out_img.jpg";
    out.open(path, ios_base::out | ios_base::binary);
    out << preview;
    out.close();

    QPixmap pixmap("out_img.jpg");
    ui->label_preview->setPixmap(pixmap);
    ui->label_preview->show();
}

imageSampleWidget::~imageSampleWidget()
{
    delete ui;
}
