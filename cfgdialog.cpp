#include "cfgdialog.h"
#include "ui_cfgdialog.h"

CfgDialog::CfgDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CfgDialog)
{
    ui->setupUi(this);
}

void CfgDialog::showEvent(QShowEvent *event)
{
    this->ui->EdChannelName->setText(ChannelName);
    this->ui->ChkBxShowChannel->setChecked(ShowChannel);
    ui->spinBoxNameX->setValue(ChannelX);
    ui->spinBoxNameY->setValue(ChannelY);
    ui->spinBoxOSDX->setValue(wOSDTopLeftX);
    ui->spinBoxOSDY->setValue(wOSDTopLeftY);
}

CfgDialog::~CfgDialog()
{
    delete ui;
}

void CfgDialog::on_buttonBox_accepted()
{
    SaveParam=true;
    ChannelName = ui->EdChannelName->text();
    ShowChannel = ui->ChkBxShowChannel->isChecked();
    ChannelX = ui->spinBoxNameX->value();
    ChannelY = ui->spinBoxNameY->value();
    wOSDTopLeftX = ui->spinBoxOSDX->value();
    wOSDTopLeftY = ui->spinBoxOSDY->value();
    close();
}

void CfgDialog::on_buttonBox_rejected()
{
    SaveParam=false;
    close();
}
