#ifndef PLAYM4INTERFACE_H
#define PLAYM4INTERFACE_H

#include <include/PlayM4.h>
#include <QString>

class playm4interface
{


public:
    playm4interface();
    static PLAYM4_HWND  hwnd;
    static int FsTime;
    static unsigned int  VideoFs (QString fileName);
    static int SetVideoWin(unsigned int nRegionNum);
    static int RefreshPlay();
    static int Pause(unsigned int nPause);//nPause=1 pause, =0 resume
    static int Play();
    static int Stop();
    static int Fast();
    static int Slow();
    static int OneByOne();
    static int OneByOneBack();
    static int SetVolume(unsigned short Volume);
    static void decimaltoOctal(int deciNum)
    {

        // initializations
        static int octalNum = 0, countval = 1;
        // int dNo = deciNum;

        while (deciNum != 0) {

            // decimals remainder is calculated
            int remainder = deciNum % 8;

            // storing the octalvalue
            octalNum += remainder * countval;

            // storing exponential value
            countval = countval * 10;
            deciNum /= 8;
            printf("%x <- Caps()\r\n",deciNum);
            printf("%x <- Caps()\r\n",octalNum);
        }
        return;
    }

    //for play local file
    static QString m_pblocalfilepath;
    static int m_pblocalportnum;
    static void SetPort();
    static void FreePort();
    static void GetMetadatas();
    static FRAME_INFO * pFRAME_INFO;
    static void PlaySound();
    static void DisplayError(QString Source, unsigned int  ErrMess);

private:

    static bool FsOpened;
    static void GetCap(int nFlag);
    static void delay();
    static void InitCallback(unsigned int nBeginTime, unsigned int nEndTime);

    static void*  nUser;
    static void CALLBACK SetFileRefCallBack(int nPort,void* nUser);
    static void CALLBACK SetAudioCallBack(int nPort, char* pAudioBuf, int nSize, int nStamp, int nType, int nUser);
    static void CALLBACK SetVerifyCallBack(int nPort, FRAME_POS* pFilePos, unsigned int bIsVideo, unsigned int nUser);
    static void CALLBACK SetDecCallBack(int nPort,char * pBuf,int nSize,FRAME_INFO * pFrameInfo, void* nUser,int nReserved2);
    static int nModule;
    static void CALLBACK SetRunTimeInfoCBFun(int nPort, RunTimeInfo* pstRunTimeInfo, void* pUser);

};

#endif // PLAYM4INTERFACE_H
