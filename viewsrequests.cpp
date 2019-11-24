#include "viewsrequests.h"
#include "ui_viewsrequests.h"

viewsRequests::viewsRequests(User * user, string requesterUsername, string imageName, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::viewsRequests)
{
    ui->setupUi(this);
    this->user = user;
    this->requesterUsername = requesterUsername;
    this->imageName = imageName;

    ui->label_info->setText(QString:: fromStdString("User " + requesterUsername + " requests access\nto image " + imageName));
//    image.setImageDir(user->getUsername());
    image.findImage(user->getUsername(), imageName);
    image.desteg();
    string preview = image.getSmallScaleImage();

    preview = base64_decode(preview);
    preview = base64_decode(preview);

    ofstream out;
    string path = "out4_img.jpg";
    out.open(path, ios_base::out | ios_base::binary);
    out << preview;
    out.close();

    QPixmap pixmap("out4_img.jpg");
    ui->label_preview->setPixmap(pixmap);
    ui->label_preview->show();
}

viewsRequests::~viewsRequests()
{
    delete ui;
}

void viewsRequests::on_pushButton_giveAccess_clicked()
{
    int views = stoi(ui->lineEdit_views->text().toStdString());
    image.readProperties();
    for (int index = 0; index < image.properties.size(); index++){
        if (image.properties[index].user_name == requesterUsername){
            image.properties[index].views = views;
            break;
        }
    }
    image.writeProperties();
    //call functoion that will send the peer that it got access
    destroy();
}

void viewsRequests::on_pushButton_reject_clicked()
{
    destroy();
}
