#include "rtspwindow.h"
#include "ui_rtspwindow.h"
#include <QComboBox>
#include <QWidgetAction>
#include <QSettings>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QVBoxLayout>
#include <QTime>
#include <QTimer>
#include <QWheelEvent>
#include <QAuthenticator>
#include <QUrl>
#include <QUrlQuery>
#include <QDebug>

QMediaPlayer *RtspWindow::player;
QVideoWidget *RtspWindow::videoWidget;
QString RtspWindow::RtspUri;

QString RtspWindow::CamIp = "192.168.";
QString RtspWindow::CamPort = "554";
QString RtspWindow::CamUser = "admin";
QString RtspWindow::CamPass = "hik12345";
QString RtspWindow::CamPortHttp = "801";

bool RtspWindow::IsPressed = false;

RtspWindow::RtspWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RtspWindow)
{
    RtspWindow::player = new QMediaPlayer(this);
    RtspWindow::videoWidget = new QVideoWidget(this);
    connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));

    ui->setupUi(this);

    setCentralWidget(videoWidget);


    ui->comboBxPresset->clear();
    for (int i=1; i<16; i++){
        ui->comboBxPresset->addItem("Presset" + QString::number(i));
    }

    ui->comboBxPatrol->clear();
    for (int i=1; i<10; i++){
        ui->comboBxPatrol->addItem("Patrol" + QString::number(i));
    }

    ui->menubar->setCornerWidget(ui->ComboBxs);
    ui->statusbar->addPermanentWidget(ui->lblLoading);

    QSettings settings;
    settings.beginGroup("CamsName");
    QStringList keys = settings.allKeys();

    ui->ComboBxCam->clear();
    for (int i=0; i<keys.length(); i++){
        ui->ComboBxCam->addItem(keys[i]);
    }

    settings.endGroup();


    manager = new QNetworkAccessManager(this);

    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));

    connect(manager, SIGNAL(authenticationRequired(QNetworkReply*, QAuthenticator*)),
            this, SLOT(authenticationRequired(QNetworkReply*, QAuthenticator*)));





}

RtspWindow::~RtspWindow()
{

    if (videoWidget != NULL) {
        //delete videoWidget;
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
    //player->setParent(this->centralWidget());
    videoWidget->setAutoFillBackground(true);
    videoWidget->setAspectRatioMode(Qt::KeepAspectRatio);
    videoWidget->setSizePolicy(QSizePolicy::Preferred,
                               QSizePolicy::Maximum);


    player->setMedia(QUrl(Camuri));
    player->play();
    videoWidget->setAspectRatioMode(Qt::KeepAspectRatioByExpanding);
}

void RtspWindow::on_ComboBxCam_currentIndexChanged(const QString &arg1)
{

    QSettings settings;

    settings.beginGroup(arg1);

    CamIp = settings.value("Ip", "").value<QString>();
    CamPort = settings.value("Port", "554").value<QString>();
    CamPortHttp = settings.value("PortHttp", "800").value<QString>();
    CamUser = settings.value("User", "admin").value<QString>();
    CamPass = settings.value("Password", "hik12345").value<QString>();
    settings.endGroup();

    RtspUri = "rtsp://" + CamUser + ":" + CamPass +
            "@" + CamIp + ":" + CamPort + "/Streaming/Channels/2";


    foreach( QAction *const EntryMenu , ui->menuchannel->actions())
    {
        if (EntryMenu->isChecked() == true) {
            EntryMenu->activate(QAction::Trigger);
        }
    }
}

void RtspWindow::positionChanged(qint64 pos)
{
    printf("%lli", pos);
    QTime time(0,0,0,0);
    time = time.addMSecs(pos);

    if(pos != 0) this->ui->statusbar-> showMessage("Elapsed time: " + time.toString());

    if (pos == 0) {
        blink();
    }else {
        ui->lblLoading->setVisible(false);
    }

}

void RtspWindow::blink()
{
    if(ui->lblLoading->isHidden())
        ui->lblLoading->show();
    else
        ui->lblLoading->hide();
}

void RtspWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    //surpress warning!
    if (event == nullptr) {
        event = nullptr;
    }
    //surpress warning!

    if (isFullScreen()) {
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
    /*videoWidget->setUpdatesEnabled(true);
    videoWidget->update();
    player->pause();
    player->play();*/
}

void RtspWindow::on_actionKeepAspectRatioByExpanding_triggered()
{
    videoWidget->setAspectRatioMode(Qt::KeepAspectRatioByExpanding);
}

void RtspWindow::on_action_Streaming_Channels_1_triggered()
{
    RtspUri = RtspUri.remove(RtspUri.length() - 1,1) + "1";

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

    RtspUri = RtspUri.remove(RtspUri.length() - 1,1) + "2";

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

    RtspUri = RtspUri.remove(RtspUri.length() - 1,1) + "3";

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

    RtspUri = RtspUri.remove(RtspUri.length() - 1,1) + "/ISAPI/Streaming/channels/101";

    ui->action_Streaming_Channels_1->setChecked(false);
    ui->action_Streaming_Channels_3->setChecked(false);
    PlayRtsp(RtspUri);
}

void RtspWindow::wheelEvent(QWheelEvent *event)
{
    QPoint Scroll = event->angleDelta();

    QUrl Adresse("http://" + CamIp + ":" + CamPortHttp +
                 "/ISAPI/PTZCtrl/channels/1/Momentary");

    if (Scroll.y() > 0)
    {

        /*manager->get(QNetworkRequest(QUrl(
                 "http://" + CamIp + ":" + CamPortHttp +
                                         "/ISAPI/PTZCtrl/channels/1/capabilities")));
        manager->get(QNetworkRequest(QUrl(
                 "http://" + CamIp + ":" + CamPortHttp +
                                         "/ISAPI/PTZCtrl/channels/1/capabilities")));*/

        manager->put((QNetworkRequest)Adresse,
                     SetXMLReq(0,0,100).toUtf8());

    }

    if (Scroll.y() < 0 )
    {
        manager->put((QNetworkRequest)Adresse,
                     SetXMLReq(0,0,-100).toUtf8());

    }

}

void RtspWindow::mouseMoveEvent(QMouseEvent *event)
{
}

void RtspWindow::mousePressEvent(QMouseEvent *event)
{

    if ((event->buttons() == Qt::RightButton))
       {
           IsPressed = true;
           PanTilt(event);
       }

}

void RtspWindow::PanTilt(QMouseEvent *event)
{
    QUrl Adresse("http://" + CamUser + ":" + CamPass +
                 "@" + CamIp + ":" + CamPortHttp +
                 "/ISAPI/PTZCtrl/channels/1/Momentary");
    int WheightC = this->height()/2;//h_y
    int WwidthC = this->width()/2;//l_x
    int WheightOfS = this->height()/2 - (WheightC/2);//h_y
    int WwidthCOfS = this->width()/2 - (WwidthC/2);//l_x

        if (event->globalY() > WheightC and event->globalY() < WheightC - WheightOfS)
            manager->put((QNetworkRequest)Adresse,
                         SetXMLReq(0,-100,0).toUtf8());
        if (event->globalY() > WheightC - WheightOfS)
            manager->put((QNetworkRequest)Adresse,
                         SetXMLReq(0,-100,0).toUtf8());

        if (event->globalY() < WheightC and event->globalY() > WheightC + WheightOfS)
            manager->put((QNetworkRequest)Adresse,
                         SetXMLReq(0,100,0).toUtf8());
        if (event->globalY() < WheightC + WheightOfS)
            manager->put((QNetworkRequest)Adresse,
                         SetXMLReq(0,100,0).toUtf8());

        if (event->globalX() > WwidthC and event->globalX() < WwidthC - WwidthCOfS)
            manager->put((QNetworkRequest)Adresse,
                         SetXMLReq(100,0,0).toUtf8());
        if (event->globalX() > WwidthC - WwidthCOfS)
            manager->put((QNetworkRequest)Adresse,
                         SetXMLReq(100,0,0).toUtf8());

        if (event->globalX() < WwidthC and event->globalX() > WwidthC + WwidthCOfS)
            manager->put((QNetworkRequest)Adresse,
                         SetXMLReq(-100,0,0).toUtf8());
        if (event->globalX() < WwidthC + WwidthCOfS)
            manager->put((QNetworkRequest)Adresse,
                         SetXMLReq(-100,0,0).toUtf8());
}

void RtspWindow::mouseReleaseEvent(QMouseEvent *event)
{
    //surpress warning!
    if (event == nullptr) {
        event = nullptr;
    }
    //surpress warning!

    IsPressed = false;
}

void RtspWindow::replyFinished(QNetworkReply *reply)
{
    if(reply->error())
    {
        qDebug() << "ERROR!";
        printf("finish : %s\n\r", reply->errorString().toUtf8().data());
        qDebug() << reply->readAll();
    }
    else
    {
        qDebug() << reply->header(QNetworkRequest::ContentTypeHeader).toString();
        qDebug() << reply->header(QNetworkRequest::LastModifiedHeader).toDateTime().toString();;
        qDebug() << reply->header(QNetworkRequest::ContentLengthHeader).toULongLong();
        qDebug() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        qDebug() << reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
        qDebug() << reply->header(QNetworkRequest::UserAgentHeader).toString();
        qDebug() << reply->readAll();


    }

    reply->deleteLater();
}

void RtspWindow::authenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator)
{

    if(reply->error())
    {
        qDebug() << "ERROR!";
        qDebug() << reply->errorString();
        printf("finish : %s\n\r", reply->errorString().toUtf8().data());
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


/************************************************************************
    url='PTZCtrl/channels/1/momentary'
    xml='<PTZData”>
    <pan>0</pan>              # Rotation de -100 à 100 (gauche ou droite)
    <tilt>0</tilt>            # Inclinaison de -100 à 100 (haut ou bas)
    <zoom>0</zoom>            # Zoom -100 à 100 (zoom+ ou zoom-)
    <Momentary>
    <duration>1000</duration> # Durée du mouvement en millisecondes
    </Momentary>
    </PTZData>'
************************************************************************/
QString RtspWindow::SetXMLReq(int pan,int tilt,int zoom)
{
    QString XmlData = "<PTZData>\r\n\
<pan>" + QString::number(pan) + "</pan>\r\n\
            <tilt>" + QString::number(tilt) + "</tilt>\r\n\
            <zoom>" + QString::number(zoom) + "</zoom>\r\n\
            <Momentary>\r\n\
            <duration>500</duration>\r\n\
            </Momentary>\r\n\
            </PTZData>";

        return XmlData;
}


void RtspWindow::CallPresset(int Presset)
{

    QString AdPath = ("/PTZ/channels/1/PTZControl?command=GOTO_PRESET&presetNo=" +
                      QString::number(Presset) + "&speed=7&mode=start").toUtf8();
    QUrl Adresse("http://" + CamUser + ":" + CamPass +
                 "@" +  CamIp + ":" + CamPortHttp + AdPath);

    manager->get((QNetworkRequest)Adresse);
}


void RtspWindow::on_comboBxPresset_activated(int index)
{
    CallPresset(index + 1);
}

void RtspWindow::on_comboBxPatrol_activated(int index)
{
    QString AdPath = ("/ISAPI/PTZCtrl/channels/1/patrols/" +
                      QString::number(index + 1) + "/start").toUtf8();
    QUrl Adresse("http://" + CamUser + ":" + CamPass +
                 "@" + CamIp + ":" + CamPortHttp + AdPath);

    QIODevice * outgoingData = 0;
    manager->put((QNetworkRequest)Adresse,outgoingData);
}
