#include "photosettingswindow.h"
#include "ui_photosettingswindow.h"
#include "homewindow.h"

PhotoSettingsWindow::PhotoSettingsWindow(bool upload, Image image, User * user, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PhotoSettingsWindow)
{
    ui->setupUi(this);
    this->user = user;
    this->image = image;
    this->upload = upload;

    QPixmap bkgnd(BACKGROUND_PATH);
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);

    if (!upload){
        ui->pushButton_upload->setText("Change Settings");
        image.readProperties();
        struct userProperty prop;
        for (int i=0; i < image.properties.size(); i++)
            if (image.properties[i].views!=0){
                users.insert(std::pair<std::string, int>(image.properties[i].user_name, image.properties[i].views));
                users[image.properties[i].user_name] = image.properties[i].views;
            }   
        viewItemsInListWidget();    
    }
}

PhotoSettingsWindow::~PhotoSettingsWindow()
{
    delete ui;
}

void PhotoSettingsWindow::on_pushButton_add_clicked()
{
    std::string username = ui->lineEdit_username->text().toStdString();

    int views = ui->lineEdit_views->text().toInt();
    if (views!=0){
        users.insert(std::pair<std::string, int>(username, views));
        users[username] = views;
    }
    ui->lineEdit_username->clear();
    ui->lineEdit_views->clear();
    viewItemsInListWidget();
}

void PhotoSettingsWindow::on_pushButton_delete_clicked()
{
    std::string username = ui->lineEdit_usernameDEL->text().toStdString();
//    users.erase(username);
    users[username] = 0;
    ui->lineEdit_usernameDEL->clear();
    viewItemsInListWidget();
}

void PhotoSettingsWindow::viewItemsInListWidget()
{
    QList<QListWidgetItem*> items = ui->listWidget->selectedItems();
    qDeleteAll(ui->listWidget->selectedItems());
    std::map<std::string, int>::iterator it;
    ui->listWidget->clear();
    for ( it = users.begin(); it != users.end(); it++ )
    {
        ui->listWidget -> addItem(QString::fromStdString(it->first + " " + std::to_string(it->second)));
    }
}

void PhotoSettingsWindow::on_pushButton_upload_clicked()
{
    struct userProperty prop;//this struct represent the setting for each user for the image

    std::map<std::string, int>::iterator it;
    ui->listWidget->clear();
    if (!upload){
        image.readProperties();
        for ( it = users.begin(); it != users.end(); it++ )
        {
            prop.user_name = it->first;
            prop.views = it->second;
            cout << "Not upload\n";
            bool found = false;
            for (int i=0; i < image.properties.size(); i++){
                if (image.properties[i].user_name == it->first){
                    cout << "found\n";
                    found = true;
                    if (image.properties[i].views != it->second){
                        string imageName;
                        image.getImageId(imageName);
                        cout << "sending image access to " <<it->first << endl;
                        int status = user->sendImageAccess(it->first, imageName, it->second);
                        if (status == MSG_SUCCESS){
                            cout << "succeeded to send access\n";
                            image.properties[i].views = it->second;
                            cout << "Properties " << image.properties[i].views << endl;
                            break;
                        } else {
                            cout << "failed to send acces\n";
                            ui->label_status->setText("Connection error. User's views not updated.");
                            break;
                        }
                     } else {
                         break;
                     }
//                    
                }
                if (!found){
                    cout << "not found\n";
                    string imageName;
                    image.getImageId(imageName);
                    cout << "sending image access to " <<  it->first << endl;
                    int status = user->sendImageAccess(it->first, imageName, it->second);
                    if (status == MSG_SUCCESS){
                        cout << "succeeded to send access\n";
                        image.properties.push_back(prop);
                        break;
                    } else {
                        cout << "failed to send acces\n";
                        ui->label_status->setText("Connection error. User's views not updated.");
                        break;
                    }
                }
            }
        }
        cout << "updating properties" << endl;
        image.updateProperties();
        image.desteg();
        hide();
        map<string, vector<imageSample>> samples;
        user->getUsersSamples(samples);
        vector <pair<string, int>> onlineUsers;
        user->getOnlineUsers(onlineUsers);
        HomeWindow *homeWindow = new HomeWindow(user, -10, samples, onlineUsers, this);        homeWindow->show();
        destroy();
        return;
    }
    else {
        for ( it = users.begin(); it != users.end(); it++ )
        {
            prop.user_name = it->first;
            prop.views = it->second;
            cout << "upload \n";
            image.properties.push_back(prop);
        }

        image.writeProperties();
        image.steg();
        int uploadStatus = user->uploadPhoto(image);
        map<string, vector<imageSample>> samples;
        user->getUsersSamples(samples);
        hide();
        vector <pair<string, int>> onlineUsers;
        user->getOnlineUsers(onlineUsers);
        HomeWindow *homeWindow = new HomeWindow(user, -10, samples, onlineUsers, this);        homeWindow->show();
        destroy();
        return;
    }
}

void PhotoSettingsWindow::on_pushButton_logout_clicked()
{
    int status = user->logout();
    if( status == MSG_SUCCESS){
        hide();
        MainWindow * mainWindow = new MainWindow(user, this);
        mainWindow->show();
        destroy();
    } else {
        ui->label_status->setText("Connection error. Try again later.");
        ui->label_status->setVisible(true);
    }
}

void PhotoSettingsWindow::on_pushButton_home_clicked()
{
    map<string, vector<imageSample>> samples;
    int status1 = user->getUsersSamples(samples);
    vector <pair<string, int>> onlineUsers;
    int status2 = user->getOnlineUsers(onlineUsers);
    if (status1 == status2 && status1 == MSG_SUCCESS){
        HomeWindow *homeWindow = new HomeWindow(user, -10, samples, onlineUsers, this);
        homeWindow->show();
        destroy();
    }
    else {
        ui->label_status->setText("Connection error. Try again later.");
    }
}
