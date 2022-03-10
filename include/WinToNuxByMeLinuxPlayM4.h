#ifndef _PLAYM4_H_
#define _PLAYM4_H_

#ifdef __cplusplus
    extern "C"
    {
#endif

typedef unsigned int PLAYM4_HWND;
typedef void * PLAYM4_HDC;


#define PLAYM4_API

#define __stdcall

#ifndef CALLBACK
#define CALLBACK
#endif

//Max channel numbers
#define PLAYM4_MAX_SUPPORTS 500
//Wave coef range;
#define MIN_WAVE_COEF -100
#define MAX_WAVE_COEF 100

//Timer type
#define TIMER_1 1 //Only 16 timers for every process.Default TIMER;
#define TIMER_2 2 //Not limit;But the precision less than TIMER_1;

//BUFFER AND DATA TYPE
#define BUF_VIDEO_SRC               (1) //mixed input,total src buffer size;splited input,video src buffer size
#define BUF_AUDIO_SRC               (2) //mixed input,not defined;splited input,audio src buffer size
#define BUF_VIDEO_RENDER            (3) //video render node count or node cout for decoded data
#define BUF_AUDIO_RENDER            (4) //audio render node count
#define BUF_VIDEO_DECODED           (5) //video decoded node count to render
#define BUF_AUDIO_DECODED           (6) //audio decoded node count to render

//Error code
#define  PLAYM4_NOERROR                            0    //no error
#define     PLAYM4_PARA_OVER                        1    //input parameter is invalid;
#define  PLAYM4_ORDER_ERROR                        2    //The order of the function to be called is error.
#define     PLAYM4_TIMER_ERROR                        3    //Create multimedia clock failed;
#define  PLAYM4_DEC_VIDEO_ERROR                    4    //Decode video data failed.
#define  PLAYM4_DEC_AUDIO_ERROR                    5    //Decode audio data failed.
#define     PLAYM4_ALLOC_MEMORY_ERROR                6    //Allocate memory failed.
#define  PLAYM4_OPEN_FILE_ERROR                    7    //Open the file failed.
#define  PLAYM4_CREATE_OBJ_ERROR                8    //Create thread or event failed
#define  PLAYM4_CREATE_DDRAW_ERROR                9    //Create DirectDraw object failed.
#define  PLAYM4_CREATE_OFFSCREEN_ERROR            10    //failed when creating off-screen surface.
#define  PLAYM4_BUF_OVER                        11    //buffer is overflow
#define  PLAYM4_CREATE_SOUND_ERROR                12    //failed when creating audio device.
#define     PLAYM4_SET_VOLUME_ERROR                13    //Set volume failed
#define  PLAYM4_SUPPORT_FILE_ONLY                14    //The function only support play file.
#define  PLAYM4_SUPPORT_STREAM_ONLY                15    //The function only support play stream.
#define  PLAYM4_SYS_NOT_SUPPORT                    16    //System not support.
#define  PLAYM4_FILEHEADER_UNKNOWN                17    //No file header.
#define  PLAYM4_VERSION_INCORHKRECT                18    //The version of decoder and encoder is not adapted.
#define  PLAYM4_INIT_DECODER_ERROR                19    //Initialize decoder failed.
#define  PLAYM4_CHECK_FILE_ERROR                20    //The file data is unknown.
#define  PLAYM4_INIT_TIMER_ERROR                21    //Initialize multimedia clock failed.
#define     PLAYM4_BLT_ERROR                        22    //Blt failed.
#define  PLAYM4_UPDATE_ERROR                    23    //Update failed.
#define  PLAYM4_OPEN_FILE_ERROR_MULTI            24   //openfile error, streamtype is multi
#define  PLAYM4_OPEN_FILE_ERROR_VIDEO            25   //openfile error, streamtype is video
#define  PLAYM4_JPEG_COMPRESS_ERROR                26   //JPEG compress error
#define  PLAYM4_EXTRACT_NOT_SUPPORT                27    //Don't support the version of this file.
#define  PLAYM4_EXTRACT_DATA_ERROR                28    //extract video data failed.
#define  PLAYM4_SECRET_KEY_ERROR                29    //Secret key is error //add 20071218
#define  PLAYM4_DECODE_KEYFRAME_ERROR            30   //add by hy 20090318
#define  PLAYM4_NEED_MORE_DATA                    31   //add by hy 20100617
#define  PLAYM4_INVALID_PORT                    32    //add by cj 20100913
#define  PLAYM4_NOT_FIND                        33    //add by cj 20110428
#define  PLAYM4_NEED_LARGER_BUFFER              34  //add by pzj 20130528
#define  PLAYM4_FAIL_UNKNOWN                    99   //Fail, but the reason is unknown;

//Fisheye error code
#define PLAYM4_FEC_ERR_ENABLEFAIL                100 // Fisheye module loading failed
#define PLAYM4_FEC_ERR_NOTENABLE                101 // Fisheye model not enabled
#define PLAYM4_FEC_ERR_NOSUBPORT                102 // Sub-port is not assigned
#define PLAYM4_FEC_ERR_PARAMNOTINIT                103 // Parameter not initialed for this port
#define PLAYM4_FEC_ERR_SUBPORTOVER                104 // Sub-port used up
#define PLAYM4_FEC_ERR_EFFECTNOTSUPPORT            105 // Not supported in the current installation mode
#define PLAYM4_FEC_ERR_INVALIDWND                106 // Invalid window
#define PLAYM4_FEC_ERR_PTZOVERFLOW                107 // PTZ location out of bounds
#define PLAYM4_FEC_ERR_RADIUSINVALID            108 // Invalid image centre parameter
#define PLAYM4_FEC_ERR_UPDATENOTSUPPORT            109 // Parameter update is not supported in the current installation mode
#define PLAYM4_FEC_ERR_NOPLAYPORT                110 // Port number has not been enabled
#define PLAYM4_FEC_ERR_PARAMVALID                111 // Null parameter
#define PLAYM4_FEC_ERR_INVALIDPORT                112 // Invalid sub-port
#define PLAYM4_FEC_ERR_PTZZOOMOVER                113 // PTZ correction out of bounds
#define PLAYM4_FEC_ERR_OVERMAXPORT                114  // Correction port number exceeds the top limit (up to 4)
#define PLAYM4_FEC_ERR_ENABLED                  115  //Fisheye already enabled on this port
#define PLAYM4_FEC_ERR_D3DACCENOTENABLE            116 // Direct 3D acceleration not enabled


//Max display regions.
#define MAX_DISPLAY_WND 4

//Display type
#define DISPLAY_NORMAL                0x00000001
#define DISPLAY_QUARTER                0x00000002
#define DISPLAY_YC_SCALE            0x00000004    //add by gb 20091116
#define DISPLAY_NOTEARING            0x00000008
//Display buffers
#define MAX_DIS_FRAMES 50
#define MIN_DIS_FRAMES 1

//Locate by
#define BY_FRAMENUM  1
#define BY_FRAMETIME 2

//Source buffer
#define SOURCE_BUF_MAX    1024*100000
#define SOURCE_BUF_MIN    1024*50

//Stream type
#define STREAME_REALTIME 0
#define STREAME_FILE     1

//frame type
#define T_AUDIO16    101
#define T_AUDIO8    100
#define T_UYVY        1
#define T_YV12        3
#define T_RGB32        7

//capability
#define SUPPORT_DDRAW        1
#define SUPPORT_BLT         2
#define SUPPORT_BLTFOURCC   4
#define SUPPORT_BLTSHRINKX  8
#define SUPPORT_BLTSHRINKY  16
#define SUPPORT_BLTSTRETCHX 32
#define SUPPORT_BLTSTRETCHY 64
#define SUPPORT_SSE         128
#define SUPPORT_MMX            256

// the microdefine use for the struct HIK_MEDIAINFO
#define FOURCC_HKMI            0x484B4D49    //  the structer flag of "HKMI" HIK_MEDIAINFO
// system pack type
#define SYSTEM_NULL            0x0                // have no system packet ,raw audio data or raw video data
#define SYSTEM_HIK          0x1                // hik file pack
#define SYSTEM_MPEG2_PS     0x2                // PS pack
#define SYSTEM_MPEG2_TS     0x3                // TS pack
#define SYSTEM_RTP          0x4                // rtp pack
#define SYSTEM_RTPHIK       0x401                // rtp pack

// video encode type
#define VIDEO_NULL          0x0 // no video
#define VIDEO_H264          0x1 // hik H.264
#define VIDEO_MPEG4         0x3 // standrand MPEG4
#define VIDEO_MJPEG            0x4
#define VIDEO_AVC264        0x0100

// audio encode type
#define AUDIO_NULL          0x0000 // no audio
#define AUDIO_ADPCM         0x1000 // ADPCM
#define AUDIO_MPEG          0x2000 // MPEG series audio,adaptive various MPEG audio decoder
#define AUDIO_AAC            0X2001 // AAC encode type
// G series audio
#define AUDIO_RAW_DATA8        0x7000      //the raw data of the sample rate is 8K
#define AUDIO_RAW_UDATA16    0x7001      //the raw data of the sample rate is 16K ,named L16
#define AUDIO_G711_U        0x7110
#define AUDIO_G711_A        0x7111
#define AUDIO_G722_1        0x7221
#define AUDIO_G723_1        0x7231
#define AUDIO_G726_U        0x7260
#define AUDIO_G726_A        0x7261
#define AUDIO_G726_16       0x7262
#define AUDIO_G729          0x7290
#define AUDIO_AMR_NB        0x3000

#define SYNCDATA_VEH        1 //synchronous data:vehicle information
#define SYNCDATA_IVS        2 //synchronous dta:intelligent information

//motion flow type
#define    MOTION_FLOW_NONE            0
#define MOTION_FLOW_CPU                1
#define MOTION_FLOW_GPU                2

//video/audio encrypt type
#define ENCRYPT_AES_3R_VIDEO     1
#define ENCRYPT_AES_10R_VIDEO    2
#define ENCRYPT_AES_3R_AUDIO     1
#define ENCRYPT_AES_10R_AUDIO    2

    typedef struct tagSystemTime
    {
        short wYear;
        short wMonth;
        short wDayOfWeek;
        short wDay;
        short wHour;
        short wMinute;
        short wSecond;
        short wMilliseconds;
    }SYSTEMTIME;
    typedef struct tagHKRect
    {
        unsigned long left;
        unsigned long top;
        unsigned long right;
        unsigned long bottom;
    }HKRECT;
//Frame position
typedef struct{
    long nFilePos;
    long nFrameNum;
    long nFrameTime;
    long nErrorFrameNum;
    SYSTEMTIME *pErrorTime;
    long nErrorLostFrameNum;
    long nErrorFrameSize;
}FRAME_POS,*PFRAME_POS;

//Frame Info
typedef struct{
    long nWidth;
    long nHeight;
    long nStamp;
    long nType;
    long nFrameRate;
    unsigned int dwFrameNum;
}FRAME_INFO;

typedef struct
{
    long         nPort;        //channel
    char         *pBuf;        //Pointer to the one channel image data returned
    unsigned int nBufLen;      //Back to the one image data siz
    char         *pBuf1;       //Pointer to the two channel image data returned
    unsigned int nBufLen1;     //Back to the two image data siz
    char         *pBuf2;       //Pointer to the third channel image data returned
    unsigned int nBufLen2;     //Back to the third image data size
    unsigned int nWidth;       //width
    unsigned int nHeight;      //height
    unsigned int nStamp;       //Time scale information, in milliseconds
    unsigned int nType;        //data type
    void         *pUser;       //uer
    unsigned int reserved[4];
}DISPLAY_INFO_YUV;

//Frame
typedef struct{
    char *pDataBuf;
    long  nSize;
    long  nFrameNum;
    int  bIsAudio;
    long  nReserved;
}FRAME_TYPE;

//Watermark Info    //add by gb 080119
typedef struct{
    char *pDataBuf;
    long  nSize;
    long  nFrameNum;
    int  bRsaRight;
    long  nReserved;
}WATERMARK_INFO;

typedef struct SYNCDATA_INFO
{
    unsigned int dwDataType;        //the ancillary information types,and stream data synchronization at present:intelligent information,vehicle information
    unsigned int dwDataLen;        //the ancillary information data length
    unsigned char* pData;            //the pointer to ancillary information data structure,such as the IVS_INFO structure
} SYNCDATA_INFO;

#ifndef _HIK_MEDIAINFO_FLAG_
#define _HIK_MEDIAINFO_FLAG_
typedef struct _HIK_MEDIAINFO_                // modified by gb 080425
{
    unsigned int    media_fourcc;            // "HKMI": 0x484B4D49 Hikvision Media Information
    unsigned short  media_version;            // Version:the version number of the information structure ;0x0101,is the 1.01 version,01: the major version number;01:sub version number
    unsigned short  device_id;                // the ID of the device,to facilitate tracking/analysis

    unsigned short  system_format;          // the system format type
    unsigned short  video_format;           // the video encode type

    unsigned short  audio_format;           // the audio encode type
    unsigned char   audio_channels;         // channels
    unsigned char   audio_bits_per_sample;  // sample rate
    unsigned int    audio_samplesrate;      // sampling rate
    unsigned int    audio_bitrate;          // compressed audio bitrate ,unit: bit

    unsigned int    reserved[4];            // reserved
}HIK_MEDIAINFO;
#endif

typedef struct
{
    long nPort;
    char * pBuf;
    long nBufLen;
    long nWidth;
    long nHeight;
    long nStamp;
    long nType;
    long nUser;
}DISPLAY_INFO;

typedef struct
{
    long nPort;
    char *pVideoBuf;
    long nVideoBufLen;
    char *pPriBuf;
    long nPriBufLen;
    long nWidth;
    long nHeight;
    long nStamp;
    long nType;
    long nUser;
}DISPLAY_INFOEX;

typedef struct PLAYM4_SYSTEM_TIME //absolute time
{
    unsigned int dwYear;    //year
    unsigned int dwMon;    //month
    unsigned int dwDay;    //day
    unsigned int dwHour;    //hour
    unsigned int dwMin;    //minute
    unsigned int dwSec;    //second
    unsigned int dwMs;        //millisecond
} PLAYM4_SYSTEM_TIME;

//ENCRYPT Info
typedef struct{ //video encryptType
    long nAudioEncryptType;
    long nSetSecretKey;
}ENCRYPT_INFO;

//////////////////////////////////////////////////////////////////////////////
//API
//////////////////////////////////////////////////////////////////////////////

////////////////ver 1.0///////////////////////////////////////
//Initialize DirecDraw.Now invalid.
int  PlayM4_InitDDraw(PLAYM4_HWND hWnd);
//Release directDraw; Now invalid.
int PlayM4_RealeseDDraw();
int PlayM4_OpenFile(int nPort,char * sFileName);
int PlayM4_CloseFile(int nPort);
int PlayM4_Play(int nPort, PLAYM4_HWND hWnd);
int PlayM4_Stop(int nPort);
int PlayM4_Pause(int nPort,unsigned int nPause);
int PlayM4_Fast(int nPort);
int PlayM4_Slow(int nPort);
int PlayM4_OneByOne(int nPort);
int PlayM4_SetPlayPos(int nPort,float fRelativePos);
float PlayM4_GetPlayPos(int nPort);
int PlayM4_SetFileEndMsg(int nPort,PLAYM4_HWND hWnd,unsigned int nMsg);
int PlayM4_SetVolume(int nPort,unsigned short nVolume);
int PlayM4_StopSound();
int PlayM4_PlaySound(int nPort);
int PlayM4_OpenStream(int nPort,unsigned char pFileHeadBuf,unsigned int nSize,unsigned int nBufPoolSize);
int PlayM4_InputData(int nPort,unsigned char pBuf,unsigned int nSize);
int PlayM4_CloseStream(int nPort);
int PlayM4_GetCaps();
unsigned intPlayM4_GetFileTime(int nPort);
unsigned intPlayM4_GetPlayedTime(int nPort);
unsigned intPlayM4_GetPlayedFrames(int nPort);

//23
////////////////ver 2.0 added///////////////////////////////////////
int    PlayM4_SetDecCallBack(int nPort,void (CALLBACK* DecCBFun)(long nPort,char * pBuf,long nSize,FRAME_INFO * pFrameInfo, long nReserved1,long nReserved2));
int    PlayM4_SetDisplayCallBackYUV(int nPort, void (CALLBACK* DisplayCBFun)(DISPLAY_INFO_YUV *pstDisplayInfo), int bTrue, void* pUser);
int    PlayM4_SetDisplayCallBack(int nPort,void (CALLBACK* DisplayCBFun)(long nPort,char * pBuf,long nSize,long nWidth,long nHeight,long nStamp,long nType,long nReserved));
int    PlayM4_ConvertToBmpFile(char * pBuf,long nSize,long nWidth,long nHeight,long nType,char *sFileName);
unsigned int   PlayM4_GetFileTotalFrames(int nPort);
unsigned int   PlayM4_GetCurrentFrameRate(int nPort);
unsigned int   PlayM4_GetPlayedTimeEx(int nPort);
int    PlayM4_SetPlayedTimeEx(int nPort,unsigned int nTime);
unsigned int   PlayM4_GetCurrentFrameNum(int nPort);
int    PlayM4_SetStreamOpenMode(int nPort,unsigned int nMode);
unsigned int   PlayM4_GetFileHeadLength();
unsigned int   PlayM4_GetSdkVersion();
//11
////////////////ver 2.2 added///////////////////////////////////////
unsigned int PlayM4_GetLastError(int nPort);
int PlayM4_RefreshPlay(int nPort);
//int PlayM4_SetOverlayMode(int nPort,int bOverlay,COLORREF colorKey);
int PlayM4_GetPictureSize(int nPort,int *pWidth,int *pHeight);
int PlayM4_SetPicQuality(int nPort,int bHighQuality);
int PlayM4_PlaySoundShare(int nPort);
int PlayM4_StopSoundShare(int nPort);
//7
////////////////ver 2.4 added///////////////////////////////////////
int PlayM4_GetStreamOpenMode(int nPort);
int PlayM4_GetOverlayMode(int nPort);
//PLAYM4_API COLORREF __stdcall PlayM4_GetColorKey(int nPort);
unsigned shortPlayM4_GetVolume(int nPort);
int PlayM4_GetPictureQuality(int nPort,int *bHighQuality);
unsigned intPlayM4_GetSourceBufferRemain(int nPort);
int PlayM4_ResetSourceBuffer(int nPort);
int PlayM4_SetSourceBufCallBack(int nPort,unsigned int nThreShold,void (CALLBACK * SourceBufCallBack)(long nPort,unsigned int nBufSize,unsigned int dwUser,void*pResvered),unsigned int dwUser,void *pReserved);
int PlayM4_ResetSourceBufFlag(int nPort);
int PlayM4_SetDisplayBuf(int nPort,unsigned int nNum);
unsigned intPlayM4_GetDisplayBuf(int nPort);
int PlayM4_OneByOneBack(int nPort);
int PlayM4_SetFileRefCallBack(int nPort, void (__stdcall *pFileRefDone)(unsigned int nPort,unsigned int nUser),unsigned int nUser);
int PlayM4_SetCurrentFrameNum(int nPort,unsigned int nFrameNum);
int PlayM4_GetKeyFramePos(int nPort,unsigned int nValue, unsigned int nType, PFRAME_POS pFramePos);
int PlayM4_GetNextKeyFramePos(int nPort,unsigned int nValue, unsigned int nType, PFRAME_POS pFramePos);
#if (WINVER >= 0x0400)
//Note: These funtion must be builded under win2000 or above with Microsoft Platform sdk.
//        You can download the sdk from "http://www.microsoft.com/msdownload/platformsdk/sdkupdate/";
int PlayM4_InitDDrawDevice();
PLAYM4_API void __stdcall PlayM4_ReleaseDDrawDevice();
unsigned intPlayM4_GetDDrawDeviceTotalNums();
int PlayM4_SetDDrawDevice(int nPort,unsigned int nDeviceNum);
//int PlayM4_GetDDrawDeviceInfo(unsigned int nDeviceNum,char *  lpDriverDescription,unsigned int nDespLen,char * lpDriverName ,unsigned int nNameLen,HMONITOR *hhMonitor);
PLAYM4_API int  __stdcall  PlayM4_GetCapsEx(unsigned int nDDrawDeviceNum);
#endif
int PlayM4_ThrowBFrameNum(int nPort,unsigned int nNum);
//23
////////////////ver 2.5 added///////////////////////////////////////
int PlayM4_SetDisplayType(int nPort,int nType);
int PlayM4_GetDisplayType(int nPort);
//2
////////////////ver 3.0 added///////////////////////////////////////
int PlayM4_SetDecCBStream(int nPort,unsigned int nStream);
int PlayM4_SetDisplayRegion(int nPort,unsigned int nRegionNum, HKRECT *pSrcRect, PLAYM4_HWND hDestWnd, int bEnable);
int PlayM4_RefreshPlayEx(int nPort,unsigned int nRegionNum);
#if (WINVER >= 0x0400)
//Note: The funtion must be builded under win2000 or above with Microsoft Platform sdk.
//        You can download the sdk from http://www.microsoft.com/msdownload/platformsdk/sdkupdate/;
int PlayM4_SetDDrawDeviceEx(int nPort,unsigned int nRegionNum,unsigned int nDeviceNum);
#endif
//4
/////////////////v3.2 added/////////////////////////////////////////

int PlayM4_GetRefValue(int nPort,unsigned char *pBuffer, unsigned int *pSize);
int PlayM4_SetRefValue(int nPort,unsigned char *pBuffer, unsigned int nSize);
int PlayM4_OpenStreamEx(int nPort,unsigned char pFileHeadBuf,unsigned int nSize,unsigned int nBufPoolSize);
int PlayM4_CloseStreamEx(int nPort);
int PlayM4_InputVideoData(int nPort,unsigned char pBuf,unsigned int nSize);
int PlayM4_InputAudioData(int nPort,unsigned char pBuf,unsigned int nSize);
int PlayM4_RegisterDrawFun(int nPort,void (CALLBACK* DrawFun)(long nPort,PLAYM4_HDC hDc,int nUser),int nUser);
int PlayM4_RigisterDrawFun(int nPort,void (CALLBACK* DrawFun)(long nPort,PLAYM4_HDC hDc,int nUser),int nUser);
//8
//////////////////v3.4/////////////////////////////////////////////////////
int PlayM4_SetTimerType(int nPort,unsigned int nTimerType,unsigned int nReserved);
int PlayM4_GetTimerType(int nPort,unsigned int *pTimerType,unsigned int *pReserved);
int PlayM4_ResetBuffer(int nPort,unsigned int nBufType);
unsigned intPlayM4_GetBufferValue(int nPort,unsigned int nBufType);

//////////////////V3.6/////////////////////////////////////////////////////////
int PlayM4_AdjustWaveAudio(int nPort,int nCoefficient);
int PlayM4_SetVerifyCallBack(int nPort, unsigned int nBeginTime, unsigned int nEndTime, void (__stdcall * funVerify)(long nPort, FRAME_POS * pFilePos, unsigned int bIsVideo, unsigned int nUser),  unsigned int nUser);
int PlayM4_SetAudioCallBack(int nPort, void (__stdcall * funAudio)(long nPort, char * pAudioBuf, long nSize, long nStamp, long nType, long nUser), long nUser);
int PlayM4_SetEncTypeChangeCallBack(int nPort,void(CALLBACK *funEncChange)(long nPort,long nUser),long nUser);
int PlayM4_SetColor(int nPort, unsigned int nRegionNum, int nBrightness, int nContrast, int nSaturation, int nHue);
int PlayM4_GetColor(int nPort, unsigned int nRegionNum, int *pBrightness, int *pContrast, int *pSaturation, int *pHue);
int PlayM4_SetEncChangeMsg(int nPort,PLAYM4_HWND hWnd,unsigned int nMsg);
int PlayM4_GetOriginalFrameCallBack(int nPort, int bIsChange,int bNormalSpeed,long nStartFrameNum,long nStartStamp,long nFileHeader,void(CALLBACK *funGetOrignalFrame)(long nPort,FRAME_TYPE *frameType, long nUser),long nUser);
int PlayM4_GetFileSpecialAttr(int nPort, unsigned int *pTimeStamp,unsigned int *pFileNum ,unsigned int *pReserved);
unsigned intPlayM4_GetSpecialData(int nPort);
int PlayM4_SetCheckWatermarkCallBack(int nPort,void(CALLBACK* funCheckWatermark)(long nPort,WATERMARK_INFO* pWatermarkInfo,unsigned int nUser),unsigned int nUser);
int PlayM4_SetImageSharpen(int nPort,unsigned int nLevel);
int PlayM4_SetDecodeFrameType(int nPort,unsigned int nFrameType);
int PlayM4_SetPlayMode(int nPort,int bNormal);
int PlayM4_SetOverlayFlipMode(int nPort,int bTrue);
//unsigned intPlayM4_GetAbsFrameNum(int nPort);

//////////////////V4.7.0.0//////////////////////////////////////////////////////
////convert yuv to jpeg
int PlayM4_ConvertToJpegFile(char * pBuf,long nSize,long nWidth,long nHeight,long nType,char *sFileName);
int PlayM4_SetJpegQuality(long nQuality);
//set deflash
int PlayM4_SetDeflash(int nPort,int bDefalsh);
//int PlayM4_SetDecCallBackEx(int nPort,void (CALLBACK* DecCBFun)(long nPort,char * pBuf,long nSize,FRAME_INFO * pFrameInfo, long nReserved1,long nReserved2), char* pDest, long nDestSize);
//////////////////V4.8.0.0/////////////////////////////////////////////////////////
//check discontinuous frame number as error data?
int PlayM4_CheckDiscontinuousFrameNum(int nPort, int bCheck);
//get bmp or jpeg
int PlayM4_GetBMP(int nPort,unsigned char pBitmap,unsigned int nBufSize,unsigned int* pBmpSize);
int PlayM4_GetJPEG(int nPort,unsigned char pJpeg,unsigned int nBufSize,unsigned int* pJpegSize);
//dec call back mend
int PlayM4_SetDecCallBackMend(int nPort,void (CALLBACK* DecCBFun)(long nPort,char * pBuf,long nSize,FRAME_INFO * pFrameInfo, long nUser,long nReserved2), long nUser);
int PlayM4_SetSecretKey(int nPort, int lKeyType, char *pSecretKey, int lKeyLen);

// add by gb 2007-12-23
int PlayM4_SetFileEndCallback(int nPort, void(CALLBACK*FileEndCallback)(long nPort, void *pUser), void *pUser);

// add by gb 080131 version 4.9.0.1
int PlayM4_GetPort(int* nPort);
int PlayM4_FreePort(int nPort);
int PlayM4_SetDisplayCallBackEx(int nPort,void (CALLBACK* DisplayCBFun)(DISPLAY_INFO *pstDisplayInfo), long nUser);
int PlayM4_SkipErrorData(int nPort, int bSkip);
int PlayM4_SetDecCallBackExMend(int nPort, void (CALLBACK* DecCBFun)(long nPort, char* pBuf, long nSize, FRAME_INFO* pFrameInfo,
                                                      long nUser, long nReserved2), char* pDest, long nDestSize, long nUser);
//reverse play add by chenjie 110609
int PlayM4_ReversePlay(int nPort);
int PlayM4_GetSystemTime(int nPort, PLAYM4_SYSTEM_TIME *pstSystemTime);

//int PlayM4_SetDecodeERC(long nPort, unsigned int nLevel);

#ifndef PLAYM4_SESSION_INFO_TAG
#define PLAYM4_SESSION_INFO_TAG
//nProtocolType
#define PLAYM4_PROTOCOL_RTSP    1
//nSessionInfoType
#define PLAYM4_SESSION_INFO_SDP 1

typedef struct _PLAYM4_SESSION_INFO_     //SessionInfo structure
{
      int            nSessionInfoType;   //Session Info type, e.x. SDP
      int            nSessionInfoLen;    //Data length of Session Info
      unsigned char* pSessionInfoData;   //Session Info data

} PLAYM4_SESSION_INFO;
#endif

int PlayM4_OpenStreamAdvanced(int nPort, int nProtocolType, PLAYM4_SESSION_INFO* pstSessionInfo, unsigned int nBufPoolSize);

#define R_ANGLE_0   -1  //No rotation
#define R_ANGLE_L90  0  //Rotate left by 90 degrees
#define R_ANGLE_R90  1  //Rotate right by 90 degrees
#define R_ANGLE_180  2  //Rotate 180 degrees

int PlayM4_SetRotateAngle(int nPort, unsigned int nRegionNum, unsigned int dwType);

#ifndef PLAYM4_ADDITION_INFO_TAG
#define PLAYM4_ADDITION_INFO_TAG
typedef struct _PLAYM4_ADDITION_INFO_     //additional data structrues
{
    unsigned char*   pData;            //the attachment data
    unsigned int   dwDatalen;        //the length of the attachment data
    unsigned int    dwDataType;        //the data type
    unsigned int    dwTimeStamp;    //the relative time stamp
} PLAYM4_ADDITION_INFO;
#endif

//dwGroupIndex temporary agreed value 0~3,the first version to cancel the synchronization only withe colsetream treatment
int PlayM4_SetSycGroup(int nPort, unsigned int dwGroupIndex);
//ont to realize this function ,the starting time withe a group of seetings is not the same,withe a minuimun of time sa the starting point of the sampe group play,only a road
int PlayM4_SetSycStartTime(int nPort, PLAYM4_SYSTEM_TIME *pstSystemTime);


// the followint API of the fisheye related :
#ifndef FISH_EYE_TAG
#define FISH_EYE_TAG

// place type
typedef enum tagFECPlaceType
{
    FEC_PLACE_WALL    = 0x1,        // Wall-mount        (normal level)
    FEC_PLACE_FLOOR   = 0x2,        // Desktop-mount        (normal upwaed)
    FEC_PLACE_CEILING = 0x3,        // Ceiling-mount        (normal down)

}FECPLACETYPE;

typedef enum tagFECCorrectType
{
    FEC_CORRECT_NULL = 0x0, // No dewarping
    FEC_CORRECT_PTZ = 0x100, // PTZ
    FEC_CORRECT_180 = 0x200, // 180 degree dewarping (corresponds to 2P)
    FEC_CORRECT_360 = 0x300, // 360 panoramic dewarping (corresponds to 1P£©
    FEC_CORRECT_LAT = 0x400         // Latitude Expansion

}FECCORRECTTYPE;


typedef struct tagCycleParam
{
    float    fRadiusLeft;    // Left coordinate of circular image
    float    fRadiusRight;    // Right coordinate of circular image
    float   fRadiusTop;        // Top coordinate of circular image
    float   fRadiusBottom;    // Bottom coordinate of circular image

}CYCLEPARAM;

typedef struct tagPTZParam
{
    float fPTZPositionX;        // the center of PTZ, X coordinates
    float fPTZPositionY;        // the center of PTZ, Y coordinates

}PTZPARAM;


// error code
/*********************************************


 ********************************************/


// update flag variable definitions


#define         FEC_UPDATE_RADIUS             0x1
#define         FEC_UPDATE_PTZZOOM             0x2
#define         FEC_UPDATE_WIDESCANOFFSET     0x4
#define         FEC_UPDATE_PTZPARAM             0x8


typedef struct tagFECParam
{


    unsigned int     nUpDateType;            // Update type

    unsigned int    nPlaceAndCorrect;        // Sum of installation and correction mode   (read ¨Conly, invalid under SetParam mode)

    PTZPARAM        stPTZParam;                // Parameter for PTZ correction

    CYCLEPARAM        stCycleParam;            // Centre info of fisheye image

    float            fZoom;                    //PTZ display range

    float            fWideScanOffset;        // Offset angle for 180¡ã or 360¡ã correction

    int                nResver[16];            // Reserved

}FISHEYEPARAM;

typedef void (__stdcall * FISHEYE_CallBack )( void* pUser  , unsigned int  nSubPort , unsigned int nCBType , void * hDC ,   unsigned int nWidth , unsigned int nHeight);

#endif
// 6.121    Enable Fisheye APIs
int PlayM4_FEC_Enable(int nPort);

// 6.122    Disable Fisheye APIs
int PlayM4_FEC_Disable(int nPort);

// Get Sub-port for Fisheye Correction [1~31]
int PlayM4_FEC_GetPort(int nPort , unsigned int* nSubPort , FECPLACETYPE emPlaceType , FECCORRECTTYPE emCorrectType);

// 6.124    Delete Sub-port for Fisheye Correction
int PlayM4_FEC_DelPort(int nPort , unsigned int nSubPort);

// 6.125    Set Fisheye Correction Parameters
int PlayM4_FEC_SetParam(int nPort , unsigned int nSubPort , FISHEYEPARAM * pPara);

// 6.126    Get Fisheye Correction Parameters
int PlayM4_FEC_GetParam(int nPort , unsigned int nSubPort , FISHEYEPARAM * pPara);

// Set Display Window
int PlayM4_FEC_SetWnd(int nPort , unsigned int nSubPort , void * hWnd);

//Set DDraw Callback for Fisheye Window
int PlayM4_FEC_SetCallBack(int nPort , unsigned int nSubPort , FISHEYE_CallBack cbFunc , void * pUser);

//motionflow
int PlayM4_MotionFlow(int nPort, unsigned int dwAdjustType);

//Unrecommended APIs
/////////////////////////////////////////////////////////////////////////////////////////////
//int  PlayM4_RigisterDrawFun(int nPort,void (CALLBACK* DrawFun)(int nPort,PLAYM4_HDC hDc,void* nUser),void* nUser);
//int  PlayM4_SetDecCallBack(int nPort,void (CALLBACK* DecCBFun)(int nPort,char* pBuf,int nSize,FRAME_INFO * pFrameInfo, void* nReserved1,int nReserved2));
int  PlayM4_SetDecCallBackEx(int nPort,void (CALLBACK* DecCBFun)(int nPort,char * pBuf,int nSize,FRAME_INFO * pFrameInfo, void* nReserved1,int nReserved2), char* pDest, int nDestSize);
int  PlayM4_SetVideoWindow(int nPort, unsigned int nRegionNum, PLAYM4_HWND hWnd);
int  PlayM4_SetColor(int nPort, unsigned int nRegionNum, int nBrightness, int nContrast, int nSaturation, int nHue);
int  PlayM4_GetColor(int nPort, unsigned int nRegionNum, int* pBrightness, int* pContrast, int* pSaturation, int* pHue);
int  PlayM4_GetFileTimeEx(int nPort, unsigned int* pStart, unsigned int* pStop, unsigned int* pRev);
int  PlayM4_GetCurrentFrameRateEx(int nPort, float* pfFrameRate);
int  PlayM4_SyncToAudio(int nPort, int bSyncToAudio);
int  PlayM4_SetImageSharpen(int nPort, unsigned int nLevel);
int  PlayM4_ResetSourceBufFlag(int nPort);
//int  PlayM4_SetSourceBufCallBack(int nPort, unsigned int nThreShold, void (CALLBACK* SourceBufCallBack)(int nPort, unsigned int nBufSize, unsigned int dwUser, void* pResvered), unsigned int dwUser, void* pReserved);
unsigned int PlayM4_GetAbsFrameNum(int nPort);
//PLAYM4_API int __stdcall PlayM4_SetRunTimeInfoCallBack(int nPort, void (CALLBACK* RunTimeInfoCBFun)(int nPort, RunTimeInfo* pstRunTimeInfo, void* pUser), void* pUser);

/////////////////////////////////////////////////////////////////////////////////////////////


//about video enhancement
#ifndef PLAYM4_HIKVIE_TAG
#define PLAYM4_HIKVIE_TAG

typedef struct _PLAYM4_VIE_DYNPARAM_
{
    int moduFlag;      //the enable module,defined in the PLAYM4_VIE_MODULES such as PLAYM4_VIE_MODU_ADJ | PLAYM4_VIE_MODU_EHAN,module is enabled ,you must set the corresponding parameters

    //PLAYM4_VIE_MODU_ADJ
    int brightVal;     //brightness value,the range is[-255, 255]
    int contrastVal;   //contrast value,the range is[-256, 255]
    int colorVal;      //color value,the range is [-256, 255]
    //PLAYM4_VIE_MODU_EHAN
    int toneScale;     //filter scale,the range is[0, 100]
    int toneGain;      //contrast adjustment,the global contrast gain  value,the range is [-256, 255]
    int toneOffset;    //adjust brightness,the offset of the bright  average ,the range is[-255, 255] range is [-256, 255]
    int toneColor;     //Adjust the color value, color fidelity,[-256, 255]
    //PLAYM4_VIE_MODU_DEHAZE
    int dehazeLevel;   //the intensity of dehaze,the range is [0, 255]
    int dehazeTrans;   //the transparent value,the range is [0, 255]
    int dehazeBright;  //luminance compensation,the range is[0, 255]
    //PLAYM4_VIE_MODU_DENOISE
    int denoiseLevel;  //the intensity of denoise,the range is [0, 255]
    //PLAYM4_VIE_MODU_SHARPEN
    int usmAmount;     //sharpen intensity,the range is [0, 255]
    int usmRadius;     //the radius of sharpen,the range is[1, 15]
    int usmThreshold;  //the threshold of sharpen,the range is[0, 255]
    //PLAYM4_VIE_MODU_DEBLOCK
    int deblockLevel;  //the level of deblock ,the range is [0, 100]
    //PLAYM4_VIE_MODU_LENS
    int lensWarp;      //warp value,the range is[-256, 255]
    int lensZoom;      //zoom value,the range is[-256, 255]
    //PLAYM4_VIE_MODU_CRB

} PLAYM4_VIE_PARACONFIG;

typedef enum _PLAYM4_VIE_MODULES
{
    PLAYM4_VIE_MODU_ADJ      = 0x00000001, //the basic image adjustment
    PLAYM4_VIE_MODU_EHAN     = 0x00000002, //part enhancement module
    PLAYM4_VIE_MODU_DEHAZE   = 0x00000004, //dehaze module
    PLAYM4_VIE_MODU_DENOISE  = 0x00000008, //denoise module
    PLAYM4_VIE_MODU_SHARPEN  = 0x00000010, //sharpen module
    PLAYM4_VIE_MODU_DEBLOCK  = 0x00000020, //deblock module
    PLAYM4_VIE_MODU_CRB      = 0x00000040, //color balance module
    PLAYM4_VIE_MODU_LENS     = 0x00000080, //Lens warp correction
}PLAYM4_VIE_MODULES;
#endif

//Set Enable or disable the mode APIs
int PlayM4_VIE_SetModuConfig(int lPort,int nModuFlag,int bEnable);


//Set image region of image enhancement
int PlayM4_VIE_SetRegion(int lPort,int lRegNum,HKRECT* pRect);

//Get the enable module of image enhancement
int PlayM4_VIE_GetModuConfig(int lPort,int* pdwModuFlag);

//Set the parameter of the enable module of image enhancement
int PlayM4_VIE_SetParaConfig(int lPort,PLAYM4_VIE_PARACONFIG* pParaConfig);

//Get the parameter of the enable module of image enhancement
int PlayM4_VIE_GetParaConfig(int lPort,PLAYM4_VIE_PARACONFIG* pParaConfig);

//synchronization audio and video
int PlayM4_SyncToAudio(int nPort, int bSyncToAudio);

// int PlayM4_EnableIVS(int nPort, int bTrue);

typedef enum _PLAYM4_PRIDATA_RENDER
{
    PLAYM4_RENDER_ANA_INTEL_DATA   = 0x00000001,
    PLAYM4_RENDER_MD               = 0x00000002,
    PLAYM4_RENDER_ADD_POS          = 0x00000004,
    PLAYM4_RENDER_ADD_PIC          = 0x00000008,
    PLAYM4_RENDER_FIRE_DETCET      = 0x00000010,
    PLAYM4_RENDER_TEM              = 0x00000020,
    PLAYM4_RENDER_TRACK_TEM        = 0x00000040,
    PLAYM4_RENDER_THERMAL          = 0x00000080
}PLAYM4_PRIDATA_RENDER;

typedef enum _PLAYM4_THERMAL_FLAG
{
    PLAYM4_THERMAL_FIREMASK = 0x00000001,
    PLAYM4_THERMAL_RULEGAS = 0x00000002,
    PLAYM4_THERMAL_TARGETGAS = 0x00000004
}PLAYM4_THERMAL_FLAG;

typedef enum _PLAYM4_FIRE_ALARM{
    PLAYM4_FIRE_FRAME_DIS = 0x00000001,
    PLAYM4_FIRE_MAX_TEMP = 0x00000002,
    PLAYM4_FIRE_MAX_TEMP_POSITION = 0x00000004,
    PLAYM4_FIRE_DISTANCE = 0x00000008
}PLAYM4_FIRE_ALARM;

typedef enum _PLAYM4_TEM_FLAG{
    PLAYM4_TEM_REGION_BOX = 0x00000001,
    PLAYM4_TEM_REGION_LINE = 0x00000002,
    PLAYM4_TEM_REGION_POINT = 0x00000004
}PLAYM4_TEM_FLAG;

typedef enum _PLAYM4_TRACK_FLAG
{
    PLAYM4_TRACK_PEOPLE = 0x00000001,
    PLAYM4_TRACK_VEHICLE = 0x00000002
}PLAYM4_TRACK_FLAG;

typedef struct TI_PTZ_INFO_
{
    unsigned short dwDefVer;
    unsigned short dwLength;
    unsigned int          dwP;    //P(0~3600)
    unsigned int          dwT;         //T(0~3600)
    unsigned int          dwZ;         //Z(0~3600)
    unsigned char        chFSMState;
    unsigned char           bClearFocusState;
    unsigned char        reserved[6];
}PTZ_INFO;



int PlayM4_RenderPrivateData(int nPort, int nIntelType, int bTrue);

int PlayM4_RenderPrivateDataEx(int nPort, int nIntelType, int nSubType, int bTrue);

int PlayM4_SetEncryptTypeCallBack(int nPort,
                                                        void (CALLBACK* EncryptTypeCBFun)(long nPort, ENCRYPT_INFO* pEncryptInfo, long nReserved));

int PlayM4_GetStreamAdditionalInfo(int nPort, int lType, unsigned char* pInfo, int* plLen);


#ifdef __cplusplus
    }
#endif

#endif //_PLAYM4_H
