#include "infos.h"
#include "ui_infos.h"

QString Infos::InfoData;
Infos::Infos(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Infos)
{
    ui->setupUi(this);
}

void Infos::showEvent(QShowEvent *event)
{
    ui->label->setText(InfoData);
}

Infos::~Infos()
{
    delete ui;
}

void Infos::on_pushButton_released()
{
    close();
}
