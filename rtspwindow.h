#ifndef RTSPWINDOW_H
#define RTSPWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSettings>
#include <settingsform.h>
#include <QXmlStreamReader>
#include <QComboBox>
#include <pantilcmd.h>
#include <QtGui>

#if defined(_WIN32)
#pragma warning( disable : 4996)
#else
#endif

#include "include/HCNetSDK.h"
#include "include/PlayM4.h"

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
    /*******HCNetSDK**************/
    void LoginInfo(qint16 Port, QString sDeviceAddress, QString sUserName, QString sPassword, bool StreamChoice);
    void Play();
    /*******HCNetSDK**************/

private slots:
    void on_ComboBxCam_currentIndexChanged(const QString &arg1);

    void on_action_Streaming_Channels_1_triggered();

    void on_action_Streaming_Channels_2_triggered();

    void on_action_Streaming_Channels_3_triggered();

    void on_actionMetadata_triggered();

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

    void HideMenu();

    void on_actionmain_stream_triggered();

    void on_actionsub_stream_triggered();

    void on_actionClose_triggered();

    void on_actionConfigure_triggered();

    void on_PauseBtn_toggled(bool checked);

    void on_actionReconnect_triggered();

private:
    Ui::RtspWindow *ui;
    bool IsShown = false;
    void PlayRtsp(QString Camuri);
    void blink();
    void CallPresset(int Presset);

    static QString RtspUri;
    static QNetworkAccessManager *manager;

    static QString CamIp;
    static QString CamPort;
    static QString CamUser;
    static QString CamPass;
    static QString CamPortHttp;
    static bool ShowXML;
    static QString Chanel;
    static unsigned int PtzSpeed;

    void LoadPreset();
    void LoadPatrol();

    static QString SetXMLReq(int pan,int tilt,int zoom);
    void setStatusInfo(const QString &info);
    void displayErrorMessage(QString Err);
    void SetmediaRecorder();
    void HikRtsp(unsigned char *pBuffer,unsigned int dwBufSize);
    static void DisplayError(QString Source, unsigned int  ErrMess);
    static void CALLBACK RemoteDisplayCBFun(int nPort, char *pBuf, int size, int width, int height,int stamp, int type, int reserved);
    static void CALLBACK  EncryptTypeCBFun(int nPort, ENCRYPT_INFO* pEncryptInfo, void* nUser, int nReserved2);

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


    /*******HCNetSDK**************/
    static qint16 CamPortSdk;
    static bool IsNVR;

    NET_DVR_CLIENTINFO ClientInfo = {0};
    NET_DVR_PREVIEWINFO struPlayInfo = {0};
    NET_DVR_DEVICEINFO_V40 struDeviceInfo = {0};
    int lRealPlayHandle;
    static int lUserID;
    static int lChannel;
    static int lPort; //Global Player port NO.
    static int hWnd;

    //device data

    // Stream type 0-main stream,1-sub stream,2-third stream,3-forth
    int     StreamType = 0;
    //µÇÂ½Éè±¸ºó·µ»ØµÄÓÃ»§ID£»²»´æÈëÎÄ¼þ
    int     m_iuserid;
    //µÇÂ½Éè±¸ºó·µ»ØÉè±¸ÐÅÏ¢£»²»´æÈëÎÄ¼þ
    NET_DVR_DEVICEINFO_V30 m_deviceinfo;
    //Éè±¸Ãû³Æ£¬´æÈëÎÄ¼þ
    QString m_qdevicename;
    //Éè±¸IP£¬´æÈëÎÄ¼þ
    QString m_qip;
    //Éè±¸¶Ë¿Ú,´æÈëÎÄ¼þ
    int     m_qiport;
    //ÓÃ»§Ãû£¬´æÈëÎÄ¼þ
    QString m_qusername;
    //ÓÃ»§ÃÜÂë£¬´æÈëÎÄ¼þ
    QString m_qpassword;
    //²¼·ÀÖÐ >=0  ·ñÔò -1
    int m_ideployed;
    //¶à²¥ipµØÖ·
    QString m_multiCast;
    //×ÓÊôÐÔÍ¨µÀ½ÚµãÁÐ±í£¬ÆäÄÚÈÝÒ²Òª´æÈëÎÄ¼þ£»
    //QList<ChannelData> m_qlistchanneldata;

    void __stdcall  RealDataCallBack(int lRealHandle,int dwDataType,unsigned char *pBuffer,int  dwBufSize, void* dwUser);
    static void __stdcall g_ExceptionCallBack(int dwType, int lUserID, int lHandle, void *pUser);
    static void CALLBACK SetDecCallBack(int nPort,char * pBuf,int nSize,FRAME_INFO * pFrameInfo, void* nUser,int nReserved2);
    static void CALLBACK g_RealDataCallBack_V30(int lRealHandle, int dwDataType, unsigned char *pBuffer,int dwBufSize,void* dwUser);
    /*******HCNetSDK**************/

public slots:
    void replyFinished (QNetworkReply *reply);
    void authenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator);
    //void processFrame(QVideoFrame frame, int levels);
    //void processFrame(const QVideoFrame &frame);

protected slots:

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
