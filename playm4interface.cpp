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



bool isIndexed = false;
bool isound = false;
PLAYM4_HWND  playm4interface::hwnd;
int playm4interface::m_pblocalportnum = -1;
QString playm4interface::m_pblocalfilepath;
bool playm4interface::FsOpened = false;
void* playm4interface::nUser;
FRAME_INFO *playm4interface::pFRAME_INFO;

unsigned int  playm4interface::VideoFs(QString fileName)
{

    VideoCtrls::EndRead = false;
    isound = false;
    isIndexed = false;
    //initval();
    m_pblocalfilepath = fileName;

    printf("pyd---File name:%s\n\r",m_pblocalfilepath.toUtf8().data());

    if (FsOpened == true) {

        Stop();
        FreePort();
        FsOpened = false;
        m_pblocalportnum = -1;
    }
    if (m_pblocalportnum == -1) {

        SetPort();
    }


    FsOpened= PlayM4_OpenFile(m_pblocalportnum, m_pblocalfilepath.toUtf8().data());
    InitCallback(1,360);

    if (!FsOpened)
    {
        DisplayError("PlayM4_OpenFile",PlayM4_GetLastError(m_pblocalportnum));
    }

    QTime dieTime= QTime::currentTime().addSecs(10);
    while(!isIndexed) { // STEP-5
        //delay();
        if (QTime::currentTime() > dieTime) {
            break;
        }
    }

    if (!PlayM4_GetCaps())
    {
        DisplayError("PlayM4_GetCaps",PlayM4_GetLastError(m_pblocalportnum));
        //PlayM4_SetCurrentFrameNum(m_pblocalportnum, 0);
    }


    if (!PlayM4_PlaySound(m_pblocalportnum))
    {
        DisplayError("PlayM4_PlaySound",PlayM4_GetLastError(m_pblocalportnum));
    }

    if (!PlayM4_Play(m_pblocalportnum, hwnd))
    {
        DisplayError("PlayM4_Play",PlayM4_GetLastError(m_pblocalportnum));
        //PlayM4_SetCurrentFrameNum(m_pblocalportnum, 0);
    }

    int frames = PlayM4_GetFileTotalFrames(m_pblocalportnum); // STEP-6
    qDebug()  << frames << " <- PlayM4_GetFileTotalFrames()";

    int FsTime =PlayM4_GetFileTime(m_pblocalportnum);
    qDebug() << "File Time" << FsTime;

    PlayM4_GetSourceBufferRemain(m_pblocalportnum);
    PlayM4_ResetSourceBuffer(m_pblocalportnum);
    PlayM4_ResetSourceBufFlag(m_pblocalportnum);

    int width = 0, height = 0;
    PlayM4_GetPictureSize(m_pblocalportnum,&width,&height);
    qDebug()  << width << "x" << height << " <- PlayM4_GetPictureSize()" ;


    //delay();
    QtVsPlayer::WVideoCtrls->RestoreSeek();


    QtVsPlayer::WVideoCtrls->InitTimeSlide();



    RefreshPlay();

    if (!PlayM4_SyncToAudio(m_pblocalportnum, true))
    {
        DisplayError("PlayM4_SyncToAudio",PlayM4_GetLastError(m_pblocalportnum));
    }
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
        //bool bSuccess;
        //from qtdemo//////
        unsigned int dwVersion = PlayM4_GetSdkVersion();
        QString strPlaySDKVersion;
        printf("PlayCtrl V%d.%d.%d.%d\n\r", (0xff000000 & dwVersion)>>24, (0x00ff0000 & dwVersion)>>16,\
               (0x0000ff00 & dwVersion)>>8, (0x000000ff & dwVersion));
        //ui.m_labelPlayerSDKVersion->setText(strPlaySDKVersion);
        //printf("\n\r%.8x <- PlayM4_GetSdkVersion()\n\r",PlayM4_GetSdkVersion());


        GetCap(PlayM4_GetCaps());
        //bSuccess = PlayM4_GetPort((int *)&m_pblocalportnum);

        if (!PlayM4_GetPort(&m_pblocalportnum))
        {
            DisplayError("PlayM4_GetPort", PlayM4_GetLastError(m_pblocalportnum));
        }
        printf("Debug---Port:%d\n\r",m_pblocalportnum);

        bool CallBResp = false;

#if (defined(_WIN32))
        //CallBResp = PlayM4_SetFileRefCallBack(m_pblocalportnum, (void (__stdcall *)(long,char *,long,long,long,long,long,long))PlayM4DisplayCallBack);
#elif defined(__linux__)
        //int  PlayM4_SetFileRefCallBack(int nPort, void (CALLBACK *pFileRefDone)(unsigned int nPort,void* nUser),void* nUser);
        CallBResp = PlayM4_SetFileRefCallBack(m_pblocalportnum, (void (CALLBACK *)(unsigned int,void*))SetFileRefCallBack,nUser);
#endif
        if (!CallBResp){
            DisplayError("PlayM4_SetFileRefCallBack",PlayM4_GetLastError(m_pblocalportnum));
        }

        //PlayM4_SetFileEndMsg(m_pblocalportnum, hwnd,0);


        VideoCtrls::HikNumPort = m_pblocalportnum;
    }
    return;
}

void playm4interface::FreePort()
{
    if (m_pblocalportnum > -1) {

        if (!PlayM4_CloseFile(m_pblocalportnum))
        {
            DisplayError("PlayM4_CloseFile", PlayM4_GetLastError(m_pblocalportnum));
        }

        if (!PlayM4_FreePort(m_pblocalportnum))
        {
            DisplayError("PlayM4_FreePort", PlayM4_GetLastError(m_pblocalportnum));
        }

    }
    m_pblocalportnum = -1;
    return;
}

int playm4interface::RefreshPlay()
{
    if(isIndexed) {
        SetVideoWin(0);
#if (defined(__linux__))
        if (!PlayM4_WndResolutionChange(m_pblocalportnum))
        {
            DisplayError("PlayM4_WndResolutionChange",PlayM4_GetLastError(m_pblocalportnum));
        }
#endif

        /*if (!PlayM4_RefreshPlay(m_pblocalportnum))
        {
            DisplayError("PlayM4_RefreshPlay",PlayM4_GetLastError(m_pblocalportnum));
        }*/
    }
    return 0;
}

int playm4interface::SetVideoWin(unsigned int nRegionNum)
{
#if (defined(__linux__))
    return PlayM4_SetVideoWindow(m_pblocalportnum,nRegionNum,hwnd);
#endif
    return nRegionNum;

}

int playm4interface::Pause(unsigned int nPause)//nPause=1 pause, =0 resume
{
    return PlayM4_Pause(m_pblocalportnum,nPause);
}

int playm4interface::Play()
{
    bool Ret = false;

    Ret = PlayM4_Play(m_pblocalportnum,hwnd);
    if (!Ret)
    {
        DisplayError("PlayM4_Play",PlayM4_GetLastError(m_pblocalportnum));
    }else{
        RefreshPlay();
        FsOpened = true;
    }

    return Ret;
}

int playm4interface::Stop()
{
    if (FsOpened) {
        if (isound) {
            if (!PlayM4_StopSound())
            {
                DisplayError("PlayM4_StopSound", PlayM4_GetLastError(m_pblocalportnum));
            }
        }
        if (!PlayM4_Stop(m_pblocalportnum))
        {
            DisplayError("PlayM4_Stop", PlayM4_GetLastError(m_pblocalportnum));
        }

        FsOpened = false;
        isound = false;
    }
    return 1;
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
    qDebug() <<  GetVolume;
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
bool CallBResp = false;
#if (defined(_WIN32))
    CallBResp = PlayM4_SetDecCallBack(m_pblocalportnum, (void (CALLBACK *)(long,char *,long,long,long,long,long,long))SetDecCallBack);
#elif defined(__linux__)
    CallBResp = PlayM4_SetDecCallBack(m_pblocalportnum, (void (CALLBACK *)(int,char *,int,FRAME_INFO *, void*,int))SetDecCallBack);
#endif
    if (!CallBResp){
        DisplayError("PlayM4_SetDecCallBack",PlayM4_GetLastError(m_pblocalportnum));
    }
}


void playm4interface::InitCallback( unsigned int nBeginTime, unsigned int nEndTime)
{

    bool CallBResp = false;

#if (defined(_WIN32))
    //PlayM4_SetAudioCallBack
    BOOL _stdcall PlayM4_SetAudioCallBack (LONG nPort, void (_stdcall * funAudio) (long
                                                                                   nPort, char * pAudioBuf, long nSize, long nStamp, long nType, long nUser), long nUser);
    //CallBResp = PlayM4_SetAudioCallBack(m_pblocalportnum, (void (__stdcall *)(long,char *,long,long,long,long))SetAudioCallBack,0);
#elif defined(__linux__)
    //int          PlayM4_SetAudioCallBack(int nPort, void (__stdcall* funAudio)(int nPort, char* pAudioBuf, int nSize, int nStamp, int nType, int nUser), int nUser);
    CallBResp = PlayM4_SetAudioCallBack(m_pblocalportnum, (void (CALLBACK *)(int, char*, int, int, int, int))SetAudioCallBack,0);
#endif
    if (!CallBResp){
        DisplayError("PlayM4_SetAudioCallBack",PlayM4_GetLastError(m_pblocalportnum));
    }

#if (defined(_WIN32))
    //CallBResp = PlayM4_SetDecCallBack(m_pblocalportnum, (void (CALLBACK *)(long,char *,long,long,long,long,long,long))PlayM4DisplayCallBack);
#elif defined(__linux__)
    //callback work, but no sound anymore !!!
    //int  PlayM4_SetDecCallBackMend(int nPort,void (CALLBACK* DecCBFun)(int nPort,char * pBuf,int nSize,FRAME_INFO * pFrameInfo, void* nUser,int nReserved2), void* nUser);
    //or
    //int  PlayM4_SetDecCallBack(int nPort,void (CALLBACK* DecCBFun)(int nPort,char* pBuf,int nSize,FRAME_INFO * pFrameInfo, void* nReserved1,int nReserved2));
    //CallBResp = PlayM4_SetDecCallBack(m_pblocalportnum, (void (CALLBACK *)(int,char *,int,FRAME_INFO *, void*,int))SetDecCallBack);
#endif
    /*if (!CallBResp){
        DisplayError("PlayM4_SetDecCallBack",PlayM4_GetLastError(m_pblocalportnum));
    }*/


}

void CALLBACK playm4interface::SetFileRefCallBack(int nPort,void* nUser)
{
    isIndexed = true;
    qDebug()  << "Callback called, file is indexed (" << nPort << "," << nUser<< ")";

    bool CallBResp = false;

#if (defined(_WIN32))
    //CallBResp = PlayM4_SetVerifyCallBack(m_pblocalportnum, (void (__stdcall *)(long,char *,long,long,long,long,long,long))PlayM4DisplayCallBack);
#elif defined(__linux__)
    //int          PlayM4_SetVerifyCallBack(int nPort, unsigned int nBeginTime, unsigned int nEndTime, void (__stdcall* funVerify)(int nPort, FRAME_POS* pFilePos, unsigned int bIsVideo, unsigned int nUser), unsigned int nUser);
    CallBResp = PlayM4_SetVerifyCallBack(m_pblocalportnum, 0, 360,(void (CALLBACK *)(int, FRAME_POS*, unsigned int, unsigned int ))SetVerifyCallBack,0);
#endif
    if (!CallBResp){
        DisplayError("PlayM4_SetVerifyCallBack",PlayM4_GetLastError(m_pblocalportnum));
    }
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
    //printf("SetDecCallBack---%l:%l\n\r",nPort, nSize);
    //qDebug() <<  pFrameInfo;
    pFRAME_INFO = pFrameInfo;

    if ((pFRAME_INFO->nType == T_AUDIO16 | pFRAME_INFO->nType == T_AUDIO8) && !isound) {
        PlaySound();
    }

    if (pFRAME_INFO) {
    qDebug() << "FrameNum : " <<playm4interface::pFRAME_INFO->dwFrameNum;
    qDebug() << "FrameRate : " <<playm4interface::pFRAME_INFO->nFrameRate;
    qDebug() << "Width : " <<playm4interface::pFRAME_INFO->nWidth;
    qDebug() << "Height : " <<playm4interface::pFRAME_INFO->nHeight;
    qDebug() << "Stamp : " <<playm4interface::pFRAME_INFO->nStamp;
    qDebug() << "Type : " <<playm4interface::pFRAME_INFO->nType;

    }
    else
    {
        int width = 0, height = 0;
        PlayM4_GetPictureSize(playm4interface::m_pblocalportnum,&width,&height);
        qDebug()  << width << "x" << height << " <- PlayM4_GetPictureSize()" ;
    }

#if (defined(_WIN32))
    PlayM4_SetDecCallBack(m_pblocalportnum, (void (CALLBACK *)(long,char *,long,long,long,long,long,long))NULL);
#elif defined(__linux__)
    PlayM4_SetDecCallBack(m_pblocalportnum, (void (CALLBACK *)(int,char *,int,FRAME_INFO *, void*,int))NULL);
#endif
}

bool AudioSwitch = true;
void playm4interface::PlaySound()
{
    if (!AudioSwitch) {
        if (!PlayM4_PlaySound(m_pblocalportnum))
        {
            DisplayError("PlayM4_PlaySound",PlayM4_GetLastError(m_pblocalportnum));
        }
        AudioSwitch = true;
    } else {
        if (!PlayM4_StopSound())
        {
            DisplayError("PlayM4_StopSound",PlayM4_GetLastError(m_pblocalportnum));
        }
        AudioSwitch = false;
    }
    isound = true;
}
