#include "myphoto.h"
#include "ui_myphoto.h"

myPhoto::myPhoto(User * user, string imageName, string preview, Image image, QWidget *parent, QWidget * grandparent) :
    QWidget(parent),
    ui(new Ui::myPhoto)
{
    ui->setupUi(this);
    this->user = user;
    this->imageName = imageName;
    this->grandparent = grandparent;
    this->image = image;

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
    cout << "Delete clicked. Not implemented yet\n";
    //not implemented by DS yet
}

void myPhoto::on_pushButton_viewImage_clicked()
{
    ViewPhotoWindow * viewPhotoWindow = new ViewPhotoWindow(user, user->getUsername(), imageName);
    viewPhotoWindow->show();
    grandparent->close();
}
