#ifndef PLAYM4INTERFACE_H
#define PLAYM4INTERFACE_H

#include <include/PlayM4.h>
#include <QString>

namespace Ui {
class playm4interface;
}

class playm4interface
{


public:
    static PLAYM4_HWND  hwnd;
    static unsigned int  VideoFs (QString fileName);
    int SetVideoWin(unsigned int nRegionNum);
    int RefreshPlay();
    int Pause(unsigned int nPause);//nPause=1 pause, =0 resume
    int Play();
    static int Stop();
    int Fast();
    int Slow();
    int OneByOne();
    int OneByOneBack();
    static void decimaltoOctal(int deciNum)
    {

        // initializations
        int octalNum = 0, countval = 1;
        // int dNo = deciNum;

        while (deciNum != 0) {

            // decimals remainder is calculated
            int remainder = deciNum % 8;

            // storing the octalvalue
            octalNum += remainder * countval;

            // storing exponential value
            countval = countval * 10;
            deciNum /= 8;
            printf("%x <- Caps()\n\r",deciNum);
            printf("%x <- Caps()\n\r",octalNum);
        }
    }

    int * usrid = 0;
    int * channelnum = 0;
    //for play local file
    static QString m_pblocalfilepath;
    static int m_pblocalportnum;
    static void SetPort();
    static void FreePort();
private:

    QString m_pbdstarttime;
    QString m_pbdstoptime;


};


#endif // PLAYM4INTERFACE_H
