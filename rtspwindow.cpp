#include "rtspwindow.h"
#include "ui_rtspwindow.h"
#include <QComboBox>
#include <QWidgetAction>
#include <QSettings>
#include <QMediaPlayer>
#include <QVideoWidget>


QMediaPlayer *RtspWindow::player;
QVideoWidget *RtspWindow::videoWidget;

RtspWindow::RtspWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RtspWindow)
{
    RtspWindow::player = new QMediaPlayer();
    RtspWindow::videoWidget = new QVideoWidget;
    connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
    ui->setupUi(this);

    ui->menubar->setCornerWidget(ui->ComboBxCam);
    ui->statusbar->addPermanentWidget(ui->lblLoading);

    QSettings settings;
    settings.beginGroup("CamsName");
    QStringList keys = settings.allKeys();

    ui->ComboBxCam->clear();
    for (int i=0; i<keys.length(); i++){
        ui->ComboBxCam->addItem(keys[i]);
    }

    settings.endGroup();


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

    setCentralWidget(videoWidget);
    player->setMedia(QUrl(Camuri));
    player->play();
    videoWidget->setAspectRatioMode(Qt::KeepAspectRatioByExpanding);
}

void RtspWindow::on_ComboBxCam_currentIndexChanged(const QString &arg1)
{

    QSettings settings;

    settings.beginGroup(arg1);

    QString CamIp = settings.value("Ip", "").value<QString>();
    QString CamPort = settings.value("Port", "554").value<QString>();
    QString CamUser = settings.value("User", "admin").value<QString>();
    QString CamPass = settings.value("Password", "hik12345").value<QString>();
    settings.endGroup();

    QString RtspUri = "rtsp://" + CamUser + ":" + CamPass + "@" + CamIp + ":" + CamPort;

    PlayRtsp(RtspUri);
}

void RtspWindow::positionChanged(qint64 pos)
{
    printf("%lli", pos);
    this->ui->statusbar-> showMessage(QString::number(pos));
    if (pos == 0) {
        ui->lblLoading->setVisible(true);
    }else {
        ui->lblLoading->setVisible(false);
    }
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
        videoWidget->showMaximized();
        //videoWidget->setFullScreen(false);
    } else {
        showFullScreen();
        menuBar()->setVisible(false);
        statusBar()->setVisible(false);
        videoWidget->showFullScreen();
        //videoWidget->setFullScreen(true);
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
