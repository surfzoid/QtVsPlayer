#ifndef PLAYM4INTERFACE_H
#define PLAYM4INTERFACE_H

#include <QString>
#include <QWidget>

class playm4interface
{
public:
        //playm4interface DisplayError(uint32_t ErrMess);
        uint32_t VideoFs (QString fileName,WId HWND);
        int SetVideoWin(unsigned int nRegionNum, WId hWnd);
        int RefreshPlay();
        int Pause(unsigned int nPause);//nPause=1 pause, =0 resume
        int Play();
        int Stop();
        int Fast();
        int Slow();
        int OneByOne();
        int OneByOneBack();
};


#endif // PLAYM4INTERFACE_H
