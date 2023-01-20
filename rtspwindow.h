#ifndef RTSPWINDOW_H
#define RTSPWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <include/PlayM4.h>
#include <QVideoProbe>
#include <QSettings>
#include <settingsform.h>
#include <QXmlStreamReader>
#include <QComboBox>
#include <pantilcmd.h>

class QVideoProbe;


namespace Ui {
class RtspWindow;
}

class RtspWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RtspWindow(QWidget *parent = nullptr);
    ~RtspWindow();
    /*******PANTIL CMD************/
    static void PanTilTopLeft_pressed();
    static void PanTilUp_pressed();
    static void PanTilTopRight_pressed();
    static void PanTilRight_pressed();
    static void PanTilBottomRight_pressed();
    static void PanTilDown_pressed();
    static void PanTilBottomLeft_pressed();
    static void PanTilLeft_pressed();
    /*******PANTIL CMD************/

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

    void on_PauseBtn_released();

    void HideMenu();

private:
    Ui::RtspWindow *ui;
    void PlayRtsp(QString Camuri);
    void blink();
    void CallPresset(int Presset);
    static QMediaPlayer *player;
    static QVideoWidget *videoWidget;

    static QString RtspUri;
    static QNetworkAccessManager *manager;

    static QString CamIp;
    static QString CamPort;
    static QString CamUser;
    static QString CamPass;
    static QString CamPortHttp;
    static QString Chanel;
    static unsigned int PtzSpeed;

    void LoadPreset();
    void LoadPatrol();

    static QString SetXMLReq(int pan,int tilt,int zoom);
    void GetMetaData(QMediaPlayer *player);
    void setStatusInfo(const QString &info);
    void displayErrorMessage();
    void SetmediaRecorder();
    void HikRtsp(unsigned char *pBuffer,unsigned int dwBufSize);
    static void DisplayError(QString Source, unsigned int  ErrMess);
    static void CALLBACK RemoteDisplayCBFun(int nPort, char *pBuf, int size, int width, int height,int stamp, int type, int reserved);

    //QVideoProbe *m_videoProbe = nullptr;
    QSettings settings;
    SettingsForm SetFrm;
    QStringList PresetList;
    void Sleep(int MSecs);
    QTimer *ShowHideTimer;
    bool MenubarHasFocus(QMenuBar *menu);
    QString GetWinState();

    QRect TopLeft;
    QRect TopRight;
    QRect BottomRight;
    QRect BottomLeft;

    static PanTilCmd *PTCmd;
    static void SendPTZ(int pan,int tilt,int zoom);

public slots:
    void replyFinished (QNetworkReply *reply);
    void authenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator);
    //void processFrame(QVideoFrame frame, int levels);
    //void processFrame(const QVideoFrame &frame);

protected slots:
    void onPlayStatusChanged(QMediaPlayer::MediaStatus status);
    void onPlayError(QMediaPlayer::Error error);
    void onPlayStateChanged(QMediaPlayer::State state);
    void onbufferStatusChanged(int percentFilled);

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *e) override;
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;
};

#endif // RTSPWINDOW_H
