#include "accessrequest.h"
#include "ui_accessrequest.h"

AccessRequest::AccessRequest(User * user, string imageName, string requesterUsername, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AccessRequest)
{
    ui->setupUi(this);
    this->user = user;
    this->image = image;
    this->requesterUsername = requesterUsername;

    this->requesterUsername = requesterUsername;

    QPixmap bkgnd(BACKGROUND_PATH);
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);

    ui->label_info->setText(QString:: fromStdString("User " + requesterUsername + " requests access\nto image " + imageName));
    image.setImageDir(user->getUsername());
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

AccessRequest::~AccessRequest()
{
    delete ui;
}

void AccessRequest::on_pushButton_giveAccess_clicked()
{
    if (ui->lineEdit_views->text() != NULL){
        int views = stoi(ui->lineEdit_views->text().toStdString());
        image.readProperties();
        for (int index = 0; index < image.properties.size(); index++){
            if (image.properties[index].user_name == requesterUsername){
                image.properties[index].views = views;
                break;
            }
        }
        image.updateProperties();
        image.desteg();
        string imageName;
        image.getImageId(imageName);

        user->sendImageAccess(requesterUsername,imageName, views);
        image.removeMiddleFiles();
        destroy();
    }
}

void AccessRequest::on_pushButton_reject_clicked()
{
    destroy();
}
