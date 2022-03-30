#include "settingsform.h"
#include "ui_settingsform.h"
#include "include/PlayM4.h"
#include "playm4interface.h"
#include "errormanager.h"
#include "qtvsplayer.h"

#include <QSettings>



SettingsForm::SettingsForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsForm)
{
    ui->setupUi(this);

    QSettings settings;
    settings.beginGroup("CamsName");
    QStringList keys = settings.allKeys();

    ui->CamNameEd->clear();
    for (int i=0; i<keys.length(); i++){
        ui->CamNameEd->addItem(keys[i]);
    }

    settings.endGroup();

    ui->ConfFsPath->setText(settings.fileName());
}

SettingsForm::~SettingsForm()
{
    delete ui;
}

void SettingsForm::on_BtnSave_released()
{
    QSettings settings;
    CamName = ui->CamNameEd->currentText();
    settings.setValue( "CamsName/" + CamName,"Name");
    settings.endGroup();
    settings.sync();

    settings.beginGroup(CamName);
    // settings.group() == "alpha/beta"

    settings.setValue( "Ip", ui->IpEd->text());
    settings.setValue("Port", ui->PortEd->text());
    settings.setValue("User", ui->UserEd->text());
    settings.setValue("Password", ui->PassEd->text());

    settings.endGroup();
    settings.sync();
    close();
}

void SettingsForm::on_CamNameEd_currentIndexChanged(const QString &arg1)
{

    QSettings settings;

    CamName = arg1;
    settings.beginGroup(CamName);

    CamIp = settings.value("Ip", "").value<QString>();
    CamPort = settings.value("Port", "554").value<QString>();
    CamUser = settings.value("User", "admin").value<QString>();
    CamPass = settings.value("Password", "hik12345").value<QString>();
    settings.endGroup();

    ui->IpEd->setText(CamIp);
    ui->PortEd->setText(CamPort);
    ui->UserEd->setText(CamUser);
    ui->PassEd->setText(CamPass);

}

void SettingsForm::on_BtnDel_released()
{
    QSettings settings;
    settings.remove(CamName);
    settings.remove("CamsName/" + CamName);
    settings.sync();
    ui->CamNameEd->removeItem(ui->CamNameEd->currentIndex());
}

void SettingsForm::on_BtPlay_released()
{
    /*playm4interface::SetPort();
    PLAYM4_SESSION_INFO SessionInf = *new PLAYM4_SESSION_INFO();
    SessionInf.nSessionInfoType = PLAYM4_SESSION_INFO_SDP;
    SessionInf.pSessionInfoData = (unsigned char*)"admin:hik12345@192.168.27.67";
    SessionInf.nSessionInfoLen = 2 + strlen((char*)SessionInf.pSessionInfoData);


    int bsucces = __stdcall PlayM4_OpenStreamAdvanced(playm4interface::m_pblocalportnum, PLAYM4_PROTOCOL_RTSP, &SessionInf, 2048);
    printf("pyd---PlayM4_OpenStreamAdvanced :%d\n\r",bsucces);
    ErrorManager::error_codes("PlayM4_OpenStreamAdvanced",PlayM4_GetLastError(playm4interface::m_pblocalportnum));*/

}
