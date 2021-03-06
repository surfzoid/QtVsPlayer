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

    void on_PtzSliderSpeed_valueChanged(int value);

    void on_actionPreset_triggered();

    void on_actionPatrol_triggered();

    void on_comboBxPresset_editTextChanged(const QString &arg1);

    void on_comboBxPatrol_editTextChanged(const QString &arg1);

    void on_comboBxPresset_textActivated(const QString &arg1);

    void on_comboBxPatrol_textActivated(const QString &arg1);

    void on_actionReboot_triggered();

    void on_RecordBtn_toggled(bool checked);

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
    static QString Chanel;
    static unsigned int PtzSpeed;

    static bool IsPressed;

    void LoadPreset();
    void LoadPatrol();

    QString SetXMLReq(int pan,int tilt,int zoom);
    void GetMetaData(QMediaPlayer *player);
    void setStatusInfo(const QString &info);
    void displayErrorMessage();
    void SetmediaRecorder();

public slots:
    void replyFinished (QNetworkReply *reply);
    void authenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator);

protected slots:
    void onPlayStatusChanged(QMediaPlayer::MediaStatus status);
    void onPlayError(QMediaPlayer::Error error);
    void onPlayStateChanged(QMediaPlayer::State state);
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

};

#endif // RTSPWINDOW_H
