#include "rtspwindow.h"
#include "ui_rtspwindow.h"
#include "infos.h"
#include <QWidgetAction>
#include <QVBoxLayout>
#include <QTime>
#include <QTimer>
#include <QWheelEvent>
#include <QAuthenticator>
#include <QUrl>
#include <QUrlQuery>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>
#include "errormanager.h"

QString RtspWindow::RtspUri;

QString RtspWindow::CamIp = "192.168.";
QString RtspWindow::CamPort = "554";
QString RtspWindow::CamUser = "admin";
QString RtspWindow::CamPass = "hik12345";
QString RtspWindow::CamPortHttp = "801";
QString RtspWindow::Chanel = "101";
qint16 RtspWindow::CamPortSdk = 8000;
unsigned int RtspWindow::PtzSpeed = 3;

PanTilCmd *RtspWindow::PTCmd;

QNetworkAccessManager *RtspWindow::manager;

/****HIKNETSDK****/
LONG RtspWindow::lPort; //Global Player port NO.
HWND RtspWindow::hWnd;
/****HIKNETSDK****/

RtspWindow::RtspWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RtspWindow)
{

    QSettings settings;//init settings before combo idx change

    PTCmd = new PanTilCmd(this);

    //    connect(player, SIGNAL(positionChanged(qint64)),
    //            this, SLOT(positionChanged(qint64)));


    //    connect(player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),
    //            this, SLOT(onPlayStatusChanged(QMediaPlayer::MediaStatus)));
    //    connect(player, SIGNAL(error(QMediaPlayer::Error)),
    //            this, SLOT(onPlayError(QMediaPlayer::Error)));
    //    connect(player, SIGNAL(stateChanged(QMediaPlayer::State)),
    //            this, SLOT(onPlayStateChanged(QMediaPlayer::State)));
    //    //connect(player, SIGNAL(bufferStatusChanged(int)),this, SLOT(onbufferStatusChanged(int)));
    //    connect(player, &QMediaPlayer::bufferStatusChanged, this, &RtspWindow::onbufferStatusChanged);
    ui->setupUi(this);


    manager = new QNetworkAccessManager(this);

    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));

    connect(manager, SIGNAL(authenticationRequired(QNetworkReply*, QAuthenticator*)),
            this, SLOT(authenticationRequired(QNetworkReply*, QAuthenticator*)));

    ui->menubar->setCornerWidget(ui->ComboBxs);
    ui->statusbar->addPermanentWidget(ui->lblLoading);

    settings.beginGroup("CamsName");
    QStringList keys = settings.allKeys();

    ui->ComboBxCam->clear();
    for (int i=0; i<keys.length(); i++){
        ui->ComboBxCam->addItem(keys[i]);
    }

    settings.endGroup();


    /*m_videoProbe = new QVideoProbe(this);
    connect(m_videoProbe, &QVideoProbe::videoFrameProbed, this, &RtspWindow::processFrame);
    m_videoProbe->setSource(player);*/

    ShowHideTimer = new QTimer(this);
    ShowHideTimer->setTimerType(Qt::PreciseTimer);
    ShowHideTimer->start( 10000 );
    RtspWindow::connect(ShowHideTimer, SIGNAL(timeout()), this, SLOT(HideMenu()));

    settings.beginGroup("RtspWindow");
    QPoint NewPos;
    QSize NewSize;
    NewPos.setX(settings.value("X", x()).value<int>());
    NewPos.setY(settings.value("Y", y()).value<int>());
    NewSize.setWidth(settings.value("Width", width()).value<int>());
    NewSize.setHeight(settings.value("Height", height()).value<int>());
    setWindowState(windowState() ^ settings.value("State", Qt::WindowNoState).value<Qt::WindowStates>());

    settings.endGroup();
    this->move(NewPos);
    this->resize(NewSize);
}

void RtspWindow::showEvent(QShowEvent *event)
{
    //    there is a ghost widget!!!!!!
    QList<QWidget *> widgets = RtspWindow::findChildren<QWidget *>();
    foreach (QWidget *var, widgets) {
        qDebug() << var->objectName() << var->hasMouseTracking() << var->size();
        if(event and var->size() == centralWidget()->size())
            var->setMouseTracking(true);
    }
    IsShown = true;
}

void RtspWindow::hideEvent(QHideEvent *event)
{
    if(event)
        PTCmd->hide();
}

RtspWindow::~RtspWindow()
{

    //---------------------------------------
    // Close preview
    NET_DVR_StopRealPlay(lRealPlayHandle);
    // Logout
    NET_DVR_Logout(lUserID);
    NET_DVR_Logout_V30(lUserID);
    // Release SDK resource
    NET_DVR_Cleanup();

    delete ui;
}



void RtspWindow::on_ComboBxCam_currentIndexChanged(const QString &arg1)
{

    ui->comboBxPresset->clear();
    settings.beginGroup(arg1);

    CamIp = settings.value("Ip", "").value<QString>();
    CamPort = settings.value("Port", "554").value<QString>();
    CamPortHttp = settings.value("PortHttp", "800").value<QString>();
    CamPortSdk = settings.value("PortSdk", "8000").value<qint16>();
    CamUser = settings.value("User", "admin").value<QString>();
    CamPass = SetFrm.crypto.decryptToString(settings.value("Password", "hik12345").value<QString>());
    settings.endGroup();

    RtspUri = "rtsp://" + CamUser + ":" + CamPass +
            "@" + CamIp + ":" + CamPort + "/Streaming/Channels/102";


    foreach( QAction *const EntryMenu , ui->menuchannel->actions())
    {
        if (EntryMenu->isChecked() == true) {
            EntryMenu->activate(QAction::Trigger);
        }
    }

    /****HIKNETSDK****/
    LoginInfo(CamPortSdk,CamIp,CamUser,CamPass,!ui->actionsub_stream->isEnabled());
    /****HIKNETSDK****/
    LoadPreset();
    LoadPatrol();
}

void RtspWindow::positionChanged(qint64 pos)
{
    //printf("%lli", pos);
    QTime time(0,0,0,0);
    time = time.addMSecs(pos);

    if(pos != 0) this->ui->statusbar-> showMessage("Elapsed time: " + time.toString());

    if (pos == 0) {
        blink();
    }else {
        ui->lblLoading->setVisible(true);//mis a true pour les tests
    }

    if (pos > 0) ui->lblLoading->setVisible(false);

}

void RtspWindow::blink()
{
    if(ui->lblLoading->isHidden())
        ui->lblLoading->show();
    else
    {
        ui->lblLoading->hide();
    }
}

void RtspWindow::mouseDoubleClickEvent(QMouseEvent *event)
{

    //surpress warning!

    if (RtspWindow::isFullScreen() && event) {
        showMaximized();
        menuBar()->setVisible(true);
        statusBar()->setVisible(true);
    } else {
        showFullScreen();
        menuBar()->setVisible(false);
        statusBar()->setVisible(false);
    }
    return;
}


void RtspWindow::on_action_Streaming_Channels_1_triggered()
{

    RtspWindow::Chanel = "101";
    ClientInfo.lChannel = 0;
    RtspUri = RtspUri.remove(RtspUri.length() - 3,3) + RtspWindow::Chanel;

    ui->action_Streaming_Channels_1->setDisabled(true);
    ui->action_Streaming_Channels_1->setChecked(true);

    ui->action_Streaming_Channels_2->setChecked(false);
    ui->action_Streaming_Channels_3->setChecked(false);

    ui->action_Streaming_Channels_2->setDisabled(false);
    ui->action_Streaming_Channels_3->setDisabled(false);

    //    PlayRtsp(RtspUri);
}

void RtspWindow::on_action_Streaming_Channels_2_triggered()
{

    RtspWindow::Chanel = "102";
    ClientInfo.lChannel = 1;
    RtspUri = RtspUri.remove(RtspUri.length() - 3,3) + RtspWindow::Chanel;

    ui->action_Streaming_Channels_2->setDisabled(true);
    ui->action_Streaming_Channels_2->setChecked(true);

    ui->action_Streaming_Channels_1->setChecked(false);
    ui->action_Streaming_Channels_3->setChecked(false);

    ui->action_Streaming_Channels_1->setDisabled(false);
    ui->action_Streaming_Channels_3->setDisabled(false);

    //PlayRtsp(RtspUri);
}

void RtspWindow::on_action_Streaming_Channels_3_triggered()
{

    RtspWindow::Chanel = "103";
    ClientInfo.lChannel = 2;
    RtspUri = RtspUri.remove(RtspUri.length() - 3,3) + RtspWindow::Chanel;

    ui->action_Streaming_Channels_3->setDisabled(true);
    ui->action_Streaming_Channels_3->setChecked(true);

    ui->action_Streaming_Channels_2->setChecked(false);
    ui->action_Streaming_Channels_1->setChecked(false);

    ui->action_Streaming_Channels_2->setDisabled(false);
    ui->action_Streaming_Channels_1->setDisabled(false);

    //PlayRtsp(RtspUri);
}

void RtspWindow::on_actionMetadata_triggered()
{
    Infos *InfDialog = new Infos(this);
    InfDialog->InfoData = "";
    QSize Vreso = centralWidget()->sizeHint();
    //    printf("Video resolution: %dX%d\n\r",Vreso.width(),Vreso.height());
    qDebug() << "Video resolution: " << Vreso.width() <<"X" << Vreso.height();
    InfDialog->InfoData.append("\nVideo resolution:  : "  + QString::number(Vreso.width()) + "x" + QString::number(Vreso.height()));

    InfDialog->show();
}

void RtspWindow::on_action_ISAPI_Streaming_channels_101_triggered()
{
    /*• rtsp://user:password@192.168.1.64:554/Streaming/Channels/channel_no01/ - Flux
principal
• rtsp://user:password@192.168.1.64:554/Streaming/Channels/channel_no02/ - Flux
auxiliaire
/doc/index.html#/preview
*/
    RtspWindow::Chanel = "104";
    struPlayInfo.dwStreamType = 3;
    QString AdPath = "/ISAPI/Streaming/Channels/" + RtspWindow::Chanel;
    QUrl Adresse("rtsp://" + CamUser + ":" + CamPass +
                 "@" +  CamIp + ":" + CamPort + AdPath);
    ui->action_Streaming_Channels_1->setChecked(false);
    ui->action_Streaming_Channels_3->setChecked(false);
    //    PlayRtsp(Adresse.url());
    //---------------------------------------
    // Close preview
    NET_DVR_StopRealPlay(lRealPlayHandle);
    // Logout
    NET_DVR_Logout(lUserID);
    NET_DVR_Logout_V30(lUserID);
    // Release SDK resource
    NET_DVR_Cleanup();
    //    Play();
}

void RtspWindow::wheelEvent(QWheelEvent *event)
{
    QPoint Scroll = event->angleDelta();

    QUrl Adresse("http://" + CamIp + ":" + CamPortHttp +
                 "/ISAPI/PTZCtrl/channels/1/Momentary");

    if (Scroll.y() > 0)
    {

        manager->put((QNetworkRequest)Adresse,
                     SetXMLReq(0,0,100).toUtf8());

    }

    if (Scroll.y() < 0 )
    {
        manager->put((QNetworkRequest)Adresse,
                     SetXMLReq(0,0,-100).toUtf8());

    }

}

void RtspWindow::replyFinished(QNetworkReply *reply)
{
    if(reply->error())
    {
        qDebug() << "ERROR!";
        printf("finish : %s\n\r", reply->errorString().toUtf8().data());
        qDebug() << reply->readAll();
        if (ui->comboBxPresset->count() == 0) {
            for (int i=1; i<16; i++){
                ui->comboBxPresset->addItem("Presset" + QString::number(i));
            }
        }
    }
    else
    {

        QString Header = reply->header(QNetworkRequest::ContentTypeHeader).toString();
        qDebug() << Header;
        qDebug() << reply->header(QNetworkRequest::LastModifiedHeader).toDateTime().toString();;
        qDebug() << reply->header(QNetworkRequest::ContentLengthHeader).toULongLong();
        qDebug() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        qDebug() << reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
        qDebug() << reply->header(QNetworkRequest::UserAgentHeader).toString();

        QByteArray Response = reply->readAll();


        if (Header.startsWith("image/jpeg") == true) {
            QString FsName = QFileDialog::getSaveFileName(this,
                                                          tr("Save as"),
                                                          QStandardPaths::writableLocation(QStandardPaths::PicturesLocation) + "/" + ui->ComboBxCam->currentText() +
                                                          QDateTime::currentDateTime().toString("-dddd-dd-MMMM-yyyy-hh.mm.ss") + ".jpg",
                                                          tr("Pictures Files (*.jpg *.jpeg)"));

            if (FsName.isEmpty()){
                return;
            }else {
                QFile file(FsName);
                file.open(QFile::WriteOnly);
                file.write(Response);
                file.close();
            }
        }else {
            QXmlStreamReader xml;
            xml.addData(Response);

            while (!xml.atEnd()) {
                xml.readNext();
                if (xml.name().contains("patrolName") == true) {
                    xml.readNext();
                    qDebug() << "patrolName" << xml.text();
                    xml.readNext();
                }
                if (xml.name().contains("presetName") == true) {
                    // do processing
                    xml.readNext();
                    //qDebug() << xml.text();
                    PresetList.append(xml.text().toUtf8());
                    xml.readNext();
                }
                if (xml.hasError()) {
                    // do error handling
                }
            }
            if (PresetList.length() > 0) {
                ui->comboBxPresset->clear();
                ui->comboBxPresset->addItems(PresetList);
                PresetList.clear();
            }
            if (ui->comboBxPresset->count() == 0) {
                for (int i=1; i<16; i++){
                    ui->comboBxPresset->addItem("Presset" + QString::number(i));
                }
            }
        }
        qDebug() << Response;
    }

    reply->deleteLater();
}

void RtspWindow::authenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator)
{

    qDebug()<< "authenticationRequired";
    if(reply->error())
    {
        qDebug()<< "ERROR!" << reply->errorString();
    }
    else
    {
        qDebug() << reply->header(QNetworkRequest::ContentTypeHeader).toString();
        qDebug() << reply->header(QNetworkRequest::LastModifiedHeader).toDateTime().toString();;
        qDebug() << reply->header(QNetworkRequest::ContentLengthHeader).toULongLong();
        qDebug() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        qDebug() << reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();

        authenticator->setUser(CamUser);
        authenticator->setPassword(CamPass);


    }

    reply->deleteLater();
}

void RtspWindow::CallPresset(int Presset)
{

    QString AdPath = ("/PTZ/channels/1/PTZControl?command=GOTO_PRESET&presetNo=" +
                      QString::number(Presset) + "&speed=" +
                      QString::number(PtzSpeed) + "&mode=start").toUtf8();
    QUrl Adresse("http://" + CamUser + ":" + CamPass +
                 "@" +  CamIp + ":" + CamPortHttp + AdPath);

    manager->get((QNetworkRequest)Adresse);
}

void RtspWindow::on_SnapshotBtn_pressed()
{
    //http://192.168.8.104/ISAPI/Streaming/channels/101/picture?videoResolutionWidth=1920&videoResolutionHeight=1080
    QString AdPath = "/ISAPI/Streaming/channels/101/picture?videoResolutionWidth=2592&videoResolutionHeight=1944";
    QUrl Adresse("http://" + CamUser + ":" + CamPass +
                 "@" +  CamIp + ":" + CamPortHttp + AdPath);

    manager->get((QNetworkRequest)Adresse);
}

void RtspWindow::on_PtzSliderSpeed_valueChanged(int value)
{
    PtzSpeed = value;
    ui->PtzSliderSpeed->setToolTipDuration(3000);
    ui->PtzSliderSpeed->setToolTip(QString::number(value));
    ui->PtzSliderSpeed->setStatusTip(QString::number(value));
}



void RtspWindow::on_actionPreset_triggered()
{
    settings.beginGroup(ui->ComboBxCam->currentText() + "_Preset");
    for (int i=0; i<ui->comboBxPresset->count() - 1; i++){
        settings.setValue( "Presset" + QString::number(i),
                           ui->comboBxPresset->itemText(i));
    }

    settings.endGroup();
    settings.sync();
}

void RtspWindow::on_actionPatrol_triggered()
{
    settings.beginGroup(ui->ComboBxCam->currentText() + "_Patrol");

    for (int i=0; i<9; i++){
        settings.setValue( "Patrol" + QString::number(i),
                           ui->comboBxPatrol->itemText(i));
    }

    settings.endGroup();
    settings.sync();

}

void RtspWindow::on_comboBxPresset_editTextChanged(const QString &arg1)
{
    ui->comboBxPresset->setItemText(ui->comboBxPresset->currentIndex(),arg1);
}

void RtspWindow::on_comboBxPatrol_editTextChanged(const QString &arg1)
{
    ui->comboBxPatrol->setItemText(ui->comboBxPatrol->currentIndex(),arg1);
}

void RtspWindow::LoadPreset()
{
    QString AdPath = ("/ISAPI/PTZCtrl/channels/1/presets");
    QUrl Adresse("http://" + CamUser + ":" + CamPass +
                 "@" +  CamIp + ":" + CamPortHttp + AdPath);

    manager->get((QNetworkRequest)Adresse);
    return;

    ui->comboBxPresset->clear();

    settings.beginGroup(ui->ComboBxCam->currentText() + "_Preset");
    QStringList keys = settings.allKeys();


    for (int i=0; i<keys.length(); i++){
        ui->comboBxPresset->addItem(settings.value("Presset" +
                                                   QString::number(i), "Presset" + QString::number(i)).value<QString>());
    }

    settings.endGroup();

    if (ui->comboBxPresset->count() == 0) {
        for (int i=1; i<16; i++){
            ui->comboBxPresset->addItem("Presset" + QString::number(i));
        }
    }


}

void RtspWindow::LoadPatrol()
{
    QString AdPath = ("/ISAPI/PTZCtrl/channels/1/patrols");
    QUrl Adresse("http://" + CamUser + ":" + CamPass +
                 "@" +  CamIp + ":" + CamPortHttp + AdPath);

    manager->get((QNetworkRequest)Adresse);
    //return;
    ui->comboBxPatrol->clear();

    settings.beginGroup(ui->ComboBxCam->currentText() + "_Patrol");
    QStringList keys = settings.allKeys();


    for (int i=0; i<keys.length(); i++){
        ui->comboBxPatrol->addItem(settings.value("Patrol" +
                                                  QString::number(i), "Patrol" + QString::number(i)).value<QString>());
    }

    settings.endGroup();

    if (ui->comboBxPatrol->count() == 0) {
        for (int i=1; i<10; i++){
            ui->comboBxPatrol->addItem("Patrol" + QString::number(i));
        }
    }

}

void RtspWindow::GetMetaData()
{

}



void RtspWindow::on_comboBxPresset_textActivated(const QString &arg1)
{
    CallPresset(ui->comboBxPresset->currentIndex() + 1);
    ui->comboBxPresset->setToolTip(arg1);
}

void RtspWindow::on_comboBxPatrol_textActivated(const QString &arg1)
{
    QString AdPath = ("/ISAPI/PTZCtrl/channels/1/patrols/" +
                      QString::number(ui->comboBxPatrol->currentIndex() + 1) + "/start").toUtf8();
    QUrl Adresse("http://" + CamUser + ":" + CamPass +
                 "@" + CamIp + ":" + CamPortHttp + AdPath);

    QIODevice * outgoingData = 0;
    manager->put((QNetworkRequest)Adresse,outgoingData);
}

void RtspWindow::on_actionReboot_triggered()
{
    QString AdPath = "/ISAPI/System/reboot";
    QUrl Adresse("http://" + CamUser + ":" + CamPass +
                 "@" + CamIp + ":" + CamPortHttp + AdPath);

    QIODevice * outgoingData = 0;
    manager->put((QNetworkRequest)Adresse,outgoingData);
}


void RtspWindow::on_RecordBtn_toggled(bool checked)
{

    if (checked) {

        QString AdPath = "/ISAPI/ContentMgmt/record/control/manual/start/tracks/101"; // + RtspWindow::Chanel;
        QUrl Adresse("http://" + CamUser + ":" + CamPass +
                     "@" + CamIp + ":" + CamPortHttp + AdPath);

        QIODevice * outgoingData = 0;
        manager->put((QNetworkRequest)Adresse,outgoingData);
        this->ui->RecordBtn->setText(tr("Stop"));
        this->ui->RecordBtn->setStyleSheet("background-color: red;");

    } else
    {

        QString AdPath = "/ISAPI/ContentMgmt/record/control/manual/stop/tracks/101"; // + RtspWindow::Chanel;
        QUrl Adresse("http://" + CamUser + ":" + CamPass +
                     "@" + CamIp + ":" + CamPortHttp + AdPath);

        QIODevice * outgoingData = 0;
        manager->put((QNetworkRequest)Adresse,outgoingData);
        this->ui->RecordBtn->setText(tr("Record"));
        this->ui->RecordBtn->setStyleSheet("background-color: green;");
    }

}


void RtspWindow::setStatusInfo(const QString &info)
{
    ui->lblLoading->show();
    ui->lblLoading->setText(info.toUtf8().data());
}

void RtspWindow::displayErrorMessage(QString Err)
{
    ui->lblLoading->show();
    ui->lblLoading->setText(Err);
}

void RtspWindow::keyPressEvent(QKeyEvent *e)
{
    if ( ui->comboBxPresset->hasFocus() && e->key() == Qt::Key_Return) {
        int IdP = ui->comboBxPresset->currentIndex() + 1;
        QString PresetName = ui->comboBxPresset->currentText();
        QString XmlPut = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><PTZPreset xmlns=\"http://www.isapi.org/ver20/XMLSchema\" version=\"2.0\"><id>" + QString::number(IdP) + "</id><presetName>" + PresetName + "</presetName></PTZPreset>";
        QUrl Adresse("http://" + CamIp + ":" + CamPortHttp +
                     "/ISAPI/PTZCtrl/channels/1/presets/" + QString::number(IdP));

        manager->put((QNetworkRequest)Adresse,
                     XmlPut.toUtf8());
        Sleep(1500);
        LoadPreset();
        Sleep(1000);
        ui->comboBxPresset->setCurrentIndex(IdP -1);

    }
}

void RtspWindow::Sleep(int MSecs)
{
    QTime dieTime= QTime::currentTime().addMSecs(MSecs);
    while (QTime::currentTime() < dieTime)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}

void RtspWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (event) {
        if(!RtspWindow::isFullScreen())
        {
            ui->menubar->show();
            ui->statusbar->show();
        }
        PTCmd->setVisible(true);
        PTCmd->setFocus();
    }
    return;
}

void RtspWindow::HideMenu()
{
    if (!MenubarHasFocus(ui->menubar)) {
        ui->menubar->hide();
        ui->statusbar->hide();
        PTCmd->setVisible(false);
    }
}

bool RtspWindow::MenubarHasFocus(QMenuBar *menu)
{
    if (menu->underMouse() || ui->menubar->activeAction() || PTCmd->underMouse())
        return true;
    foreach (QWidget *Widg,  menu->findChildren<QWidget *>()) {
        if (Widg->underMouse())
            return true;
    }
    return false;
}

PLAYRECT Adjust;
void RtspWindow::resizeEvent(QResizeEvent *event)
{
    //surpress warning!
    if (event == nullptr) {
        event = nullptr;
    }
    //surpress warning!

    QSettings settings;
    settings.beginGroup("RtspWindow");

    settings.setValue( "X", this->pos().x());
    settings.setValue("Y", this->pos().y());
    settings.setValue("Width", this->width());
    settings.setValue("Height", this->height());
    settings.setValue("State", GetWinState());

    settings.endGroup();
    settings.sync();

    /****HikNetSdk****/
    Adjust.uHeight = centralWidget()->height();
    Adjust.uWidth  = centralWidget()->width();
    Adjust.x = centralWidget()->x();
    Adjust.y  = centralWidget()->y();


    hWnd = 0;

    hWnd = centralWidget()->winId();

#if (defined(__linux__))
    if (!PlayM4_WndResolutionChange(lPort))
    {
        qDebug()<< "PlayM4_WndResolutionChange error " << PlayM4_GetLastError(lPort);
    }
#endif
    //    return;

    //    //struPlayInfo.hPlayWnd = videoWidget->winId();// centralWidget()->winId();
    //    if (!NET_DVR_RealPlayRestart(lRealPlayHandle,centralWidget()->winId())) {
    //        int err = NET_DVR_GetLastError();
    //        qDebug()<< NET_DVR_GetErrorMsg(&err);
    //    }


    //    if (!PlayM4_WndResolutionChange(lRealPlayHandle))
    //    {
    //        qDebug()<< PlayM4_GetLastError(lRealPlayHandle);
    //    }
    //    if (!PlayM4_RefreshPlay(lRealPlayHandle))
    //    {
    //        qDebug()<< PlayM4_GetLastError(lRealPlayHandle);
    //    }
    /****HikNetSdk****/
    return ;
}

QString RtspWindow::GetWinState()
{
    switch (this->windowState()) {

    default:
    case Qt::WindowNoState://0x00000000 The window has no state set (in normal state).
        return "0x00000000";
        break;
    case Qt::WindowMinimized://0x00000001 The window is minimized (i.e. iconified).
        return "0x00000001";
        break;
    case Qt::WindowMaximized://0x00000002 The window is maximized with a frame around it.
        return "0x00000002";
        break;
    case Qt::WindowFullScreen://0x00000004 The window fills the entire screen without any frame around it.
        return "0x00000004";
        break;
    case Qt::WindowActive://0x00000008 The window is the active window, i.e. it has keyboard focus.
        return "0x00000008";
        break;

    }
}

void RtspWindow::PanTilTopLeft_pressed()
{
    RtspWindow::SendPTZ(-100,100,0);
}

void RtspWindow::PanTilUp_pressed()
{
    RtspWindow::SendPTZ(0,100,0);
}

void RtspWindow::PanTilTopRight_pressed()
{
    RtspWindow::SendPTZ(100,100,0);
}

void RtspWindow::PanTilRight_pressed()
{
    RtspWindow::SendPTZ(100,0,0);
}

void RtspWindow::PanTilBottomRight_pressed()
{
    RtspWindow::SendPTZ(100,-100,0);
}

void RtspWindow::PanTilDown_pressed()
{
    RtspWindow::SendPTZ(0,-100,0);
}

void RtspWindow::PanTilBottomLeft_pressed()
{
    RtspWindow::SendPTZ(-100,-100,0);
}

void RtspWindow::PanTilLeft_pressed()
{
    RtspWindow::SendPTZ(-100,0,0);
}

void RtspWindow::SendPTZ(int pan, int tilt, int zoom)
{
    QUrl PanTilAdresse("http://" + CamUser + ":" + CamPass +
                       "@" + CamIp + ":" + CamPortHttp +
                       "/ISAPI/PTZCtrl/channels/1/Momentary");
    manager->put((QNetworkRequest)PanTilAdresse,
                 SetXMLReq(pan,tilt,zoom).toUtf8());

}

/**************************************************
    url='PTZCtrl/channels/1/momentary'
    xml='<PTZData”>
    <pan>0</pan>              # Rotation de -100 à 100 (gauche ou droite)
    <tilt>0</tilt>            # Inclinaison de 100 à -100 (haut ou bas)
    <zoom>0</zoom>            # Zoom -100 à 100 (zoom+ ou zoom-)
    <Momentary>
    <duration>1000</duration> # Durée du mouvement en millisecondes
    </Momentary>
    </PTZData>'
***************************************************/
QString RtspWindow::SetXMLReq(int pan,int tilt,int zoom)
{
    int Duration = PtzSpeed * 100;

    QString XmlData = "<PTZData>\r\n\
<pan>" + QString::number(pan) + "</pan>\r\n\
            <tilt>" + QString::number(tilt) + "</tilt>\r\n\
            <zoom>" + QString::number(zoom) + "</zoom>\r\n\
            <Momentary>\r\n\
            <duration>" +
                      QString::number(Duration) + "</duration>\r\n\
            </Momentary>\r\n\
            </PTZData>";

        return XmlData;
}

/****HIKNETSDK****/

void RtspWindow::LoginInfo(qint16 Port,QString sDeviceAddress,QString sUserName, QString sPassword,bool StreamChoice)
{

    // Close preview
    NET_DVR_StopRealPlay(lRealPlayHandle);
    // Logout
    NET_DVR_Logout(lUserID);
    NET_DVR_Logout_V30(lUserID);
    // Release SDK resource
    NET_DVR_Cleanup();

    hWnd = centralWidget()->winId();
    //qDebug() << sDeviceAddress << Port << sUserName << sPassword;
    //AT last need init SDK
    if (!NET_DVR_Init())
    {
        int err = NET_DVR_GetLastError();
        qDebug()<< NET_DVR_GetErrorMsg(&err);
        return;
    }
    //---------------------------------------
    //Set connection time and reconnection time
    NET_DVR_SetConnectTime(2000, 1);
    NET_DVR_SetReconnect(10000, true);
    //---------------------------------------
    // Login
    NET_DVR_DEVICEINFO_V30 struDeviceInfo;
    lUserID = NET_DVR_Login_V30(sDeviceAddress.toUtf8().data(), Port, sUserName.toUtf8().data(), sPassword.toUtf8().data(), &struDeviceInfo);
    if (lUserID < 0)
    {
        printf("Login error, %d\n", NET_DVR_GetLastError());
        NET_DVR_Cleanup();
        return;
    }
    //---------------------------------------
    //Set exception callback function
    NET_DVR_SetExceptionCallBack_V30(0, NULL,g_ExceptionCallBack, NULL);
    //---------------------------------------
    //Start preview and set to callback stream data
    NET_DVR_CLIENTINFO ClientInfo = {0};
    ClientInfo.hPlayWnd = NULL;
    //If need to decode, please set it valid. If want to get stream data only, we can set to NULL
    ClientInfo.lChannel = 1; //Preview channel NO.cam=1,nvr=33
    if (StreamChoice) {
        ClientInfo.lLinkMode |= (1u << 31);
    }else {
        ClientInfo.lLinkMode = 0;
    }/*If 31st bit is 0, it means connect main stream, is 1 means sub stream.
    Bit 0~bit 30 are used for link mode: 0- TCP mode, 1- UDP mode, 2- Multi-play mode, 3- RTP mode, 4- RTP over
    RTSP, 5- RTP over HTTP */
    ClientInfo.sMultiCastIP = NULL;
    //Multicast IP. Please set when require to preview in multicast mode.
    BOOL bPreviewBlock = false;
    //whether blocked when requiring a stream connection, 0 means unblocked, 1 means blocked
    lRealPlayHandle = NET_DVR_RealPlay_V30(lUserID, &ClientInfo, g_RealDataCallBack_V30, NULL, 0);
    if (lRealPlayHandle < 0)
    {
        printf("NET_DVR_RealPlay_V30 error\n");
        int err = NET_DVR_GetLastError();
        qDebug()<< NET_DVR_GetErrorMsg(&err);
        NET_DVR_Logout(lUserID);
        NET_DVR_Cleanup();
        return;
    }
    return;
}

//void RtspWindow::Play()
//{

//    struPlayInfo.lChannel     = StreamType;  //channel NO
//    struPlayInfo.dwLinkMode   = 0;
//    struPlayInfo.dwStreamType   = StreamType;// Stream type 0-main stream,1-sub stream,2-third stream,3-forth
//    struPlayInfo.byRecvMetaData = 1;
//    struPlayInfo.hPlayWnd = videoWidget->winId();// centralWidget()->winId();
//    struPlayInfo.bBlocked = 1;
//    struPlayInfo.dwDisplayBufNum = 1;
//    lRealPlayHandle = NET_DVR_RealPlay_V40(0, &struPlayInfo, RealDataCallBack, NULL);
//    qDebug() << "realhandle" << lRealPlayHandle;
//    if (!PlayM4_SetDecCallBack(0,DecCBFun))
//    {
//        qDebug() << PlayM4_GetLastError(0);
//    }
//    if (!PlayM4_RenderPrivateData(0, PLAYM4_RENDER_ANA_INTEL_DATA, true))
//    {
//        qDebug() << PlayM4_GetLastError(0);
//    }

//}


/**  @fn  void __stdcall  RealDataCallBack(LONG lRealHandle,int dwDataType,BYTE *pBuffer,int  dwBufSize, void* dwUser)
 *   @brief data callback funtion
 *   @param (OUT) LONG lRealHandle
 *   @param (OUT) int dwDataType
 *   @param (OUT) BYTE *pBuffer
 *   @param (OUT) int  dwBufSize
 *   @param (OUT) void* dwUser
 *   @return none
 */
void __stdcall  RtspWindow::RealDataCallBack(LONG lRealHandle,int dwDataType,BYTE *pBuffer,int  dwBufSize, void* dwUser)
{
    //qDebug() << "pBuffer" << pBuffer;
    if (dwUser != NULL)
    {
        qDebug() << "init error..." << NET_DVR_GetLastError();
        qDebug("Demmo lRealHandle[%d]: Get StreamData! Type[%d], BufSize[%d], pBuffer:%p\n", lRealHandle, dwDataType, dwBufSize, pBuffer);
        int err = NET_DVR_GetLastError();
        qDebug()<< NET_DVR_GetErrorMsg(&err);
    }
}

void __stdcall RtspWindow::g_ExceptionCallBack(int dwType, LONG lUserID, LONG lHandle, void *pUser)
{
    switch(dwType)
    {
    case EXCEPTION_RECONNECT:
        qDebug() << "init error..." << NET_DVR_GetLastError();
        qDebug() <<"reconnect--------" << QDateTime::currentDateTime().toString();
        break;
    default:
        int err = NET_DVR_GetLastError();
        qDebug()<< NET_DVR_GetErrorMsg(&err);
        break;
    }
}

void __stdcall RtspWindow::DecCBFun(int nPort,char * pBuf,int nSize,FRAME_INFO * pFrameInfo, void* nReserved1,int nReserved2)
{
    //    qDebug("TYPE:%d-[%d*%d]",pFrameInfo->nType,pFrameInfo->nWidth,pFrameInfo->nHeight);
    switch (pFrameInfo->nType) {
    case T_YV12:
    {

    }
        break;
    case T_AUDIO8:
    case T_AUDIO16:

        break;
    default:
        break;
    }
}

/*Mode 2 Users theirselves deal with stream data which called back by g_RealDataCallBack_V30.
Here takes software decoding as an example.*/
void CALLBACK RtspWindow::g_RealDataCallBack_V30(LONG lRealHandle, int dwDataType, BYTE *pBuffer, int dwBufSize, void* dwUser)
{
    //    QString str;
    //    QByteArray TmpB((char*)pBuffer);
    //    qDebug() << "dwDataType" << dwDataType;
    switch (dwDataType)
    {
    case 1: //NET_DVR_SYSHEAD System head
        //        str = QString::fromUtf8((char*)pBuffer);
        //        qDebug() << str << dwBufSize;
        if (!PlayM4_GetPort(&lPort))//Get unused port
        {
            break;
        }
        //m_iPort = lPort;
        /*The data called back at the first time is system header. Please
assign this port to global port, and it will be used to play in next callback */
        if (dwBufSize > 0)
        {
            if (!PlayM4_SetStreamOpenMode(lPort, STREAME_REALTIME))
                //Set real-time stream playing mode
            {
                break;
            }
            if (!PlayM4_OpenStream(lPort, pBuffer, dwBufSize, 1024*1024))
                //Open stream
            {
                break;
            }
            if (!PlayM4_Play(lPort, hWnd))//Start play
            {
                break;
            }
            /*if (!PlayM4_SetDecCallBack(lPort,DecCBFun))
            {
                qDebug() << PlayM4_GetLastError(lPort);
            }*/
            if (!PlayM4_RenderPrivateData(lPort, PLAYM4_RENDER_ANA_INTEL_DATA, true))
            {
                qDebug() << PlayM4_GetLastError(0);
            }
        }
        break;
    case 2://NET_DVR_STREAMDATA Stream data
        if (dwBufSize > 0 && lPort != -1)
        {
            if (!PlayM4_InputData(lPort, pBuffer, dwBufSize))
            {
                break;
            }
            //            str = QString::fromUtf8((char*)pBuffer);
            //            qDebug() << str << dwBufSize;
        }
    }
}
/****HIKNETSDK****/

void RtspWindow::on_actionmain_stream_triggered()
{
    if (IsShown) {
        ui->actionsub_stream->setEnabled(true);
        ui->actionmain_stream->setEnabled(false);
        LoginInfo(CamPortSdk,CamIp,CamUser,CamPass,false);
    }
}

void RtspWindow::on_actionsub_stream_triggered()
{
    if (IsShown) {
        ui->actionmain_stream->setEnabled(true);
        ui->actionsub_stream->setEnabled(false);
        LoginInfo(CamPortSdk,CamIp,CamUser,CamPass,true);
    }
}
