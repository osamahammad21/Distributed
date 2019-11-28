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
    users.erase(username);
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
    for ( it = users.begin(); it != users.end(); it++ )
    {
        prop.user_name = it->first;
        prop.views = it->second;
        image.properties.push_back(prop);
    }

    
    if (upload)
        image.writeProperties();
    else
        image.updateProperties();

    image.steg();

    if (upload){
        int uploadStatus = user->uploadPhoto(image); 
        map<string, vector<imageSample>> samples;
        user->getUsersSamples(samples);
        hide();
        HomeWindow *homeWindow = new HomeWindow(user, uploadStatus, samples, this);
        homeWindow->show();
        destroy();   
    } else {
        hide();
        map<string, vector<imageSample>> samples;
        user->getUsersSamples(samples);
        HomeWindow *homeWindow = new HomeWindow(user, MSG_SUCCESS, samples, this);
        homeWindow->show();
        destroy();
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
    user->getUsersSamples(samples);
    HomeWindow *homeWindow = new HomeWindow(user, -10, samples, this);
    homeWindow->show();
    destroy();
}
