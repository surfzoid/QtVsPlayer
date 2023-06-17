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
    close();
}

void CfgDialog::on_buttonBox_rejected()
{
    SaveParam=false;
    close();
}
