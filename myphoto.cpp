#include "myphoto.h"
#include "ui_myphoto.h"

myPhoto::myPhoto(User * user, string imageName, string preview, Image image, QWidget *parent, QWidget * grandparent) :
    QWidget(parent),
    ui(new Ui::myPhoto)
{
    ui->setupUi(this);
    this->user = user;
    this->imageName = imageName;
    this->parent = parent;
    this->grandparent = grandparent;
    this->image = image;

    ui->pushButton_delete->setStyleSheet("background-color: white");
    ui->pushButton_viewImage->setStyleSheet("background-color: white");
    ui->pushButton_changeSettings->setStyleSheet("background-color: white");

    preview = base64_decode(preview);
    preview = base64_decode(preview);

    ofstream out;
    string path = "out3_img.jpg";
    out.open(path, ios_base::out | ios_base::binary);
    out << preview;
    out.close();

    QPixmap pixmap("out3_img.jpg");
    ui->label_preview->setPixmap(pixmap);
    ui->label_preview->show();

    ui->label_imageName->setText(QString:: fromStdString(imageName));
}

myPhoto::~myPhoto()
{
    delete ui;
}

void myPhoto::on_pushButton_changeSettings_clicked()
{
    PhotoSettingsWindow * settingsWin = new PhotoSettingsWindow(false, image, user, nullptr);
    settingsWin->show();
    grandparent->close();
}

void myPhoto::on_pushButton_delete_clicked()
{
    int status = user->removeImage(imageName);
    if (status == CONN_FAILURE)
        ui->label_status->setText("Connection error. Try again later.");
    destroy();
    setVisible(false);
}

void myPhoto::on_pushButton_viewImage_clicked()
{
    string photo;
    Image * im = new Image();
    im->setImageDir(user->getUsername());
    if (im->findImage(user->getUsername(),imageName)){
        photo = im->extractImage();
        ViewPhotoWindow * viewPhotoWindow = new ViewPhotoWindow(user, user->getUsername(), imageName, im, photo, nullptr);
        viewPhotoWindow->show();
        grandparent->close();
    }  
}
