#include "playm4interface.h"
#include"include/PlayM4.h"
#include "errormanager.h"
#include "qtvsplayer.h"
#include "ui_qtvsplayer.h"
#include <unistd.h>
#include "filesliste.h"
#include "videoctrls.h"
#include <string>
#include <QtDebug>

PLAYM4_HWND  playm4interface::hwnd;
int playm4interface::m_pblocalportnum = -1;
unsigned int  playm4interface::VideoFs(QString fileName)
{
    QtVsPlayer().WVideoCtrls->EndRead = false;
    decimaltoOctal(PlayM4_GetCaps());
    BOOL bSuccess;
    bSuccess = playm4interface::Stop();
    bSuccess = PlayM4_CloseFile(m_pblocalportnum);
    bSuccess = PlayM4_FreePort(m_pblocalportnum);

    printf("%.8x <- PlayM4_GetSdkVersion()\n\r",PlayM4_GetSdkVersion());

    initval();
    m_pblocalfilepath = fileName;
    //from qtdemo//////
    bSuccess = PlayM4_GetPort((LONG *)&m_pblocalportnum);
    QtVsPlayer().WVideoCtrls->HikNumPort = m_pblocalportnum;

    qDebug("Debug---File name:%s", m_pblocalfilepath.toUtf8().data());
    qDebug("Debug---Port:%s",(std::to_string(m_pblocalportnum)).data());

    printf("pyd---File name:%s\n\r",m_pblocalfilepath.toUtf8().data());


    bSuccess= PlayM4_OpenFile(m_pblocalportnum, m_pblocalfilepath.toUtf8().data());

    bSuccess = PlayM4_Play(m_pblocalportnum, hwnd);

    if (bSuccess) {

        QtVsPlayer().SetWindowTitle(m_pblocalfilepath.toUtf8().data());
    }

    QtVsPlayer().WVideoCtrls->InitTimeSlide();


    //emit DisplayError(PlayM4_GetLastError(m_pblocalportnum));
    return PlayM4_GetLastError(m_pblocalportnum);
}


//int m_pblocalportnum=0;
int playm4interface::RefreshPlay()
{
    return PlayM4_RefreshPlay(m_pblocalportnum);
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
    //QtVsPlayer().WinIdWorkarround();
    return PlayM4_Play(m_pblocalportnum,hwnd);
}

int playm4interface::Stop()
{
    //QtVsPlayer().WinIdWorkarround();
    return PlayM4_Stop(m_pblocalportnum);
    //return PlayM4_FreePort(m_pblocalportnum);
}

int playm4interface::Fast()
{
    qDebug("Debug---Fastest Port:%s",(std::to_string(playm4interface::m_pblocalportnum)).data());
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
    return PlayM4_OneByOneBack(m_pblocalportnum);
}

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


