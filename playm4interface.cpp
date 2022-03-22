#include "playm4interface.h"
#include "errormanager.h"
#include "qtvsplayer.h"
#include "ui_qtvsplayer.h"
#include <include/PlayM4.h>
#include "filesliste.h"
#include "videoctrls.h"
#include <string>
#include <QtDebug>

using namespace std;



PLAYM4_HWND  playm4interface::hwnd;
int playm4interface::m_pblocalportnum = -1;
QString playm4interface::m_pblocalfilepath;

unsigned int  playm4interface::VideoFs(QString fileName)
{
    VideoCtrls().EndRead = false;
    BOOL bSuccess;

    //initval();
    m_pblocalfilepath = fileName;

    printf("Debug---File name:%s", m_pblocalfilepath.toUtf8().data());
    printf("pyd---File name:%s\n\r",m_pblocalfilepath.toUtf8().data());


    bSuccess = PlayM4_CloseFile(m_pblocalportnum);
    bSuccess= PlayM4_OpenFile(m_pblocalportnum, m_pblocalfilepath.toUtf8().data());

    bSuccess = PlayM4_Play(m_pblocalportnum, hwnd);

    if (bSuccess) {

        QtVsPlayer().SetWindowTitle(m_pblocalfilepath.toUtf8().data());
    }

    QtVsPlayer().WVideoCtrls->InitTimeSlide();


    //emit DisplayError(PlayM4_GetLastError(m_pblocalportnum));
    return PlayM4_GetLastError(m_pblocalportnum);
}

void playm4interface::SetPort()
{
    if (m_pblocalportnum == -1) {
        //FreePort();
        BOOL bSuccess;
        //from qtdemo//////
        printf("%.8x <- PlayM4_GetSdkVersion()\n\r",PlayM4_GetSdkVersion());

        decimaltoOctal(PlayM4_GetCaps());

        bSuccess = PlayM4_GetPort((int *)&m_pblocalportnum);
        printf("Debug---Port:%s",((QString)m_pblocalportnum).toUtf8().data());

        QtVsPlayer::InitPort(m_pblocalportnum);
    }
    return;
}

void playm4interface::FreePort()
{
    BOOL bSuccess;
    bSuccess = Stop();
    bSuccess = PlayM4_CloseFile(m_pblocalportnum);
    bSuccess = PlayM4_FreePort(m_pblocalportnum);
    return;
}

int playm4interface::RefreshPlay()
{
    //return PlayM4_RefreshPlay(m_pblocalportnum);
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
    return PlayM4_Stop(m_pblocalportnum);
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

    PlayM4_WndResolutionChange(m_pblocalportnum);

    return PlayM4_ReversePlay(m_pblocalportnum);
    //return PlayM4_OneByOneBack(m_pblocalportnum);
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

