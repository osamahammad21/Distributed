#include "allphotoswindow.h"
#include "ui_allphotoswindow.h"
#include "viewphotowindow.h"
#include <string>
using namespace std;
void addItemToList(AllPhotosWindow *window,string name,string username,int views,string imageId);
AllPhotosWindow::AllPhotosWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AllPhotosWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(QString::fromStdString("All Photos"));
    addItemToList(this,"flowers","osama21",20,"im21390294");
    addItemToList(this,"daumn","osama21",15,"im21390294");

}
void AllPhotosWindow::addImageToList(ImageModel image)
{
    images.push_back(image);
     ui->listWidget_photos->addItem(QString::fromStdString(image.image_name+"\n"+image.owner_username));
}
void AllPhotosWindow::removeImageIfFound(string image_id)
{
    for (size_t i = 0;i<images.size();i++)
        if(images[i].image_id==image_id)
            images.erase(images.begin()+i);
}
void addItemToList(AllPhotosWindow *window,string name,string username,int views,string imageId)
{
    ImageModel *image = new ImageModel();
    image->image_name=name;
    image->owner_username=username;
    image->views=views;
    image->image_id=imageId;
    window->addImageToList(*image);
}
AllPhotosWindow::~AllPhotosWindow()
{
    delete ui;
}
void AllPhotosWindow::on_pushButton_show_clicked()
{
     if(window==nullptr)
         window = new ViewPhotoWindow(this);
    if(rand()%2)
        images[ui->listWidget_photos->currentIndex().row()].imagePath="/home/admin/Downloads/instagram-logo-png-paint-brush-colour-1.png";
    else
        images[ui->listWidget_photos->currentIndex().row()].imagePath="/home/admin/Downloads/Download-Vector-PNG-File.png";
    window->setImage(images[ui->listWidget_photos->currentIndex().row()]);
    window->show();
    window->raise();
}
