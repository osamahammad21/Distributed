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
    ui->label_info->setText(QString:: fromStdString("User " + requesterUsername + " requests access\nto image " + imageName));
    image.setImageDir(ownerUsername);
    image.findImage(ownerUsername, imageName);
//    image.setImageDir(ownerUsername);
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
        image.writeProperties();


        cout << "Message sent from user to DS\n";
        string reply = peer->getPortnIP(token, requesterUsername);
        cout << "Reply received by user from DS\n";
        vector <string> args;
        split(reply, args, ',');
        cout << "Message sent to peer\n";
        peer->sendImageAccess(ownerUsername, requesterUsername, args[1], stoi(args[0]), imageName, views);
        destroy();
    }
}

void viewsRequests::on_pushButton_reject_clicked()
{
    destroy();
}
