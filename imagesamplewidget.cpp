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
    string photo;
    Image * im = new Image();
    im->setImageDir(user->getUsername());
    if (im->findImage(ownerUsername,imageName)){
        photo = im->extractImage();
        ViewPhotoWindow * viewPhotoWindow = new ViewPhotoWindow(user, ownerUsername, imageName, im, photo, nullptr);
        viewPhotoWindow->show();
        grandparent->close();
    }
    else {
        int status = user->getImage(ownerUsername, imageName, photo);
        if (status == CONN_FAILURE){
            ui->label_status->setText("Connection error. Try again later");
        }
        else {
            im = new Image();
            im->setImageDir(user->getUsername());
            im->writeImage(photo, ownerUsername, imageName);
            im->desteg();
            ViewPhotoWindow * viewPhotoWindow = new ViewPhotoWindow(user, ownerUsername, imageName, im, photo, nullptr);
            viewPhotoWindow->show();
            grandparent->close();
        }
    }
}

void imageSampleWidget::on_pushButton_username_clicked()
{

    vector <imageSample> allOwnerImages;
    int status = user->getAllOwnerImages(ownerUsername, allOwnerImages);
    if (status == MSG_SUCCESS){
        ViewAllUserPhotosWin * allUserPhotos = new ViewAllUserPhotosWin(user, ownerUsername, allOwnerImages, nullptr);
        allUserPhotos->show();
        grandparent->close();
    } else if (status == CONN_FAILURE){
        ui->label_status->setText("Connection error. Try again later");
    }

}