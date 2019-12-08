#include "viewsrequests.h"
#include "ui_viewsrequests.h"

viewsRequests::viewsRequests(Peer * peer, string token, string ownerUsername, string requesterUsername, string imageName, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::viewsRequests)
{
    ui->setupUi(this);
    this->ownerUsername = ownerUsername;
    this->requesterUsername = requesterUsername;
    this->imageName = imageName;
    this->peer = peer;
    this->token = token;

    QPixmap bkgnd(BACKGROUND_PATH);
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);

    ui->label_info->setText(QString:: fromStdString("User " + requesterUsername + " requests access\nto image " + imageName));
    image.setImageDir(ownerUsername);
    image.findImage(ownerUsername, imageName);
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

     connect(ui->pushButton_reject, SIGNAL(clicked()), this, SLOT(on_pushButton_reject_clicked()));
     connect(ui->pushButton_giveAccess, SIGNAL(clicked()), this, SLOT(on_pushButton_giveAccess_clicked()));
}

viewsRequests::~viewsRequests()
{
    delete ui;
}

inline void split(string str, vector<string>& cont, char delim = ' ')
{
    try {
      cont.clear();
    } catch (exception e) {

    }

    stringstream ss(str);
    string token;
    while (getline(ss, token, delim)) {
        cont.push_back(token);
    }
}

void viewsRequests::on_pushButton_giveAccess_clicked()
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

        cout << "Message sent to peer\n";
        peer->sendImageAccess(token, requesterUsername, imageName, views);
        destroy();
    }
}

void viewsRequests::on_pushButton_reject_clicked()
{
    destroy();
}
