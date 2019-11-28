#include "imagesamplewidget.h"
#include "ui_imagesamplewidget.h"

imageSampleWidget::imageSampleWidget(string imageName, string preview, string username, User * user, QWidget *parent, QWidget * grandparent) :
    QWidget(parent),
    ui(new Ui::imageSampleWidget)
{
    ui->setupUi(this);
    ui->pushButton_imageName->setText(QString::fromStdString(imageName));
    ui->pushButton_username->setText(QString::fromStdString(username));

    ui->pushButton_imageName->setStyleSheet("background-color: white");
    ui->pushButton_username->setStyleSheet("background-color: white");
    this->user = user;
    this->ownerUsername = username;
    this->imageName = imageName;
    this->grandparent = grandparent;

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

void imageSampleWidget::on_pushButton_imageName_clicked()
{
    ViewPhotoWindow * viewPhotoWindow = new ViewPhotoWindow(user, ownerUsername, imageName);
    viewPhotoWindow->show();
    grandparent->close();

}

void imageSampleWidget::on_pushButton_username_clicked()
{
    ViewAllUserPhotosWin * allUserPhotos = new ViewAllUserPhotosWin(user, ownerUsername, nullptr);
    allUserPhotos->show();
    grandparent->close();
}
