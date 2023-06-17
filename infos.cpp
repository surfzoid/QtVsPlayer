#include "infos.h"
#include "ui_infos.h"

QString Infos::InfoData;
Infos::Infos(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Infos)
{
    ui->setupUi(this);
    this->setAccessibleName("Infos");
    this->setWindowTitle("Infos");
}


void Infos::showEvent(QShowEvent *event)
{
    ui->label->setText(InfoData);
}

Infos::~Infos()
{
    delete ui;
}
