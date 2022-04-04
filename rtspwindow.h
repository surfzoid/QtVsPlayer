#ifndef RTSPWINDOW_H
#define RTSPWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace Ui {
class RtspWindow;
}

class RtspWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RtspWindow(QWidget *parent = nullptr);
    ~RtspWindow();

private slots:
    void on_ComboBxCam_currentIndexChanged(const QString &arg1);
    void positionChanged(qint64 pos);


    void on_actionIgnoreAspectRatio_triggered();

    void on_actionDefault_triggered();

    void on_actionKeepAspectRatio_triggered();

    void on_actionKeepAspectRatioByExpanding_triggered();

    void on_action_Streaming_Channels_1_triggered();

    void on_action_Streaming_Channels_2_triggered();

    void on_action_Streaming_Channels_3_triggered();

    void on_actionMetadata_triggered();

    void on_action_ISAPI_Streaming_channels_101_triggered();

    void on_comboBxPresset_activated(int index);

    void on_comboBxPatrol_activated(int index);

    void on_SnapshotBtn_pressed();

private:
    Ui::RtspWindow *ui;
    void PlayRtsp(QString Camuri);
    void blink();
    void PanTilt(QMouseEvent *event);
    void CallPresset(int Presset);
    static QMediaPlayer *player;
    static QVideoWidget *videoWidget;
    static QString RtspUri;
    QNetworkAccessManager *manager;

    static QString CamIp;
    static QString CamPort;
    static QString CamUser;
    static QString CamPass;
    static QString CamPortHttp;

    static bool IsPressed;

    QString SetXMLReq(int pan,int tilt,int zoom);

public slots:
    void replyFinished (QNetworkReply *reply);
    void authenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

};

#endif // RTSPWINDOW_H
