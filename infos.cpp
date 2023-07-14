#include "infos.h"
#include "ui_infos.h"
#include <QLabel>
#include <QLineEdit>

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
    //    ui->label->setText(InfoData);
    QStringList Infos = InfoData.split("\n");
    Infos.removeAt(0);

    foreach (QString InfosData, Infos) {
        QStringList Data = InfosData.split(": ");
        QLabel *Name = new  (QLabel);
        Name->setText(Data.at(0));
//        Name->setParent(ui->formLayout);
        QLineEdit *Value =  new  (QLineEdit);
        Value->setText(Data.at(1));
        Value->setReadOnly(false);
        Value->setDragEnabled(true);
        Value->setEnabled(true);
        Value->setMouseTracking(true);
        Value->setAcceptDrops(true);
        Value->setDisabled(false);
//        Value->setParent(ui->formLayoutWidget);
        ui->formLayout->addRow(Name,Value);
        Name->setVisible(true);
        Value->setVisible(true);
    }
    ui->formLayout->setEnabled(true);
    resize(ui->formLayout->sizeHint());
}

void Infos::resizeEvent(QResizeEvent *event)
{

}

Infos::~Infos()
{
    delete ui;
}
