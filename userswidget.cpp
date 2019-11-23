#include "userswidget.h"
#include "ui_userswidget.h"

usersWidget::usersWidget(User * user, string username, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::usersWidget)
{
    ui->setupUi(this);
    this->user = user;
    if(username != "")
        ui -> label_username->setText(QString::fromStdString(username));
    this->parent = parent;
}

usersWidget::~usersWidget()
{
    delete ui;
}

void usersWidget::on_pushButton_viewSamples_clicked()
{
    string username = ui->label_username->text().toStdString();
    vector <imageSample> samples;
    user->getSamples(username, samples);
    ViewImageSamples * viewImageSample = new ViewImageSamples(user, samples, username, parent);
    viewImageSample->show();
}
