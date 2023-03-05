#include "rtspwindow.h"
#include "ui_rtspwindow.h"
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
#include <QMediaMetaData>
#include "errormanager.h"

QMediaPlayer *RtspWindow::player;
QVideoWidget *RtspWindow::videoWidget;
QString RtspWindow::RtspUri;

QString RtspWindow::CamIp = "192.168.";
QString RtspWindow::CamPort = "554";
QString RtspWindow::CamUser = "admin";
QString RtspWindow::CamPass = "hik12345";
QString RtspWindow::CamPortHttp = "801";
QString RtspWindow::Chanel = "101";
unsigned int RtspWindow::PtzSpeed = 3;

PanTilCmd *RtspWindow::PTCmd;

QNetworkAccessManager *RtspWindow::manager;

RtspWindow::RtspWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RtspWindow)
{

    QSettings settings;//init settings before combo idx change
    RtspWindow::player = new QMediaPlayer(this, QMediaPlayer::StreamPlayback);
    RtspWindow::videoWidget = new QVideoWidget(this);

    PTCmd = new PanTilCmd(this);

    connect(player, SIGNAL(positionChanged(qint64)),
            this, SLOT(positionChanged(qint64)));


    connect(player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),
            this, SLOT(onPlayStatusChanged(QMediaPlayer::MediaStatus)));
    connect(player, SIGNAL(error(QMediaPlayer::Error)),
            this, SLOT(onPlayError(QMediaPlayer::Error)));
    connect(player, SIGNAL(stateChanged(QMediaPlayer::State)),
            this, SLOT(onPlayStateChanged(QMediaPlayer::State)));
    //connect(player, SIGNAL(bufferStatusChanged(int)),this, SLOT(onbufferStatusChanged(int)));
    connect(player, &QMediaPlayer::bufferStatusChanged, this, &RtspWindow::onbufferStatusChanged);
    ui->setupUi(this);

    setCentralWidget(videoWidget);

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
    videoWidget->setObjectName("videoWidget");
    //there is a ghost widget!!!!!!
    QList<QWidget *> widgets = RtspWindow::findChildren<QWidget *>();
    foreach (QWidget *var, widgets) {
        //qDebug() << var->objectName() << var->hasMouseTracking() << var->size();
        if(event and var->size() == videoWidget->size())
            var->setMouseTracking(true);
    }
}

void RtspWindow::hideEvent(QHideEvent *event)
{
    if(event)
        PTCmd->hide();
}

RtspWindow::~RtspWindow()
{

    if (videoWidget != NULL) {
        videoWidget->~QVideoWidget();
    }

    if (player != NULL) {
        player->~QMediaPlayer();
        //delete  player;
    }
    delete ui;
}


void RtspWindow::PlayRtsp(QString Camuri)
{

    if (player->isVideoAvailable() == true) {
        player->stop();
    }

    player->setVolume(50);

    player->setVideoOutput(videoWidget);
    videoWidget->setObjectName("videoWidget");
    videoWidget->setMouseTracking(true);
    videoWidget->setAutoFillBackground(true);
    videoWidget->setAspectRatioMode(Qt::KeepAspectRatio);
    videoWidget->setSizePolicy(QSizePolicy::Preferred,
                               QSizePolicy::Maximum);

    player->setMedia(QUrl(Camuri));
    //player->setPlaybackRate(0);
    player->play();
    videoWidget->setAspectRatioMode(Qt::KeepAspectRatioByExpanding);
    //videoWidget->hide();




}

void RtspWindow::on_ComboBxCam_currentIndexChanged(const QString &arg1)
{

    ui->comboBxPresset->clear();
    settings.beginGroup(arg1);

    CamIp = settings.value("Ip", "").value<QString>();
    CamPort = settings.value("Port", "554").value<QString>();
    CamPortHttp = settings.value("PortHttp", "800").value<QString>();
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

void RtspWindow::on_actionIgnoreAspectRatio_triggered()
{
    videoWidget->setAspectRatioMode(Qt::IgnoreAspectRatio);
}

void RtspWindow::on_actionDefault_triggered()
{
    videoWidget->setAspectRatioMode( (Qt::AspectRatioMode) -1);
}

void RtspWindow::on_actionKeepAspectRatio_triggered()
{
    videoWidget->setAspectRatioMode(Qt::KeepAspectRatio);
}

void RtspWindow::on_actionKeepAspectRatioByExpanding_triggered()
{
    videoWidget->setAspectRatioMode(Qt::KeepAspectRatioByExpanding);
}

void RtspWindow::on_action_Streaming_Channels_1_triggered()
{

    RtspWindow::Chanel = "101";
    RtspUri = RtspUri.remove(RtspUri.length() - 3,3) + RtspWindow::Chanel;

    ui->action_Streaming_Channels_1->setDisabled(true);
    ui->action_Streaming_Channels_1->setChecked(true);

    ui->action_Streaming_Channels_2->setChecked(false);
    ui->action_Streaming_Channels_3->setChecked(false);

    ui->action_Streaming_Channels_2->setDisabled(false);
    ui->action_Streaming_Channels_3->setDisabled(false);

    PlayRtsp(RtspUri);
}

void RtspWindow::on_action_Streaming_Channels_2_triggered()
{

    RtspWindow::Chanel = "102";
    RtspUri = RtspUri.remove(RtspUri.length() - 3,3) + RtspWindow::Chanel;

    ui->action_Streaming_Channels_2->setDisabled(true);
    ui->action_Streaming_Channels_2->setChecked(true);

    ui->action_Streaming_Channels_1->setChecked(false);
    ui->action_Streaming_Channels_3->setChecked(false);

    ui->action_Streaming_Channels_1->setDisabled(false);
    ui->action_Streaming_Channels_3->setDisabled(false);

    PlayRtsp(RtspUri);
}

void RtspWindow::on_action_Streaming_Channels_3_triggered()
{

    RtspWindow::Chanel = "103";
    RtspUri = RtspUri.remove(RtspUri.length() - 3,3) + RtspWindow::Chanel;

    ui->action_Streaming_Channels_3->setDisabled(true);
    ui->action_Streaming_Channels_3->setChecked(true);

    ui->action_Streaming_Channels_2->setChecked(false);
    ui->action_Streaming_Channels_1->setChecked(false);

    ui->action_Streaming_Channels_2->setDisabled(false);
    ui->action_Streaming_Channels_1->setDisabled(false);

    PlayRtsp(RtspUri);
}

void RtspWindow::on_actionMetadata_triggered()
{
    GetMetaData(player);
    QSize Vreso = videoWidget->sizeHint();
    printf("Video resolution: %dX%d\n\r",Vreso.width(),Vreso.height());
    qDebug() << "Video resolution: " << Vreso.width() <<"X" << Vreso.height();
    return;

    bool IsMeta ;
    IsMeta = player->isMetaDataAvailable();

    if (IsMeta == true) {
        QStringList nMeta = player->availableMetaData();
        foreach (QString nData, nMeta)
        {
            printf("%s\n\r",nData.toUtf8().data());
            QVariant KeyVal = player->metaData(nData);
            printf("%s\n\r",KeyVal.toString().toUtf8().data());
        }


    }
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
    QString AdPath = "/ISAPI/Streaming/Channels/" + RtspWindow::Chanel;
    QUrl Adresse("rtsp://" + CamUser + ":" + CamPass +
                 "@" +  CamIp + ":" + CamPort + AdPath);
    ui->action_Streaming_Channels_1->setChecked(false);
    ui->action_Streaming_Channels_3->setChecked(false);
    PlayRtsp(Adresse.url());
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

void RtspWindow::GetMetaData(QMediaPlayer *player)
{
    // Get the list of keys there is metadata available for
    QStringList metadatalist = player->availableMetaData();

    // Get the size of the list
    int list_size = metadatalist.size();

    qDebug() << player->isMetaDataAvailable() << list_size;

    // Define variables to store metadata key and value
    QString metadata_key;
    QVariant var_data;

    for (int indx = 0; indx < list_size; indx++)
    {
        // Get the key from the list
        metadata_key = metadatalist.at(indx);

        // Get the value for the key
        var_data = player->metaData(metadata_key);

        qDebug() << metadata_key << var_data.toString();
        printf("%s : %s\n\r",metadata_key.toUtf8().data(),var_data.toString().toUtf8().data());
    }
    qDebug() <<  player->metaData(QMediaMetaData::Resolution).toString();
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


void RtspWindow::onPlayError(QMediaPlayer::Error error)
{
    qDebug() << "play error - " << error;
    ui->lblLoading->setText(player->errorString().toUtf8().data());
}

void RtspWindow::onPlayStateChanged(QMediaPlayer::State state)
{
    qDebug() << "play State - " << state;
    QNetworkRequest PStream;
    // handle state message
    switch (state) {
    case QMediaPlayer::State::PausedState:
        setStatusInfo(tr("Paused"));
        break;
    case QMediaPlayer::State::PlayingState:
        setStatusInfo(tr("Playing"));
        break;
    case QMediaPlayer::State::StoppedState:
        setStatusInfo(tr("Stopped"));
        break;
    }

}
void RtspWindow::onbufferStatusChanged(int percentFilled)
{
    qDebug() << "bufferStatusChanged - " << percentFilled;
}

void RtspWindow::onPlayStatusChanged(QMediaPlayer::MediaStatus status)
{
    qDebug() << "play Status - " << status;
    //ui->lblLoading->setText(QString(status).toUtf8().data());
    QNetworkRequest PStream;

    // handle status message
    switch (status) {
    case QMediaPlayer::UnknownMediaStatus:
    case QMediaPlayer::NoMedia:
    case QMediaPlayer::LoadedMedia:
        setStatusInfo(QString());
        break;
    case QMediaPlayer::LoadingMedia:
        setStatusInfo(tr("Loading..."));
        break;
    case QMediaPlayer::BufferingMedia:
    case QMediaPlayer::BufferedMedia:
        //setStatusInfo(tr("Buffering %1%").arg(player->bufferStatus()));
        setStatusInfo(tr("Loading video stream ........"));
        break;
    case QMediaPlayer::StalledMedia:
        setStatusInfo(tr("Stalled %1%").arg(player->bufferStatus()));
        break;
    case QMediaPlayer::EndOfMedia:
        QApplication::alert(this);
        break;
    case QMediaPlayer::InvalidMedia:
        displayErrorMessage();
        break;
    }
}

void RtspWindow::setStatusInfo(const QString &info)
{
    ui->lblLoading->show();
    ui->lblLoading->setText(info.toUtf8().data());
}

void RtspWindow::displayErrorMessage()
{
    ui->lblLoading->show();
    ui->lblLoading->setText(player->errorString());
}

void RtspWindow::on_PauseBtn_released()
{
    player->pause();
}

void RtspWindow::DisplayError(QString Source, unsigned int  ErrMess)
{

    QString QerrMess=ErrorManager::error_codes(Source, ErrMess);

    //printf("---Hik Sdk error response :from %s : %s\n\r", Source.toUtf8().data(), QerrMess.toUtf8().data());

    return;

}

int nPort = -1 ;
void RtspWindow::HikRtsp(unsigned char *pBuffer,unsigned int dwBufSize)
{

    qDebug() << "pBuffer  : " << pBuffer <<"\n";
    if (!PlayM4_GetPort(&nPort))
    {
        DisplayError("PlayM4_GetPort", PlayM4_GetLastError(nPort));
    }

    if (!PlayM4_SetStreamOpenMode(nPort, STREAME_REALTIME))
    {
        DisplayError("PlayM4_SetStreamOpenMode", PlayM4_GetLastError(nPort));
        PlayM4_FreePort(nPort);
        DisplayError("PlayM4_FreePort", PlayM4_GetLastError(nPort));
    }

    if (dwBufSize > 0)
    {
        if (!PlayM4_OpenStream(nPort, pBuffer, dwBufSize, 1920*1080*3))
        {
            DisplayError("PlayM4_OpenStream", PlayM4_GetLastError(nPort));
            PlayM4_FreePort(nPort);
            DisplayError("PlayM4_FreePort", PlayM4_GetLastError(nPort));
        }

        if (!PlayM4_SetDisplayCallBack(nPort, RemoteDisplayCBFun))
        {
            DisplayError("PlayM4_SetDisplayCallBack", PlayM4_GetLastError(nPort));
            PlayM4_CloseStream(nPort);
            DisplayError("PlayM4_CloseStream", PlayM4_GetLastError(nPort));
            PlayM4_FreePort(nPort);
            DisplayError("PlayM4_FreePort", PlayM4_GetLastError(nPort));
        }

        PLAYM4_HWND pWnd= centralWidget()->winId();
        if (!pWnd)
        {
            PlayM4_CloseStream(nPort);
            DisplayError("PlayM4_CloseStream", PlayM4_GetLastError(nPort));
            PlayM4_FreePort(nPort);
            DisplayError("PlayM4_FreePort", PlayM4_GetLastError(nPort));
            return;
        }
        if (!PlayM4_Play(nPort, pWnd))
        {
            DisplayError("PlayM4_Play", PlayM4_GetLastError(nPort));
            PlayM4_CloseStream(nPort);
            DisplayError("PlayM4_CloseStream", PlayM4_GetLastError(nPort));
            PlayM4_FreePort(nPort);
            DisplayError("PlayM4_FreePort", PlayM4_GetLastError(nPort));
        }
    }
    else
    {

        if (!PlayM4_Play(nPort,0))
        {
            DisplayError("PlayM4_Play", PlayM4_GetLastError(nPort));
            PlayM4_CloseStream(nPort);
            DisplayError("PlayM4_CloseStream", PlayM4_GetLastError(nPort));
            PlayM4_FreePort(nPort);
            DisplayError("PlayM4_FreePort", PlayM4_GetLastError(nPort));
        }
    }
}

/*********************************************************
  Function:	DisplayCBFun
  Desc:		the call back function to snatch the bmp pictrue
  Input:	nPort,port;pBuf,pic buffer;nSize,pic size;nWidth,pic width;nHeight,pic height;nType,type;
  Output:	none
  Return:	none
**********************************************************/
void CALLBACK RtspWindow::RemoteDisplayCBFun(int nPort, char *pBuf, int size, int width, int height,int stamp, int type, int reserved)
{
    DisplayError("RemoteDisplayCBFun", PlayM4_GetLastError(nPort));
}


/*void RtspWindow::processFrame(const QVideoFrame &frame)
{
    if (frame.isValid())
        //qDebug() << MediaStream->bytesAvailable();

        //HikRtsp((unsigned char*)frame.buffer(), sizeof(frame.buffer()));
        //PlayM4_InputData(nPort,(unsigned char*)frame.buffer(), sizeof(frame.buffer()));
        return; //drop frame

}*/

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

