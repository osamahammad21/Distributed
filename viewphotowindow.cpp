#include "viewphotowindow.h"
#include "ui_viewphotowindow.h"
#include "string"
using namespace std;
//void ViewPhotoWindow::setImage(ImageModel image)
//{
//    this->image=image;
//    const QString filename =QString::fromStdString(image.imagePath);
//    const QString views = QString::fromStdString("Views: "+to_string(image.views));
//    QPixmap img(filename);
//    ui->label_image->setPixmap(img);
//    ui->label_image->setScaledContents(true);
//    ui->label_no_views->setText(views);
//    this->setWindowTitle(QString::fromStdString(image.image_name));
//}
ViewPhotoWindow::ViewPhotoWindow(User * user, string ownerUsername, string imageName, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ViewPhotoWindow)
{
    ui->setupUi(this);
    this->user = user;

    string photo = user->getImage(ownerUsername, imageName);

    Image * im = new Image();
    im->writeImage(photo, ownerUsername, imageName);
    im->desteg();

    string path;
    int i;
    cout << user->getUsername() << endl;
    cout << im -> properties.size()  << endl;
    bool existsInProperties = false;
    for (i=0; i< im->properties.size(); i++){

        cout << im->properties[i].user_name<< im->properties[i].views << endl;

        if (im->properties[i].user_name == user->getUsername()){
            ui->label_no_views->setText(QString::fromStdString(to_string( im->properties[i].views )));
            existsInProperties = true;
            cout << user -> getUsername();
            if(im->properties[i].views <= 0){
                path = im->getUnAuthorizedImagePath();
                cout << "have no access"<<endl;
            }
            else{
                path = im->getAuthorizedImagePath();
                im->properties[i].views--;
                cout << "have access" << endl;
            }
            break;
        }
    }

    if (!existsInProperties){
        path = im->getUnAuthorizedImagePath();
        cout << "user not in properties" << endl;
    }
    this->setWindowTitle(QString::fromStdString(image.image_name));
//    ofstream out;
//    string path = "out2_img.jpg";
//    out.open(path, ios_base::out | ios_base::binary);
//    out << photo;
//    out.close();

    int n = path.length();
    char *char_array=new char[n+1];
    strcpy(char_array, path.c_str());
    QPixmap pixmap(char_array);
    ui->label_image->setPixmap(pixmap);
    ui->label_image->show();
}

ViewPhotoWindow::~ViewPhotoWindow()
{
    delete ui;
}

void ViewPhotoWindow::on_pushButton_home_clicked()
{
    HomeWindow *homeWindow = new HomeWindow(user, this);
    homeWindow->show();
    destroy();
}

void ViewPhotoWindow::on_pushButton_logout_clicked()
{
    if( user->logout()){
        hide();
        MainWindow * mainWindow = new MainWindow(user, this);
        mainWindow->show();
        destroy();
    }
}
