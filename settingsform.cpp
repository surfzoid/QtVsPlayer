#include "settingsform.h"
#include "ui_settingsform.h"
#include "include/PlayM4.h"
#include "playm4interface.h"
#include "errormanager.h"
#include "qtvsplayer.h"


SimpleCrypt SettingsForm::crypto;
SettingsForm::SettingsForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsForm)
{
    ui->setupUi(this);
    QSettings settings;


crypto.setKey(Q_UINT64_C(0x0c2ad4a4acb9f023 * 3));//some random number
    settings.beginGroup("CamsName");
    QStringList keys = settings.allKeys();

    ui->CamNameEd->clear();
    for (int i=0; i<keys.length(); i++){
        ui->CamNameEd->addItem(keys.at(i));
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

    CamName = ui->CamNameEd->currentText();
    settings.setValue( "CamsName/" + CamName,"Name");
    //settings.endGroup();
    settings.sync();

    settings.beginGroup(CamName);
    // settings.group() == "alpha/beta"

    settings.setValue( "Ip", ui->IpEd->text());
    settings.setValue("Port", ui->PortEd->text());
    settings.setValue("PortHttp", ui->PortHttpEd->text());
    /*******HCNetSDK**************/
    settings.setValue("PortSdk", ui->PortSdkEd->text());
    /*******HCNetSDK**************/
    settings.setValue("User", ui->UserEd->text());
    settings.setValue("Password",crypto.encryptToString(ui->PassEd->text()) );
    settings.setValue("ShowXML", ui->ChkBxShowXml->checkState());

    settings.endGroup();
    settings.sync();
    close();

    CamIp = ui->IpEd->text();
    CamPort = ui->PortEd->text();
    CamPortHttp = ui->PortHttpEd->text();
    /*******HCNetSDK**************/
    CamPortSdk = ui->PortSdkEd->text().toShort();
    /*******HCNetSDK**************/
    CamUser = ui->UserEd->text();
    CamPass = ui->PassEd->text();
    ShowXML = ui->ChkBxShowXml->checkState();
}

void SettingsForm::on_CamNameEd_currentIndexChanged(const QString &arg1)
{
    CamName = arg1;
    settings.beginGroup(CamName);

    CamIp = settings.value("Ip", "").value<QString>();
    CamPort = settings.value("Port", "554").value<QString>();
    CamPortHttp = settings.value("PortHttp", "800").value<QString>();
    /*******HCNetSDK**************/
    CamPortSdk = settings.value("PortSdk", "8000").value<qint16>();
    /*******HCNetSDK**************/
    CamUser = settings.value("User", "admin").value<QString>();
    CamPass = crypto.decryptToString(settings.value("Password", "hik12345").value<QString>());
    ShowXML = settings.value("ShowXML", "0").value<bool>();
    settings.endGroup();

    ui->IpEd->setText(CamIp);
    ui->PortEd->setText(CamPort);
    ui->PortHttpEd->setText(CamPortHttp);
    /*******HCNetSDK**************/
    ui->PortSdkEd->setText(QString::number(CamPortSdk));
    /*******HCNetSDK**************/
    ui->UserEd->setText(CamUser);
    ui->PassEd->setText(CamPass);
    ui->ChkBxShowXml->setChecked(ShowXML);

}

void SettingsForm::on_BtnDel_released()
{

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
    printf("---PlayM4_OpenStreamAdvanced :%d\r\n",bsucces);
    ErrorManager::error_codes("PlayM4_OpenStreamAdvanced",PlayM4_GetLastError(playm4interface::m_pblocalportnum));*/

}

void SettingsForm::on_BtnCancel_released()
{
    close();
}
