#include "photoswidget.h"
#include "ui_photoswidget.h"
#include"photolistwidget.h"
using namespace std;
void addItemToList(PhotosWidget *window,string name,string username,int views,string imageId);
PhotosWidget::PhotosWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PhotosWidget)
{
    ui->setupUi(this);
    addItemToList(this,"flowers","osama21",20,"im21390294");
    addItemToList(this,"daumn","osama21",15,"im21390294");
    addItemToList(this,"daumn","osama21",15,"im21390294");
    addItemToList(this,"daumn","osama21",15,"im21390294");
    addItemToList(this,"daumn","osama21",15,"im21390294");
    addItemToList(this,"daumn","osama21",15,"im21390294");
    addItemToList(this,"daumn","osama21",15,"im21390294");
}
void PhotosWidget::addImageToList(ImageModel image)
{
    images.push_back(image);
    PhotoListWidget* photo = new PhotoListWidget(nullptr,&image);
    QListWidgetItem* lwi = new QListWidgetItem("");
    lwi->setSizeHint(QSize(0,70));
    lwi->setFlags(lwi->flags() & ~Qt::ItemIsSelectable);
    ui->listWidget_photos->addItem(lwi);
    ui->listWidget_photos->setItemWidget(lwi, photo);
}
void PhotosWidget::removeImageIfFound(string image_id)
{
    for (size_t i = 0;i<images.size();i++)
        if(images[i].image_id==image_id)
            images.erase(images.begin()+i);
}
void addItemToList(PhotosWidget *window,string name,string username,int views,string imageId)
{
    ImageModel *image = new ImageModel();
    image->image_name=name;
    image->owner_username=username;
    image->views=views;
    image->image_id=imageId;
    window->addImageToList(*image);
}
PhotosWidget::~PhotosWidget()
{
    delete ui;
}
