#include "allimagesforuser.h"
#include "ui_allimagesforuser.h"

AllImagesForUser::AllImagesForUser(User * user, string ownerusername, string imageName, string preview, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AllImagesForUser)
{
    ui->setupUi(this);
    this->user = user;
    this->parent = parent;
    this->ownerusername = ownerusername;
    this->imageName = imageName;
    ui->pushButton_imageName->setText(QString::fromStdString(imageName));
    ui->pushButton_imageName->setStyleSheet("background-color: white");

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

AllImagesForUser::~AllImagesForUser()
{
    delete ui;
}

void AllImagesForUser::on_pushButton_imageName_clicked()
{
    ViewPhotoWindow * viewPhotoWindow = new ViewPhotoWindow(user, ownerusername, imageName, nullptr);
    viewPhotoWindow->show();
    parent->close();
}
