#include "playm4interface.h"
#include "errormanager.h"
#include "qtvsplayer.h"
#include "ui_qtvsplayer.h"
//#include <include/PlayM4.h>
#include "filesliste.h"
#include "videoctrls.h"
#include <string>
#include <QtDebug>

using namespace std;



PLAYM4_HWND  playm4interface::hwnd;
int playm4interface::m_pblocalportnum = -1;
QString playm4interface::m_pblocalfilepath;
bool playm4interface::FsOpened = false;
void* playm4interface::nUser;

unsigned int  playm4interface::VideoFs(QString fileName)
{
    VideoCtrls::EndRead = false;
    BOOL bSuccess;

    //initval();
    m_pblocalfilepath = fileName;

    printf("pyd---File name:%s\n\r",m_pblocalfilepath.toUtf8().data());

    if (FsOpened == true) {

        bSuccess = Stop();

        bSuccess = PlayM4_CloseFile(m_pblocalportnum);
        DisplayError("PlayM4_CloseFile", PlayM4_GetLastError(m_pblocalportnum));
        FsOpened = false;
    }
    if (m_pblocalportnum == -1) {
        //bSuccess = PlayM4_GetPort((int *)&m_pblocalportnum);
        SetPort();
        //DisplayError("PlayM4_GetPort", PlayM4_GetLastError(m_pblocalportnum));
    }

    //InitCallback(0,PlayM4_GetFileTime(m_pblocalportnum));
    InitCallback(0,1);

    FsOpened= PlayM4_OpenFile(m_pblocalportnum, m_pblocalfilepath.toUtf8().data());
    DisplayError("PlayM4_OpenFile",PlayM4_GetLastError(m_pblocalportnum));

    GetCap(PlayM4_GetCaps());

    //BOOL __stdcall    PlayM4_SetDecCallBack(LONG nPort,void (CALLBACK* DecCBFun)(long nPort,char * pBuf,long nSize,FRAME_INFO * pFrameInfo, long nReserved1,long nReserved2));

#if (defined(_WIN32))
    //PlayM4_SetDecCallBack(m_pblocalportnum, (void (CALLBACK *)(long,char *,long,long,long,long,long,long))PlayM4DisplayCallBack);
#elif defined(__linux__)
    //callback work, but no sound anymore !!!
    //int  PlayM4_SetDecCallBackMend(int nPort,void (CALLBACK* DecCBFun)(int nPort,char * pBuf,int nSize,FRAME_INFO * pFrameInfo, void* nUser,int nReserved2), void* nUser);
    //PlayM4_SetDecCallBackMend(m_pblocalportnum, (void (CALLBACK *)(int,char *,int,FRAME_INFO *, void*,int))SetDecCallBack, nUser);
#endif

    bSuccess = PlayM4_Play(m_pblocalportnum, hwnd);
    DisplayError("PlayM4_Play",PlayM4_GetLastError(m_pblocalportnum));

    bSuccess = PlayM4_PlaySound(m_pblocalportnum);
    DisplayError("PlayM4_PlaySound",PlayM4_GetLastError(m_pblocalportnum));


    delay();
    QtVsPlayer::WVideoCtrls->RestoreSeek();

    if (bSuccess) {

        //QtVsPlayer::SetWindowTitle(m_pblocalfilepath.toUtf8().data());
    }

    QtVsPlayer::WVideoCtrls->InitTimeSlide();


    SetVideoWin(0);
    RefreshPlay();
#if (defined(__linux__))
PlayM4_WndResolutionChange(m_pblocalportnum);
#endif

    //emit DisplayError(PlayM4_GetLastError(m_pblocalportnum));
    return 0;//PlayM4_GetLastError(m_pblocalportnum);
}

void playm4interface::delay()
{
    QTime dieTime= QTime::currentTime().addSecs(1);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void playm4interface::DisplayError(QString Source, unsigned int  ErrMess)
{

    QString QerrMess=ErrorManager::error_codes(Source, ErrMess);

    //printf("pyd---Hik Sdk error response :from %s : %s\n\r", Source.toUtf8().data(), QerrMess.toUtf8().data());

    return;

}

void playm4interface::SetPort()
{
    if (m_pblocalportnum == -1) {
        //FreePort();
        BOOL bSuccess;
        //from qtdemo//////
        unsigned int dwVersion = PlayM4_GetSdkVersion();
        QString strPlaySDKVersion;
        printf("PlayCtrl V%d.%d.%d.%d\n\r", (0xff000000 & dwVersion)>>24, (0x00ff0000 & dwVersion)>>16,\
               (0x0000ff00 & dwVersion)>>8, (0x000000ff & dwVersion));
        //ui.m_labelPlayerSDKVersion->setText(strPlaySDKVersion);
        //printf("\n\r%.8x <- PlayM4_GetSdkVersion()\n\r",PlayM4_GetSdkVersion());


        bSuccess = PlayM4_GetPort((int *)&m_pblocalportnum);
        printf("Debug---Port:%d\n\r",m_pblocalportnum);
        DisplayError("PlayM4_GetPort", PlayM4_GetLastError(m_pblocalportnum));

        //surpress warning!
        if (bSuccess == false ) {
            //I'm happy for yu
        }
        //surpress warning!

        VideoCtrls::HikNumPort = m_pblocalportnum;
    }
    return;
}

void playm4interface::FreePort()
{
    if (FsOpened == true) {
        BOOL bSuccess = false;
        bSuccess = Stop();

        bSuccess = PlayM4_CloseFile(m_pblocalportnum);
        DisplayError("PlayM4_CloseFile", PlayM4_GetLastError(m_pblocalportnum));

        bSuccess = PlayM4_FreePort(m_pblocalportnum);
        DisplayError("PlayM4_FreePort", PlayM4_GetLastError(m_pblocalportnum));

        if (bSuccess == true) {
            m_pblocalportnum = -1;
        }

    }

    return;
}

int playm4interface::RefreshPlay()
{
    //return PlayM4_RefreshPlay(m_pblocalportnum);
    //DisplayError("PlayM4_RefreshPlay", PlayM4_GetLastError(m_pblocalportnum));
    return 0;
}

int playm4interface::SetVideoWin(unsigned int nRegionNum)
{
    return PlayM4_SetVideoWindow(m_pblocalportnum,nRegionNum,hwnd);
}

int playm4interface::Pause(unsigned int nPause)//nPause=1 pause, =0 resume
{
    return PlayM4_Pause(m_pblocalportnum,nPause);
}

int playm4interface::Play()
{
    return PlayM4_Play(m_pblocalportnum,hwnd);
}

int playm4interface::Stop()
{
    PlayM4_Stop(m_pblocalportnum);
    return PlayM4_StopSound();
}

int playm4interface::Fast()
{
    //printf("Debug---Fastest Port:%s",(std::to_string(playm4interface::m_pblocalportnum)).data());
    return PlayM4_Fast(playm4interface::m_pblocalportnum);

}

int playm4interface::Slow()
{
    return PlayM4_Slow(m_pblocalportnum);
}

int playm4interface::OneByOne()
{
    return PlayM4_OneByOne(m_pblocalportnum);
}

int playm4interface::OneByOneBack()
{


#if (defined(__linux__))
PlayM4_WndResolutionChange(m_pblocalportnum);
#endif

    return PlayM4_ReversePlay(m_pblocalportnum);
    //return PlayM4_OneByOneBack(m_pblocalportnum);
}

int playm4interface::SetVolume(unsigned short Volume)
{
    unsigned short GetVolume = PlayM4_GetVolume(m_pblocalportnum);
    return PlayM4_SetVolume(m_pblocalportnum, Volume);
}

/*
void playm4interface::initval()
{

    //parameters Assignment
    m_pbUserID = usrid;
    m_pbCurrChannel = channelnum;

    //m_pblocalfilepath.clear();
    m_pbdstarttime.clear();
    m_pbdstoptime.clear();
    m_pbfiletype = 0;
    m_pbfileattr = 0;
    m_pbdownloadhandle = -1;

    //play handle
    m_pbhandle = -1;

}
*/

void playm4interface::GetCap(int nFlag)
{
    QString csCap="";
    if(!(nFlag&SUPPORT_SSE))
    {
        csCap+="Don't support SSE instruction set;\r\n";
    }

    if(!(nFlag&SUPPORT_DDRAW))
    {
        csCap+="Create DirectDraw faild;\r\n";
    }

    if(!(nFlag&SUPPORT_BLT))
    {
        csCap+="Error when blitting overlay or offscreen;Error when blitting overlay or offscreen;\r\n";
    }

    if(!(nFlag&SUPPORT_BLTFOURCC))
    {
        csCap+="Don't support color-space conversions;\r\n";
    }

    if(!(nFlag&SUPPORT_BLTSHRINKX))
    {
        csCap+="Don't support arbitrary shrinking of a surface along the x-axis\r\n";
    }

    if(!(nFlag&SUPPORT_BLTSHRINKY))
    {
        csCap+="Don't supports arbitrary shrinking of a surface along the y-axis (vertically);\r\n";
    }

    if(!(nFlag&SUPPORT_BLTSTRETCHX))
    {
        csCap+="Don't supports arbitrary stretching of a surface along the x-axis;\r\n";
    }

    if(!(nFlag&SUPPORT_BLTSTRETCHY))
    {
        csCap+="Don't supports arbitrary stretching of a surface along the y-axis;\r\n";
    }

    if(csCap.length()>0)
    {
        csCap+="If your video adapter chip is made by nVidia,please update the new driver!\r\n";

    }

    printf("\n\r%s\n\r",csCap.toUtf8().data());
}


void playm4interface::GetMetadatas()
{

    HIK_MEDIAINFO HIKMI;
    qDebug() <<  HIKMI.video_format;
}


void playm4interface::InitCallback( unsigned int nBeginTime, unsigned int nEndTime)
{

#if (defined(_WIN32))
    //PlayM4_SetFileRefCallBack(m_pblocalportnum, (void (__stdcall *)(long,char *,long,long,long,long,long,long))PlayM4DisplayCallBack);
#elif defined(__linux__)
    //int  PlayM4_SetFileRefCallBack(int nPort, void (CALLBACK *pFileRefDone)(unsigned int nPort,void* nUser),void* nUser);
    PlayM4_SetFileRefCallBack(m_pblocalportnum, (void (CALLBACK *)(unsigned int,void*))SetFileRefCallBack,nUser);
#endif

#if (defined(_WIN32))
    //PlayM4_SetAudioCallBack(m_pblocalportnum, (void (__stdcall *)(long,char *,long,long,long,long,long,long))PlayM4DisplayCallBack);
#elif defined(__linux__)
    //int          PlayM4_SetAudioCallBack(int nPort, void (__stdcall* funAudio)(int nPort, char* pAudioBuf, int nSize, int nStamp, int nType, int nUser), int nUser);
    PlayM4_SetAudioCallBack(m_pblocalportnum, (void (CALLBACK *)(int, char*, int, int, int, int))SetAudioCallBack,0);
#endif


#if (defined(_WIN32))
    //PlayM4_SetVerifyCallBack(m_pblocalportnum, (void (__stdcall *)(long,char *,long,long,long,long,long,long))PlayM4DisplayCallBack);
#elif defined(__linux__)
    //int          PlayM4_SetVerifyCallBack(int nPort, unsigned int nBeginTime, unsigned int nEndTime, void (__stdcall* funVerify)(int nPort, FRAME_POS* pFilePos, unsigned int bIsVideo, unsigned int nUser), unsigned int nUser);
    PlayM4_SetVerifyCallBack(m_pblocalportnum, nBeginTime, nEndTime,(void (CALLBACK *)(int, FRAME_POS*, unsigned int, unsigned int ))SetVerifyCallBack,0);
#endif


    //PlayM4_SetFileEndMsg(m_pblocalportnum, hwnd,0);


}

void CALLBACK playm4interface::SetFileRefCallBack(int nPort,void* nUser)
{
    printf("SetFileRefCallBack---%u:%u\n\r",nPort, nUser);

}

void CALLBACK playm4interface::SetAudioCallBack(int nPort, char* pAudioBuf, int nSize, int nStamp, int nType, int nUser)
{
    printf("SetAudioCallBack---%d:%d\n\r",nPort, nUser);

}

void CALLBACK playm4interface::SetVerifyCallBack(int nPort, FRAME_POS* pFilePos, unsigned int bIsVideo, unsigned int nUser)
{
    printf("SetVerifyCallBack---%d:%d:%d:%u\n\r", nPort,pFilePos->nErrorFrameSize, bIsVideo, nUser);

}

void CALLBACK playm4interface::SetDecCallBack(int nPort,char * pBuf,int nSize,FRAME_INFO * pFrameInfo, void* nUser,int nReserved2)
{
    //printf("SetDecCallBack---%ld:%ld\n\r",nPort, nSize);

}
