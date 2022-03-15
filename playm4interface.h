#ifndef PLAYM4INTERFACE_H
#define PLAYM4INTERFACE_H

#include"include/PlayM4.h"
#include <QString>
#include <QWidget>
#include <QFrame>

class playm4interface
{


public:
        static PLAYM4_HWND  hwnd;
        //playm4interface DisplayError(unsigned int  ErrMess);
        unsigned int  VideoFs (QString fileName);
        int SetVideoWin(unsigned int nRegionNum);
        int RefreshPlay();
        int Pause(unsigned int nPause);//nPause=1 pause, =0 resume
        int Play();
        int Stop();
        int Fast();
        int Slow();
        int OneByOne();
        int OneByOneBack();
        //void (CALLBACK FileRefDone)(unsigned int nPort,void* nUser);
        void decimaltoOctal(int deciNum)
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
        QString m_pblocalfilepath;
        //play area
        QWidget *m_pbframe;
        //playM4 handle
        static int m_pblocalportnum;
private:

        void initval();
        //current user id
        int* m_pbUserID;
        //current valid channel
        int* m_pbCurrChannel;
        //qtablewidget page index
        int m_pbpageindex;
        //page index backup
        int m_pbcurrentpageindex;
        //for play
        //play handle
        int m_pbhandle;
        //file type for search filelist
        unsigned int m_pbfiletype;
        //file attr for search filelist
        unsigned int m_pbfileattr;
        //doawnload handle
        int m_pbdownloadhandle;
        //0 normal 1 alarm 2 download
        int m_pblocaltype;
        //tmp variants in 0,2 page
        QString m_pbdstarttime;
        QString m_pbdstoptime;

private slots:

};


#endif // PLAYM4INTERFACE_H
