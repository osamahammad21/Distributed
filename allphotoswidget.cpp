#include "allphotoswidget.h"
#include "ui_allphotoswidget.h"

AllPhotosWidget::AllPhotosWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AllPhotosWidget)
{
    ui->setupUi(this);
}

AllPhotosWidget::~AllPhotosWidget()
{
    delete ui;
}
