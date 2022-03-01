#include "playm4interface.h"
#include"include/PlayM4.h"
#include "errormanager.h"

int nPort=0;
PLAYM4_HWND hwnd=0;
uint32_t playm4interface::VideoFs(QString fileName,WId HWND)
    {

    hwnd=HWND; //this->ui->centralwidget->DrawWindowBackground;
    PlayM4_GetCaps();
    nPort=PlayM4_GetPort(0);
    //PlayM4_SetFileRefCallBack();
    //PlayM4_SetVerifyCallBack();
    //PlayM4_SetFileEndMsg();
    QByteArray ba = fileName.toLocal8Bit();
    char *c_str2 = ba.data();
    PlayM4_CloseFile(nPort);
    PlayM4_OpenFile(nPort,c_str2);
    PlayM4_Play(nPort,hwnd);

    //emit DisplayError(PlayM4_GetLastError(nPort));
    return PlayM4_GetLastError(nPort);
    }

int playm4interface::RefreshPlay()
    {
    return PlayM4_RefreshPlay(nPort);
    }


int playm4interface::SetVideoWin(unsigned int nRegionNum, WId hWnd)
    {
    return PlayM4_SetVideoWindow(nPort,nRegionNum,hWnd);
    }

int playm4interface::Pause(unsigned int nPause)//nPause=1 pause, =0 resume
    {
    return PlayM4_Pause(nPort,nPause);
    }

int playm4interface::Play()
    {
    return PlayM4_Play(nPort,hwnd);
    }

int playm4interface::Stop()
    {
    return PlayM4_Stop(nPort);
    }

int playm4interface::Fast()
    {
    return PlayM4_Fast(nPort);
    }

int playm4interface::Slow()
    {
    return PlayM4_Slow(nPort);
    }

int playm4interface::OneByOne()
    {
    return PlayM4_OneByOne(nPort);
    }

int playm4interface::OneByOneBack()
    {
    return PlayM4_OneByOneBack(nPort);
    }
