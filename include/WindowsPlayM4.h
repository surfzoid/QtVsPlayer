#include <windows.h>
#ifndef _WINDOWS_PLAYM4_H_
#define _WINDOWS_PLAYM4_H_

#if defined( _WINDLL)
    #define PLAYM4_API  extern "C" __declspec(dllexport)
#else
    #define PLAYM4_API  extern "C" __declspec(dllimport)
#endif

typedef int PLAYM4_HWND;

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
#define BUF_VIDEO_RENDER            (3) //video render node count
#define BUF_AUDIO_RENDER            (4) //audio render node count
#define BUF_VIDEO_DECODED           (5) //video decoded node count to render
#define BUF_AUDIO_DECODED           (6) //audio decoded node count to render

//Error code
#define  PLAYM4_NOERROR                         0   //no error
#define  PLAYM4_PARA_OVER                       1   //input parameter is invalid;
#define  PLAYM4_ORDER_ERROR                     2   //The order of the function to be called is error.
#define  PLAYM4_TIMER_ERROR                     3   //Create multimedia clock failed;
#define  PLAYM4_DEC_VIDEO_ERROR                 4   //Decode video data failed.
#define  PLAYM4_DEC_AUDIO_ERROR                 5   //Decode audio data failed.
#define  PLAYM4_ALLOC_MEMORY_ERROR              6   //Allocate memory failed.
#define  PLAYM4_OPEN_FILE_ERROR                 7   //Open the file failed.
#define  PLAYM4_CREATE_OBJ_ERROR                8   //Create thread or event failed
#define  PLAYM4_CREATE_DDRAW_ERROR              9   //Create DirectDraw object failed.
#define  PLAYM4_CREATE_OFFSCREEN_ERROR          10  //failed when creating off-screen surface.
#define  PLAYM4_BUF_OVER                        11  //buffer is overflow
#define  PLAYM4_CREATE_SOUND_ERROR              12  //failed when creating audio device.
#define  PLAYM4_SET_VOLUME_ERROR                13  //Set volume failed
#define  PLAYM4_SUPPORT_FILE_ONLY               14  //The function only support play file.
#define  PLAYM4_SUPPORT_STREAM_ONLY             15  //The function only support play stream.
#define  PLAYM4_SYS_NOT_SUPPORT                 16  //System not support.
#define  PLAYM4_FILEHEADER_UNKNOWN              17  //No file header.
#define  PLAYM4_VERSION_INCORRECT               18  //The version of decoder and encoder is not adapted.
#define  PLAYM4_INIT_DECODER_ERROR              19  //Initialize decoder failed.
#define  PLAYM4_CHECK_FILE_ERROR                20  //The file data is unknown.
#define  PLAYM4_INIT_TIMER_ERROR                21  //Initialize multimedia clock failed.
#define  PLAYM4_BLT_ERROR                       22  //Blt failed.
#define  PLAYM4_UPDATE_ERROR                    23  //Update failed.
#define  PLAYM4_OPEN_FILE_ERROR_MULTI           24  //openfile error, streamtype is multi
#define  PLAYM4_OPEN_FILE_ERROR_VIDEO           25  //openfile error, streamtype is video
#define  PLAYM4_JPEG_COMPRESS_ERROR             26  //JPEG compress error
#define  PLAYM4_EXTRACT_NOT_SUPPORT             27  //Don't support the version of this file.
#define  PLAYM4_EXTRACT_DATA_ERROR              28  //extract video data failed.
#define  PLAYM4_SECRET_KEY_ERROR                29  //Secret key is error //add 20071218
#define  PLAYM4_DECODE_KEYFRAME_ERROR           30  //add by hy 20090318
#define  PLAYM4_NEED_MORE_DATA                  31  //add by hy 20100617
#define  PLAYM4_INVALID_PORT                    32  //add by cj 20100913
#define  PLAYM4_NOT_FIND                        33  //add by cj 20110428
#define  PLAYM4_NEED_LARGER_BUFFER              34  //add by pzj 20130528
#define  PLAYM4_FAIL_UNKNOWN                    99  //Fail, but the reason is unknown;

//Error Codes of Fisheye Camera
#define PLAYM4_FEC_ERR_ENABLEFAIL               100 // Fisheye camera module loading failed.
#define PLAYM4_FEC_ERR_NOTENABLE                101 // No fisheye camera loaded.
#define PLAYM4_FEC_ERR_NOSUBPORT                102 // No sub port allocated.
#define PLAYM4_FEC_ERR_PARAMNOTINIT             103 // Parameters of corresponding ports uninitialized.
#define PLAYM4_FEC_ERR_SUBPORTOVER              104 // No sub port available.
#define PLAYM4_FEC_ERR_EFFECTNOTSUPPORT         105 // This installation mode does not support this effect.
#define PLAYM4_FEC_ERR_INVALIDWND               106 // Illegal Window
#define PLAYM4_FEC_ERR_PTZOVERFLOW              107 // Line Crossing of PTZ Position
#define PLAYM4_FEC_ERR_RADIUSINVALID            108 // Invalid parameter of the center of a circle
#define PLAYM4_FEC_ERR_UPDATENOTSUPPORT         109 // Some installation modes and correction modes do not support the update of this parameters.
#define PLAYM4_FEC_ERR_NOPLAYPORT               110 // The port of the player is disabled.
#define PLAYM4_FEC_ERR_PARAMVALID               111 // No parameter.
#define PLAYM4_FEC_ERR_INVALIDPORT              112 // Invalid sub port.
#define PLAYM4_FEC_ERR_PTZZOOMOVER              113 // Line Crossing of PTZ Correction Range
#define PLAYM4_FEC_ERR_OVERMAXPORT              114 // No more correction channel is allowed. Up to 4 correction channels are supported.
#define PLAYM4_FEC_ERR_ENABLED                  115 // The fisheye module has been enabled in this port.
#define PLAYM4_FEC_ERR_D3DACCENOTENABLE         116 // D3D acceleration function is disabled.
#define PLAYM4_FEC_ERR_PLACETYPE                117 // Invalid installation mode. One port of the player corresponds to one kind of the installation mode.
#define PLAYM4_FEC_ERR_CorrectType              118 // Dewarping mode erroor: If a dewarping mode is already selected for the camera, you can not enable                                                              other dewarping modes any more. For one player SDK port, each dewarping mode (excepts PTZ mode and                                                             fisheye mode) is available in one camera only. Dual-180 degree panorama mode cannot be enabled                                                                 together with the PTZ mode. And there is no relation between fisheye mode and other modes.
#define PLAYM4_FEC_ERR_NULLWND                  119 // The fisheye window is empty.
#define PLAYM4_FEC_ERR_PARA                     120 // Invalid parameter


//Max display regions.
#define MAX_DISPLAY_WND     4

//Display type
#define DISPLAY_NORMAL      0x00000001
#define DISPLAY_QUARTER     0x00000002
#define DISPLAY_YC_SCALE    0x00000004
#define DISPLAY_NOTEARING   0x00000008
//Display buffers
#define MAX_DIS_FRAMES      50
#define MIN_DIS_FRAMES      1

//Locate by
#define BY_FRAMENUM         1
#define BY_FRAMETIME        2

//Source buffer
#define SOURCE_BUF_MAX      1024*100000
#define SOURCE_BUF_MIN      1024*50

//Stream type
#define STREAME_REALTIME    0
#define STREAME_FILE        1

//frame type
#define T_AUDIO16           101
#define T_AUDIO8            100
#define T_UYVY              1
#define T_YV12              3
#define T_RGB32             7

//capability
#define SUPPORT_DDRAW       1
#define SUPPORT_BLT         2
#define SUPPORT_BLTFOURCC   4
#define SUPPORT_BLTSHRINKX  8
#define SUPPORT_BLTSHRINKY  16
#define SUPPORT_BLTSTRETCHX 32
#define SUPPORT_BLTSTRETCHY 64
#define SUPPORT_SSE         128
#define SUPPORT_MMX         256

// Following macro definitions can be applied to the structure HIK_MEDIAINFO
#define FOURCC_HKMI         0x484B4D49  // "HKMI" HIK_MEDIAINFO structure mark
// System Package Format
#define SYSTEM_NULL         0x0         // No system layer. Audio stream only or video stream only.
#define SYSTEM_HIK          0x1         // Hikvision File Layer
#define SYSTEM_MPEG2_PS     0x2         // PS Package
#define SYSTEM_MPEG2_TS     0x3         // TS Package
#define SYSTEM_RTP          0x4         // rtp Package
#define SYSTEM_RTPHIK       0x401       // rtp Package

// Video Encoding Type
#define VIDEO_NULL          0x0 // No video
#define VIDEO_H264          0x1 // Hikvision H.264
#define VIDEO_MPEG2         0x2 // Standard MPEG2
#define VIDEO_MPEG4         0x3 // Standard MPEG4
#define VIDEO_MJPEG         0x4
#define VIDEO_AVC264        0x0100

// Audio Encoding Type
#define AUDIO_NULL          0x0000 // No Audio
#define AUDIO_ADPCM         0x1000 // ADPCM
#define AUDIO_MPEG          0x2000 // The decoder can auto-fit all kinds of MPEG series audios.
#define AUDIO_AAC           0X2001 // AAC Encoding
#define AUDIO_RAW_DATA8     0x7000 // The original data whose sampling rate is 8k.
#define AUDIO_RAW_UDATA16   0x7001 // The original data whose sampling rate is 16k.
// G series audio
#define AUDIO_RAW_DATA8     0x7000      //The original data whose sampling rate is 8k.
#define AUDIO_RAW_UDATA16   0x7001      //The original data whose sampling rate is16k, that is, L16.
#define AUDIO_G711_U        0x7110
#define AUDIO_G711_A        0x7111
#define AUDIO_G722_1        0x7221
#define AUDIO_G723_1        0x7231
#define AUDIO_G726_U        0x7260
#define AUDIO_G726_A        0x7261
#define AUDIO_G726_16       0x7262
#define AUDIO_G729          0x7290
#define AUDIO_AMR_NB        0x3000

#define SYNCDATA_VEH        1 // Synchronization data: mobile camera information
#define SYNCDATA_IVS        2 // Synchronization data: smart function information

//motion flow type
#define	MOTION_FLOW_NONE    0
#define MOTION_FLOW_CPU     1
#define MOTION_FLOW_GPU     2

//Encryption Type of Audio/Video
#define ENCRYPT_AES_3R_VIDEO     1
#define ENCRYPT_AES_10R_VIDEO    2
#define ENCRYPT_AES_3R_AUDIO     1
#define ENCRYPT_AES_10R_AUDIO    2

///<Rotation Degree
#define R_ANGLE_0           -1      //No rotation
#define R_ANGLE_L90         0       //Rotate Left 90??
#define R_ANGLE_R90         1       //Rotate Right 90??
#define R_ANGLE_180         2       //Rotate 180??


//Frame position
typedef struct{
    LONGLONG nFilePos;
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
    DWORD dwFrameNum;
}FRAME_INFO;

//Frame
typedef struct{
    char *pDataBuf;
    long  nSize;
    long  nFrameNum;
    BOOL  bIsAudio;
    long  nReserved;
}FRAME_TYPE;

//Watermark Info
typedef struct{
    char *pDataBuf;
    long  nSize;
    long  nFrameNum;
    BOOL  bRsaRight;
    long  nReserved;
}WATERMARK_INFO;

typedef struct SYNCDATA_INFO
{
    DWORD dwDataType;       //Subsidiary information type which is synchronized with the stream data: smart function information and the mobile camera information.
    DWORD dwDataLen;        // The Data Length of Subsidiary Information
    BYTE* pData;            // Pointer to the structure of subsidiary information, e.g., IVS_INFO structure
} SYNCDATA_INFO;

#ifndef _HIK_MEDIAINFO_FLAG_
#define _HIK_MEDIAINFO_FLAG_
typedef struct _HIK_MEDIAINFO_
{
    unsigned int    media_fourcc;           // "HKMI": 0x484B4D49 Hikvision Media Information
    unsigned short  media_version;          // Version No.: the version No. of the current information. The current version is 0x0101, that is, V1.01. The first 01 refers to the main version No., and the last 01 refers to the sub version No..
    unsigned short  device_id;              // Device ID, Easy for being tracked down and analyzed.
    unsigned short  system_format;          // System Package Layer
    unsigned short  video_format;           // Video Encoding Type
    unsigned short  audio_format;           // Audio Encoding Type
    unsigned char   audio_channels;         // Channel Number
    unsigned char   audio_bits_per_sample;  // Sample Bitrate
    unsigned int    audio_samplesrate;      // Sampling Rate
    unsigned int    audio_bitrate;          // Compression Audio Bitrate ??Unit: Bit??
    unsigned int    reserved[4];            // Reserved
}HIK_MEDIAINFO;
#endif

typedef struct
{
    int nPort;
    char * pBuf;
    long nBufLen;
    long nWidth;
    long nHeight;
    long nStamp;
    long nType;
    void* nUser;
}DISPLAY_INFO;

typedef struct
{
    int nPort;
    char *pVideoBuf;
    long nVideoBufLen;
    char *pPriBuf;
    long nPriBufLen;
    long nWidth;
    long nHeight;
    long nStamp;
    long nType;
    void* nUser;
}DISPLAY_INFOEX;

typedef struct PLAYM4_SYSTEM_TIME // Absolute Time
{
    DWORD dwYear;   // Year
    DWORD dwMon;    // Month
    DWORD dwDay;    // Day
    DWORD dwHour;   // Hour
    DWORD dwMin;    // Minute
    DWORD dwSec;    // Second
    DWORD dwMs;     // Millisecond
} PLAYM4_SYSTEM_TIME;

#ifndef CROP_PIC_INFO_TAG
#define CROP_PIC_INFO_TAG
typedef struct
{
    BYTE  *pDataBuf;      //Buffer of Capturing Data
    DWORD dwPicSize;      //Actual Picture Size
    DWORD dwBufSize;      //Buffer of Data Size
    DWORD dwPicWidth;     //Width of Screenshot
    DWORD dwPicHeight;    //Height of Screenshot
    DWORD dwReserve;      //Add a segment of reserve.
    RECT  *pCropRect;     //Select region NULL, same with the previous capturing interface.
}CROP_PIC_INFO;
#endif

//ENCRYPT Info
typedef struct{
    long nVideoEncryptType;  //Video Encryption Type
    long nAudioEncryptType;  //Audio Encryption Type
    long nSetSecretKey;      //Whether to set the key. 1: Yes; 2: No.
}ENCRYPT_INFO;

///<Private Information Module Type
typedef enum _PLAYM4_PRIDATA_RENDER
{
    PLAYM4_RENDER_ANA_INTEL_DATA    = 0x00000001, // VCA
    PLAYM4_RENDER_MD                = 0x00000002, //Motion Detection
    PLAYM4_RENDER_ADD_POS           = 0x00000004, //POS Overlay
    PLAYM4_RENDER_ADD_PIC           = 0x00000008, //Picture Overlay
    PLAYM4_RENDER_FIRE_DETCET       = 0x00000010, //Thermal Imagery Information
    PLAYM4_RENDER_TEM               = 0x00000020, //Temperature Information
}PLAYM4_PRIDATA_RENDER;

typedef enum _PLAYM4_FIRE_ALARM
{
    PLAYM4_FIRE_FRAME_DIS           = 0x00000001, //Fire Source Frame Display
    PLAYM4_FIRE_MAX_TEMP            = 0x00000002, //Max. Temperature
    PLAYM4_FIRE_MAX_TEMP_POSITION   = 0x00000004, //Max. Temperature Position
    PLAYM4_FIRE_DISTANCE            = 0x00000008, //Max. Temperature Distance
}PLAYM4_FIRE_ALARM;

typedef enum _PLAYM4_TEM_FLAG
{
    PLAYM4_TEM_REGION_BOX             = 0x00000001, //Temperature Measurement via frame
    PLAYM4_TEM_REGION_LINE            = 0x00000002, //Temperature Measurement via Line
    PLAYM4_TEM_REGION_POINT           = 0x00000004, //Temperature Measurement via Point
}PLAYM4_TEM_FLAG;

/////////////////////////////////////////////////////////////////////////////////////////////
//Proposed Interface
/////////////////////////////////////////////////////////////////////////////////////////////
///<<Initialization
PLAYM4_API BOOL __stdcall PlayM4_GetPort(int* nPort);
PLAYM4_API BOOL __stdcall PlayM4_FreePort(int nPort);
PLAYM4_API BOOL __stdcall PlayM4_OpenFile(int nPort,LPSTR sFileName);
PLAYM4_API BOOL __stdcall PlayM4_CloseFile(int nPort);
PLAYM4_API BOOL __stdcall PlayM4_OpenStream(int nPort,PBYTE pFileHeadBuf,DWORD nSize,DWORD nBufPoolSize);
PLAYM4_API BOOL __stdcall PlayM4_CloseStream(int nPort);
PLAYM4_API BOOL __stdcall PlayM4_InputData(int nPort,PBYTE pBuf,DWORD nSize);
PLAYM4_API BOOL __stdcall PlayM4_SetStreamOpenMode(int nPort,DWORD nMode);
PLAYM4_API LONG __stdcall PlayM4_GetStreamOpenMode(int nPort);

///<Index
PLAYM4_API BOOL __stdcall PlayM4_SetFileRefCallBack(int nPort, void (__stdcall *pFileRefDone)(DWORD nPort,void* nUser),void* nUser);
PLAYM4_API BOOL __stdcall PlayM4_GetRefValue(int nPort,BYTE *pBuffer, DWORD *pSize);
PLAYM4_API BOOL __stdcall PlayM4_SetRefValue(int nPort,BYTE *pBuffer, DWORD nSize);
PLAYM4_API BOOL __stdcall PlayM4_GetRefValueEx(int nPort,BYTE *pBuffer, DWORD *pSize);//< Add Frame Type Callback
PLAYM4_API BOOL __stdcall PlayM4_GetKeyFramePos(int nPort,DWORD nValue, DWORD nType, PFRAME_POS pFramePos);
PLAYM4_API BOOL __stdcall PlayM4_GetNextKeyFramePos(int nPort,DWORD nValue, DWORD nType, PFRAME_POS pFramePos);

///<Play Control
PLAYM4_API BOOL __stdcall PlayM4_Play(int nPort, int hWnd);
PLAYM4_API BOOL __stdcall PlayM4_Stop(int nPort);
PLAYM4_API BOOL __stdcall PlayM4_Pause(int nPort,DWORD nPause);
PLAYM4_API BOOL __stdcall PlayM4_Fast(int nPort);
PLAYM4_API BOOL __stdcall PlayM4_Slow(int nPort);
PLAYM4_API BOOL __stdcall PlayM4_OneByOne(int nPort);
PLAYM4_API BOOL __stdcall PlayM4_OneByOneBack(int nPort);
PLAYM4_API BOOL __stdcall PlayM4_ReversePlay(int nPort);
PLAYM4_API BOOL __stdcall PlayM4_RefreshPlay(int nPort);
PLAYM4_API BOOL __stdcall PlayM4_RefreshPlayEx(int nPort,DWORD nRegionNum);

///<Audio Play
PLAYM4_API BOOL __stdcall PlayM4_PlaySound(int nPort);
PLAYM4_API BOOL __stdcall PlayM4_StopSound();
PLAYM4_API BOOL __stdcall PlayM4_PlaySoundShare(int nPort);
PLAYM4_API BOOL __stdcall PlayM4_StopSoundShare(int nPort);
PLAYM4_API BOOL __stdcall PlayM4_SetVolume(int nPort,WORD nVolume);
PLAYM4_API WORD __stdcall PlayM4_GetVolume(int nPort);
PLAYM4_API BOOL __stdcall PlayM4_AdjustWaveAudio(int nPort,LONG nCoefficient);

///<Get Information
PLAYM4_API BOOL __stdcall PlayM4_SetPlayPos(int nPort,float fRelativePos);
PLAYM4_API float __stdcall PlayM4_GetPlayPos(int nPort);
PLAYM4_API DWORD __stdcall PlayM4_GetFileTime(int nPort);
PLAYM4_API DWORD __stdcall PlayM4_GetPlayedTime(int nPort);
PLAYM4_API DWORD __stdcall PlayM4_GetPlayedFrames(int nPort);
PLAYM4_API DWORD __stdcall PlayM4_GetFileTotalFrames(int nPort);
PLAYM4_API DWORD __stdcall PlayM4_GetCurrentFrameRate(int nPort);
PLAYM4_API DWORD __stdcall PlayM4_GetPlayedTimeEx(int nPort);
PLAYM4_API BOOL __stdcall PlayM4_SetPlayedTimeEx(int nPort,DWORD nTime);
PLAYM4_API DWORD __stdcall PlayM4_GetCurrentFrameNum(int nPort);
PLAYM4_API BOOL __stdcall PlayM4_SetCurrentFrameNum(int nPort,DWORD nFrameNum);
PLAYM4_API BOOL __stdcall PlayM4_GetSystemTime(int nPort, PLAYM4_SYSTEM_TIME *pstSystemTime);
PLAYM4_API DWORD __stdcall PlayM4_GetSpecialData(int nPort);
PLAYM4_API BOOL __stdcall PlayM4_GetPictureSize(int nPort,int *pWidth,int *pHeight);
PLAYM4_API BOOL __stdcall PlayM4_SetFileEndCallback(int nPort, void(CALLBACK*FileEndCallback)(int nPort, void *pUser), void *pUser);
///<Time zone, unit: second, +: eastern time zone, -: western time zone
PLAYM4_API BOOL __stdcall PlayM4_SetSupplementaryTimeZone(int nPort, int nTimeZone);
///<If the time zone is configured, the time zone will be returned, otherwise, returns failure
PLAYM4_API BOOL __stdcall PlayM4_GetSupplementaryTimeZone(int nPort, int* pTimeZone);

///<Buffer Operation
PLAYM4_API DWORD __stdcall PlayM4_GetSourceBufferRemain(int nPort);
PLAYM4_API BOOL __stdcall PlayM4_ResetSourceBuffer(int nPort);
PLAYM4_API BOOL __stdcall PlayM4_SetSourceBufCallBack(int nPort,DWORD nThreShold,void (CALLBACK * SourceBufCallBack)(int nPort,DWORD nBufSize,void* dwUser,void*pResvered),void* dwUser,void *pReserved);
PLAYM4_API BOOL __stdcall PlayM4_ResetSourceBufFlag(int nPort);
PLAYM4_API BOOL __stdcall PlayM4_ResetBuffer(int nPort,DWORD nBufType);
PLAYM4_API DWORD __stdcall PlayM4_GetBufferValue(int nPort,DWORD nBufType);
PLAYM4_API BOOL __stdcall PlayM4_SetDisplayBuf(int nPort,DWORD nNum);
PLAYM4_API DWORD __stdcall PlayM4_GetDisplayBuf(int nPort);

///<Capture Picture
PLAYM4_API BOOL __stdcall PlayM4_SetJpegQuality(long nQuality);
PLAYM4_API BOOL __stdcall PlayM4_GetBMP(int nPort,PBYTE pBitmap,DWORD nBufSize,DWORD* pBmpSize);
PLAYM4_API BOOL __stdcall PlayM4_GetJPEG(int nPort,PBYTE pJpeg,DWORD nBufSize,DWORD* pJpegSize);
PLAYM4_API BOOL __stdcall PlayM4_ConvertToBmpFile(char * pBuf,long nSize,long nWidth,long nHeight,long nType,char *sFileName);
PLAYM4_API BOOL __stdcall PlayM4_ConvertToJpegFile(char * pBuf,long nSize,long nWidth,long nHeight,long nType,char *sFileName);

///<Decode
PLAYM4_API BOOL __stdcall PlayM4_SetSecretKey(int nPort, LONG lKeyType, char *pSecretKey, LONG lKeyLen);
PLAYM4_API BOOL __stdcall PlayM4_ThrowBFrameNum(int nPort,DWORD nNum);
PLAYM4_API BOOL __stdcall PlayM4_SetDecCBStream(int nPort,DWORD nStream);
PLAYM4_API BOOL __stdcall PlayM4_SetDecodeFrameType(int nPort,DWORD nFrameType);
PLAYM4_API BOOL __stdcall PlayM4_CheckDiscontinuousFrameNum(int nPort, BOOL bCheck);
PLAYM4_API BOOL __stdcall PlayM4_SkipErrorData(int nPort, BOOL bSkip);
PLAYM4_API BOOL __stdcall PlayM4_SetDecCallBackMend(int nPort,void (CALLBACK* DecCBFun)(int nPort,char * pBuf,long nSize,FRAME_INFO * pFrameInfo, void* nUser,void* nReserved2), void* nUser);
PLAYM4_API BOOL __stdcall PlayM4_SetDecCallBackExMend(int nPort, void (CALLBACK* DecCBFun)(int nPort, char* pBuf, long nSize, FRAME_INFO* pFrameInfo, void* nUser, void* nReserved2), char* pDest, long nDestSize, void* nUser);
// Encrypted Stream Callback, nType=0 indicates that callback when the stream encryption sign position is changed, nType=1 indicates that callback if there is the stream encryption sign position.
PLAYM4_API BOOL __stdcall PlayM4_SetEncryptTypeCallBack(int nPort, DWORD nType, void (CALLBACK* EncryptTypeCBFun)(int nPort, ENCRYPT_INFO* pEncryptInfo, void* nUser, long nReserved2), void* nUser);

///<Display
PLAYM4_API BOOL __stdcall PlayM4_SetDisplayRegion(int nPort,DWORD nRegionNum, RECT *pSrcRect, int hDestWnd, BOOL bEnable);///<Digital Zoom
PLAYM4_API BOOL __stdcall PlayM4_SetDisplayRegionOnWnd(int nPort,DWORD nRegionNum, RECT *pSrcRect, BOOL bEnable);///<Multi-Screen Splicing Interface
PLAYM4_API BOOL __stdcall PlayM4_SetDisplayType(int nPort,LONG nType);
PLAYM4_API long __stdcall PlayM4_GetDisplayType(int nPort);
PLAYM4_API BOOL __stdcall PlayM4_SetDisplayCallBack(int nPort,void (CALLBACK* DisplayCBFun)(int nPort,char * pBuf,int nSize,int nWidth,int nHeight,int nStamp,int nType,int nReserved));
PLAYM4_API BOOL __stdcall PlayM4_SetDisplayCallBackEx(int nPort,void (CALLBACK* DisplayCBFun)(DISPLAY_INFO *pstDisplayInfo), void* nUser);
PLAYM4_API BOOL __stdcall PlayM4_RegisterDrawFun(int nPort,void (CALLBACK* DrawFun)(int nPort,HDC hDc,void* nUser),void* nUser);
PLAYM4_API BOOL __stdcall PlayM4_SetEncTypeChangeCallBack(int nPort,void(CALLBACK *funEncChange)(int nPort,void* nUser),void* nUser);
PLAYM4_API BOOL __stdcall PlayM4_SetCheckWatermarkCallBack(int nPort,void(CALLBACK* funCheckWatermark)(int nPort,WATERMARK_INFO* pWatermarkInfo,void* nUser),void* nUser);
PLAYM4_API BOOL __stdcall PlayM4_RenderPrivateData(int nPort, int nIntelType, BOOL bTrue);
PLAYM4_API BOOL __stdcall PlayM4_RenderPrivateDataEx(int nPort, int nIntelType, int nSubType, BOOL bTrue);
PLAYM4_API BOOL __stdcall PlayM4_SetOverlayPriInfoFlag(int nPort, int nIntelType, BOOL bTrue);

//lType: 1 indicates getting the current PTZ information of the displayed frame. It is saved in pInfo as a specified structure. plLen will return the length. Input pInfo = null and get the memory length plLen needs to be allocated.
PLAYM4_API BOOL __stdcall PlayM4_GetStreamAdditionalInfo(int nPort, LONG lType, BYTE* pInfo, LONG* plLen);

///<Image Settings
PLAYM4_API BOOL __stdcall PlayM4_SetColor(int nPort, DWORD nRegionNum, int nBrightness, int nContrast, int nSaturation, int nHue);
PLAYM4_API BOOL __stdcall PlayM4_GetColor(int nPort, DWORD nRegionNum, int *pBrightness, int *pContrast, int *pSaturation, int *pHue);
PLAYM4_API BOOL __stdcall PlayM4_SetImageSharpen(int nPort,DWORD nLevel);
PLAYM4_API BOOL __stdcall PlayM4_SetRotateAngle(int nPort, DWORD nRegionNum, DWORD dwType);

///<Others
PLAYM4_API DWORD __stdcall PlayM4_GetFileHeadLength();
PLAYM4_API DWORD __stdcall PlayM4_GetSdkVersion();
PLAYM4_API DWORD __stdcall PlayM4_GetLastError(int nPort);
PLAYM4_API BOOL __stdcall PlayM4_SetSycGroup(int nPort, DWORD dwGroupIndex);
PLAYM4_API BOOL __stdcall PlayM4_MotionFlow(int nPort, DWORD dwAdjustType);

//Get the online positioning offset of MP4 container format according to relative time
PLAYM4_API int __stdcall PlayM4_GetMpOffset(int nPort, int nTime, int* nOffset);


#ifndef PLAYM4_HIKD3D11_TAG
#define PLAYM4_HIKD3D11_TAG

#define HXVA_RESOLUTION_NONE   0x00      // 0
#define HXVA_RESOLUTION_200W   0x01      // 1920*1080
#define HXVA_RESOLUTION_300W   0x02      // 2048*1536
#define HXVA_RESOLUTION_500W   0x03      // 2560*1920
#define HXVA_RESOLUTION_800W   0x04      // 3840*2160
#define HXVA_RESOLUTION_1600W  0x05      // 4096*4096
#define HXVA_RESOLUTION_6400W  0x06      // 8192*8192(H.265)

typedef struct _tagHDECODESUPPORT_
{
    unsigned char  chGPUType;
    unsigned char  bDXVA_D3D9;
    unsigned char  bCUVID_D3D11;
    unsigned char  chDXVAH264_Max_Resolution;
    unsigned char  chDXVAH265_Max_Resolution;
    unsigned char  chCUVIDH264_Max_Resolution;
    unsigned char  chCUVIDH265_Max_Resolution;
    unsigned char  chValidFlag;
    unsigned char  nReserved[12];
}HDECODESUPPORT;

typedef struct _tagRENDERSUPPORT_
{
    unsigned char  bDDraw;
    unsigned char  bD3D9Surface;
    unsigned char  bD3D9Texture;
    unsigned char  bD3D11;
    unsigned char  chValidFlag;
    unsigned char  nReserved[11];
}RENDERSUPPORT;

typedef struct _tagENGINESUPPORT_
{
    HDECODESUPPORT stHDecodeSupport;
    RENDERSUPPORT  stRenderSupport;
    unsigned int   chReserved[4];
}ENGINESUPPORT;


typedef struct _tagD3D11_PIC_INFO_
{
    unsigned int   nPicMode;
    unsigned char* pBuf;
    unsigned int   nBufSize;
    unsigned int*  pPicSize;
    unsigned int   nPicWidth;
    unsigned int   nPicHeight;
    unsigned char  chReserve[8];
}D3D_PIC_INFO;

typedef enum tagPLAYM4PostProcType
{
    PLAYM4_PPT_BRIGHTNESS               = 0x1,
    PLAYM4_PPT_HUE                      = 0x2,
    PLAYM4_PPT_SATURATION               = 0x3,
    PLAYM4_PPT_CONTRAST                 = 0x4,
    PLAYM4_PPT_SHARPNESS                = 0x5,
}PLAYM4PostProcType;

#endif

PLAYM4_API int __stdcall PlayM4_GetEngineSupport(int nPort, ENGINESUPPORT* pstEngineSupport);

PLAYM4_API int __stdcall PlayM4_GetD3DCapture(int nPort, unsigned int nType, D3D_PIC_INFO* pstPicInfo);


PLAYM4_API int __stdcall PlayM4_SetD3DPostProcess(int nPort, PLAYM4PostProcType nPostType, float fValue);

PLAYM4_API int __stdcall PlayM4_GetD3DPostProcess(int nPort, PLAYM4PostProcType nPostType, float* fValue);

PLAYM4_API int __stdcall PlayM4_SetConfigFontPath(int nPort, char* pFontPath);

////////////////////////////////////

///<SDP//////////////////////////////////////////////////////////////////////////
#ifndef PLAYM4_SESSION_INFO_TAG
#define PLAYM4_SESSION_INFO_TAG

//nProtocolType
#define PLAYM4_PROTOCOL_RTSP    1
//nSessionInfoType
#define PLAYM4_SESSION_INFO_SDP 1

typedef struct _PLAYM4_SESSION_INFO_   //Interaction Information Structure
{
    int            nSessionInfoType;   //Interaction Information Type, only SDP is supported
    int            nSessionInfoLen;    //Interaction Information Length
    unsigned char* pSessionInfoData;   //Interaction Information Data
} PLAYM4_SESSION_INFO;
#endif

PLAYM4_API BOOL __stdcall PlayM4_OpenStreamAdvanced(int nPort, int nProtocolType, PLAYM4_SESSION_INFO* pstSessionInfo, DWORD nBufPoolSize);

///<Fish-Eye Related Interface//////////////////////////////////////////////////////////////////////////
#ifndef FISH_EYE_TAG
#define FISH_EYE_TAG

// Installation Type
typedef enum tagFECPlaceType
{
    FEC_PLACE_WALL    = 0x1,        // Wall Mounting(Normal on Horizontal)
    FEC_PLACE_FLOOR   = 0x2,        // Ground Mounting(Normal Upwards)
    FEC_PLACE_CEILING = 0x3,        // Ceiling Mounting(Normal Downwards)

}FECPLACETYPE;

typedef enum tagFECCorrectType
{
    FEC_CORRECT_NULL                    = 0x0000,  // Not correct
    FEC_CORRECT_PTZ                     = 0x0100,  // PTZ
    FEC_CORRECT_180                     = 0x0200,  // 180 Degree??Corresponds to 2P??
    FEC_CORRECT_360                     = 0x0300,  // 360 Degree??Corresponds to 1P??
    FEC_CORRECT_LAT                     = 0x0400,  // Expend Dimensionality
    FEC_CORRECT_SEMISPHERE              = 0x0500,  // Fisheye dewarping (not supported by hardware decoding, but supports multiple channels)
    FEC_CORRECT_CYLINDER                = 0x0600,  // Cylindrical-surface-sharped view (ceiling mounting or ground mounting)
    FEC_CORRECT_CYLINDER_SPLIT          = 0x0700,  // Dissected-cylindrical-surface-sharped view (ceiling mounting or ground mounting)
    FEC_CORRECT_PLANET                  = 0x0800,  // Asteroids-shaped view
    FEC_CORRECT_ARCSPHERE_HORIZONTAL    = 0x0900,  // Horizontal-curved-surface-shaped view (wall mounting)
    FEC_CORRECT_ARCSPHERE_VERTICAL      = 0x0A00,  // Vertical-curved-surface-shaped view (wall mounting)

}FECCORRECTTYPE;

typedef enum tagFECCorrectEffect
{
    FEC_CORRECT_EFFECT_BACK_FACE_CULLING    = 0x100,

}FECCORRECTEFFECT;

typedef struct tagCycleParam
{
    float   fRadiusLeft;    // Left Vertex X Axis
    float   fRadiusRight;   // Right Vertex X Axis
    float   fRadiusTop;     // Up Vertex Y Axis
    float   fRadiusBottom;  // Down Vertex Y Axis

}CYCLEPARAM;

typedef struct tagPTZParam
{
    float fPTZPositionX;        // PTZ Displayed Center Position, X Axis
    float fPTZPositionY;        // PTZ Displayed Center Position, Y Axis

}PTZPARAM;

#define         FEC_UPDATE_RADIUS            0x1
#define         FEC_UPDATE_PTZZOOM           0x2
#define         FEC_UPDATE_WIDESCANOFFSET    0x4
#define         FEC_UPDATE_PTZPARAM          0x8
#define         FEC_UPDATT_PTZCOLOR          0x10

// Color Structure
typedef struct tagFECColor
{
    unsigned char nR;     // R Component
    unsigned char nG;	  // G Component
    unsigned char nB;     // B Component
    unsigned char nAlpha; // Alpha Component
}FECCOLOR;

typedef struct tagFECParam
{
    unsigned int    nUpDateType;            // Updated type
    unsigned int    nPlaceAndCorrect;       // Installation and correction mode, can be getonly,  invalid when the value is SetParam, this value indicate the sum of installation mode value and correction mode value.
    PTZPARAM        stPTZParam;             // PTZ correction parameters
    CYCLEPARAM      stCycleParam;           // Fish-eye image circle center parameters
    float           fZoom;                  // PTZ displayed range
    float           fWideScanOffset;        // 180 degree or 360 degree correction offset angle
    FECCOLOR        stPTZColor;             // PTZ Color
    int	            nResver[15];            // Reserved
}FISHEYEPARAM;

// Display Mode for PTZ on the original fish-eye image.
typedef enum tagFECShowMode
{
    FEC_PTZ_OUTLINE_NULL,   // Not display
    FEC_PTZ_OUTLINE_RECT,   // Display in Rectangular
    FEC_PTZ_OUTLINE_RANGE,  // Display Effective Region
}FECSHOWMODE;

#define    FEC_JPEG   0  // JPEG Capture
#define    FEC_BMP    1  // BMP Capture

// Hardware Decoding Sign
#define FEC_DISPLAYSURFACE          0x400  // Fish-eye Hardware Sign

// Rotation Unit Structure
typedef struct tagPLAYM4SRTransformElement
{
    float fAxisX;
    float fAxisY;
    float fAxisZ;
    float fValue; // Rotation Degree

}PLAYM4SRTRANSFERELEMENT;

// Rotation Combination Parameters
typedef struct tagPLAYM4SRTransformParam
{
    PLAYM4SRTRANSFERELEMENT* pTransformElement;		// Rotation Coordinate
    unsigned int		     nTransformCount;		// Rotation Combination Times
}PLAYM4SRTRANSFERPARAM;


//////////*************Not Recommanded API************************//////////////

#define FEC_DISPLAYSPHERE           0x402  // Fisheye render display-not ssuggested

PLAYM4_API BOOL __stdcall PlayM4_FEC_Rotate(int nPort, PLAYM4SRTRANSFERPARAM *pstRotateParam);///<Rotation API (not suggested)

//////////////////////**************************/////////////////////////


///<View Adjustment of New Fisheye Dewarping Image (Zoom in and out)
typedef enum tagPLAYM4HRViewParamType
{
    PLAYM4_HR_VPT_ROTATION_X       = 0x1,          ///<Horizontal rotation
    PLAYM4_HR_VPT_ROTATION_Y       = 0x2,          ///<Vertical rotation
    PLAYM4_HR_VPT_SCALE            = 0x3,          ///<Zoom in or out (when the absolute value=float 0, black screen occurred, but it is normal)
}PLAYM4HRVIEWPARAMTYPE;

// Enumerate the model parameters of cylindrical-surface-shaped view.
typedef enum tagPLAYM4FEC3DModelParam
{
    PLAYM4_FEC_3DMP_CYLINDER_HEIGHT              = 0x1,       ///< Height of cylindrical model
    PLAYM4_FEC_3DMP_CYLINDER_RADIUS              = 0x2,       ///< Radius of cylindrical model
}PLAYM4FEC3DMODELPARAM;

// Enumerate the specific image views
typedef enum tagPLAYM4FECSpecialViewType
{
    PLAYM4_FEC_SVT_EDGE                          = 0x1        ///<A view that the curved surface intersecting with the display window edge.
}PLAYM4FECSPECIALVIEWTYPE;


typedef void (__stdcall* FISHEYE_CallBack )( void* pUser, unsigned int nSubPort, unsigned int nCBType, void* hDC, unsigned int nWidth, unsigned int nHeight);

#endif
// Enable Fish-eye
PLAYM4_API BOOL __stdcall PlayM4_FEC_Enable(int nPort);

// Disable Fish-eye Module
PLAYM4_API BOOL __stdcall PlayM4_FEC_Disable(int nPort);

// Set Fish-eye correction sub port [1~31]
PLAYM4_API BOOL __stdcall PlayM4_FEC_GetPort(int nPort, unsigned int* nSubPort, FECPLACETYPE emPlaceType, FECCORRECTTYPE emCorrectType);

// Delete Fish-eye correction sub API.
PLAYM4_API BOOL __stdcall PlayM4_FEC_DelPort(int nPort, unsigned int nSubPort);

// Set fish-eye image correction parameters
PLAYM4_API BOOL __stdcall PlayM4_FEC_SetParam(int nPort, unsigned int nSubPort, FISHEYEPARAM* pPara);

// Get fish-eye image correction parameters
PLAYM4_API BOOL __stdcall PlayM4_FEC_GetParam(int nPort, unsigned int nSubPort, FISHEYEPARAM* pPara);

// Display window, can be swithed.
PLAYM4_API BOOL __stdcall PlayM4_FEC_SetWnd(int nPort, unsigned int nSubPort, void* hWnd);

// Set the drawing callback of fish-eye window
PLAYM4_API BOOL __stdcall PlayM4_FEC_SetCallBack(int nPort, unsigned int nSubPort, FISHEYE_CallBack cbFunc, void* pUser);

// Fisheye Capturing
PLAYM4_API BOOL __stdcall PlayM4_FEC_Capture(int nPort, unsigned int nSubPort, unsigned int nType, char* pFileName);

// Fish-eye Hardware Decoding
PLAYM4_API BOOL __stdcall PlayM4_FEC_SetConfig(int nPort,unsigned int nType);

PLAYM4_API BOOL __stdcall PlayM4_FEC_GetCurrentPTZPort(int nPort, float fPositionX,float fPositionY, unsigned int *pnPort);

PLAYM4_API BOOL __stdcall PlayM4_FEC_SetCurrentPTZPort(int nPort, unsigned int nSubPort);

PLAYM4_API BOOL __stdcall PlayM4_FEC_SetPTZOutLineShowMode(int nPort,FECSHOWMODE nPTZShowMode);

//API for view rotation of new fisheye dewarping
//Get view parameters (before setting)
PLAYM4_API BOOL __stdcall PlayM4_FEC_GetViewParam(int nPort, unsigned int nSubPort, PLAYM4HRVIEWPARAMTYPE enViewParamType, float* fValue);
//Set view change parameters (the configured value=obtained value + offset)
PLAYM4_API BOOL __stdcall PlayM4_FEC_SetViewParam(int nPort, unsigned int nSubPort, PLAYM4HRVIEWPARAMTYPE enViewParamType, float fValue);

PLAYM4_API BOOL __stdcall PlayM4_FEC_SetDisplayRegion(int nPort, unsigned int nSubPort,unsigned int nType, DWORD nRegionNum, RECT *pSrcRect, int hDestWnd, BOOL bEnable);

PLAYM4_API BOOL __stdcall PlayM4_FEC_SetCorrectEffect(int nPort, unsigned int nSubPort, FECCORRECTEFFECT nCorrectEffect, float fValue);

// Set modelling parameters (i.e., cylindrical height and radius) for cylindrical-surface-sharped view during 3D fisheye dewarping.
PLAYM4_API BOOL __stdcall PlayM4_FEC_Set3DModelParam(int nPort, unsigned int nSubPort, PLAYM4FEC3DMODELPARAM enType, float fValue);

// Get the parameters of a specific image view for curved-surface-shaped view during 3D fisheye dewarping.
PLAYM4_API BOOL __stdcall PlayM4_FEC_GetSpecialViewParam(int nPort, unsigned int nSubPort, PLAYM4FECSPECIALVIEWTYPE enSVType, PLAYM4HRVIEWPARAMTYPE enVPType, float* pValue);


///<Image Enhance API//////////////////////////////////////////////////////////////////////////
#ifndef PLAYM4_HIKVIE_TAG
#define PLAYM4_HIKVIE_TAG

typedef struct _PLAYM4_VIE_DYNPARAM_
{
    int moduFlag;      //Enables Algorithm Processing Module, identified in PLAYM4_VIE_MODULES, such as PLAYM4_VIE_MODU_ADJ | PLAYM4_VIE_MODU_EHAN, you need to set the parameters after enable the module.
    //PLAYM4_VIE_MODU_ADJ
    int brightVal;     //Brightness,[-255, 255]
    int contrastVal;   //Contrast, [-256, 255]
    int colorVal;      //Saturability, [-256, 255]
    //PLAYM4_VIE_MODU_EHAN
    int toneScale;     //Filtering Range, [0, 100]
    int toneGain;      //Gain, [-256, 255]
    int toneOffset;    //Offset, [-255, 255]
    int toneColor;     //Color Hue, [-256, 255]
    //PLAYM4_VIE_MODU_DEHAZE
    int dehazeLevel;   //Defog Level, [0, 255]
    int dehazeTrans;   //Transmission Value, [0, 255]
    int dehazeBright;  //Light Compensation, [0, 255]
    //PLAYM4_VIE_MODU_DENOISE
    int denoiseLevel;  //Denoise Level, [0, 255]
    //PLAYM4_VIE_MODU_SHARPEN
    int usmAmount;     //Sharpen Level, [0, 255]
    int usmRadius;     //Sharpen Radius, [1, 15]
    int usmThreshold;  //Sharpen Threshold, [0, 255]
    //PLAYM4_VIE_MODU_DEBLOCK
    int deblockLevel;  //Deblock Level, [0, 100]
    //PLAYM4_VIE_MODU_LENS
    int lensWarp;      //Warp Value, [-256, 255]
    int lensZoom;      //Zoom, [-256, 255]
    //PLAYM4_VIE_MODU_CRB
    //No Response
} PLAYM4_VIE_PARACONFIG;

typedef enum _PLAYM4_VIE_MODULES
{
    PLAYM4_VIE_MODU_ADJ      = 0x00000001, //Basic Settings
    PLAYM4_VIE_MODU_EHAN     = 0x00000002, //Parts Enhance Module
    PLAYM4_VIE_MODU_DEHAZE   = 0x00000004, //Defog Module
    PLAYM4_VIE_MODU_DENOISE  = 0x00000008, //Denoise Module
    PLAYM4_VIE_MODU_SHARPEN  = 0x00000010, //Sharpen Module
    PLAYM4_VIE_MODU_DEBLOCK  = 0x00000020, //Deblock Module
    PLAYM4_VIE_MODU_CRB      = 0x00000040, //CRB Module
    PLAYM4_VIE_MODU_LENS     = 0x00000080, //Lens Correction Module
}PLAYM4_VIE_MODULES;
#endif

//Enable/Disable Module
//dwModuFlag Corresponded PLAYM4_VIE_MODULES Macro, can be combined.
//Enable the module and set module parameters.
//Clear the parameters set last time when disabling the module.
//Callback other API only if the module is enabled, otherwise, system will return error code.
PLAYM4_API BOOL __stdcall PlayM4_VIE_SetModuConfig(LONG lPort,int nModuFlag,BOOL bEnable);

//Set image enhance region, NULL full screen, min region: 32*32 pixel
//Region Settings, Max. 4 regions. Regions cannot be overlapped.
PLAYM4_API BOOL __stdcall PlayM4_VIE_SetRegion(LONG lPort,LONG lRegNum,RECT* pRect);

//Get the enabling module
PLAYM4_API BOOL __stdcall PlayM4_VIE_GetModuConfig(LONG lPort,int* pdwModuFlag);

//Setting parameters, ignore the settings of module not enabled
PLAYM4_API BOOL __stdcall PlayM4_VIE_SetParaConfig(LONG lPort,PLAYM4_VIE_PARACONFIG* pParaConfig);

//Get the parameters of enabling the module
PLAYM4_API BOOL __stdcall PlayM4_VIE_GetParaConfig(LONG lPort,PLAYM4_VIE_PARACONFIG* pParaConfig);

typedef struct
{
    long    lDataType;          //Private data type
    long    lDataStrVersion;    //Structure version for data returning
    long    lDataTimeStamp;
    long    lDataLength;
    char*   pData;
}AdditionDataInfo;

PLAYM4_API BOOL __stdcall PlayM4_SetAdditionDataCallBack(int nPort, DWORD nSyncType, void (CALLBACK* AdditionDataCBFun)(int nPort, AdditionDataInfo* pstAddDataInfo, void* pUser), void* pUser);


#define SOFT_DECODE_ENGINE 0 ///<Software Decoding
#define HARD_DECODE_ENGINE 1 ///<Hardware Decoding

PLAYM4_API BOOL  __stdcall PlayM4_SetDecodeEngineEx(int nPort, DWORD nDecodeEngine);
PLAYM4_API DWORD __stdcall PlayM4_GetDecodeEngine(int nPort);

#define PLAYM4_SOURCE_MODULE             0 // Data Source Module
#define PLAYM4_DEMUX_MODULE              1 // Parsing Module
#define PLAYM4_DECODE_MODULE             2 // Decoding Module
#define PLAYM4_RENDER_MODULE             3 // Render Module

#define PLAYM4_RTINFO_HARDDECODE_ERROR   0 // Hardware Decoding Error
#define PLAYM4_RTINFO_SOFTDECODE_ERROR   1 // Software Decoding Error
#define PLAYM4_RTINFO_MEDIAHEADER_ERROR  2 // Media Header Error
#define PLAYM4_RTINFO_SWITCH_SOFT_DEC    3 // Switch to software decoding.
#define PLAYM4_RTINFO_ALLOC_MEMORY_ERROR 4 // Memory Allocation failed.
#define PLAYM4_RTINFO_ENCRYPT_ERROR      5 // Private key error.

typedef struct
{
    int            nRunTimeModule;     //Current Running Module
    int            nStrVersion;        //Structure Version
    int            nFrameTimeStamp;    //Frame No.
    int            nFrameNum;          //Timestamp
    int            nErrorCode;         //Error Coe
    unsigned char  reserved[12];       //Reserved
}RunTimeInfo;

PLAYM4_API BOOL __stdcall PlayM4_SetRunTimeInfoCallBackEx(int nPort, int nModule, void (CALLBACK* RunTimeInfoCBFun)(int nPort, RunTimeInfo* pstRunTimeInfo, void* pUser), void* pUser);

#ifndef PLAYM4_HIKSR_TAG
#define PLAYM4_HIKSR_TAG

#define CFG_DISPLAY_MODEL_MODE         0x499

// Module Texture Mode 0- Both Sides, 1-Outer Side, 2- Inner Side
typedef enum tagPLAYM4SRTextureMode
{
    PLAYM4_TEXTURE_DOUBLE       = 0x0,           // Texture on Both Sides
    PLAYM4_TEXTURE_OUTER        = 0x1,           // Texture on Outer Side
    PLAYM4_TEXTURE_INNER        = 0x2,           // Texture on Inner Side
}PLAYM4SRTEXTUREMODE;

//Module Type
typedef enum tagPLAYM4SRModelType
{
    PLAYM4_MODELTYPE_HEMISPHERE = 0x0,           //Dome Camera Module for Fish-eye Camera
    PLAYM4_MODELTYPE_EAGLE_EYE  = 0x1,           //Module of PanoVu without PTZ
    PLAYM4_MODELTYPE_CUBE       = 0x2,           //Cube Module
}PLAYM4SRMODELTYPE;

#define PLAYM4_MODEL_SOLID      0x0001           //Entity Module
#define PLAYM4_MODEL_FRAME      0x0010           //Frame Module
#define PLAYM4_MODEL_AXIS       0x0100           //Coordinate

//Parameters of Rendering to 3D Module
typedef struct tagRenderModelMode
{
    bool                          bModelMode;      // Whether to render to 3D Module
    PLAYM4SRTEXTUREMODE           emTextureMode;   // Texure Mode, two sides, inner side, outer side
    unsigned long                 ulDisplayType;   // Combied by Sign Position SR_MODEL_SOLID | SR_MODEL_FRAME | SR_MODEL_AXIS
    unsigned int                  nTransformMode;  // 0 Frame coordinate changed according to the module, 1- Frame coordinate not changed according to the module,
    PLAYM4SRMODELTYPE             emModelType;     // Module Type, fish-eye, PanoVu, etc,.
}PLAYM4SRMODELMODE;

#define SR_JPEG    0
#define SR_BMP     1

#endif


PLAYM4_API BOOL __stdcall PlayM4_SR_SetConfig(int nPort, int nCfgType, void *pValue);

PLAYM4_API BOOL __stdcall PlayM4_SR_Rotate(int nPort, PLAYM4SRTRANSFERPARAM *pstRotateParam);

PLAYM4_API BOOL __stdcall PlayM4_SR_Capture(int nPort, unsigned int nType, char *pFileName);



///<Pre-record API-Not Support//////////////////////////////////////////////////////////////////////////
#define PLAYM4_MEDIA_HEAD     1
#define PLAYM4_VIDEO_DATA     2
#define PLAYM4_AUDIO_DATA     3
#define PLAYM4_PRIVT_DATA     4


typedef struct
{
    long nType;
    long nStamp;
    long nFrameNum;
    long nBufLen;
    char* pBuf;
    PLAYM4_SYSTEM_TIME  stSysTime;
}RECORD_DATA_INFO;

/////////////////////////////////////////////////////////////////////////////////////////////
//Unproposed Interface Not Recommended Interface
/////////////////////////////////////////////////////////////////////////////////////////////
PLAYM4_API BOOL __stdcall PlayM4_RigisterDrawFun(int nPort,void (CALLBACK* DrawFun)(int nPort,HDC hDc,void* nUser),void* nUser);
PLAYM4_API BOOL __stdcall PlayM4_SetDecCallBack(int nPort,void (CALLBACK* DecCBFun)(int nPort,char * pBuf,long nSize,FRAME_INFO * pFrameInfo, void* nReserved1,void* nReserved2));
PLAYM4_API BOOL __stdcall PlayM4_SetDecCallBackEx(int nPort,void (CALLBACK* DecCBFun)(int nPort,char * pBuf,long nSize,FRAME_INFO * pFrameInfo, void* nReserved1,void* nReserved2), char* pDest, long nDestSize);
PLAYM4_API BOOL __stdcall PlayM4_SetSycStartTime(int nPort, PLAYM4_SYSTEM_TIME *pstSystemTime);
PLAYM4_API BOOL __stdcall PlayM4_SyncToAudio(int nPort, BOOL bSyncToAudio);
PLAYM4_API BOOL __stdcall PlayM4_SetDecodeEngine(int nPort, DWORD nDecodeEngine);
PLAYM4_API BOOL __stdcall PlayM4_SetRunTimeInfoCallBack(int nPort, void (CALLBACK* RunTimeInfoCBFun)(int nPort, RunTimeInfo* pstRunTimeInfo, void* pUser), void* pUser);

/////////////////////////////////////////////////////////////////////////////////////////////
//Unsupported API Not supported
/////////////////////////////////////////////////////////////////////////////////////////////
//Set pre-record switch, bFlag=1: Enable, bFlag=0: Disable
PLAYM4_API BOOL __stdcall PlayM4_SetPreRecordFlag(LONG lPort, BOOL bFlag);
//Callback function of pre-recorded data
PLAYM4_API BOOL __stdcall PlayM4_SetPreRecordCallBack(LONG lPort, void (CALLBACK* PreRecordCBfun)(int nPort, RECORD_DATA_INFO* pRecordDataInfo, void* pUser), void* pUser);

PLAYM4_API BOOL __stdcall PlayM4_InitDDraw(int hWnd);
PLAYM4_API BOOL __stdcall PlayM4_RealeseDDraw();
PLAYM4_API BOOL __stdcall PlayM4_SetFileEndMsg(int nPort,int hWnd,UINT nMsg);
PLAYM4_API int  __stdcall PlayM4_GetCaps();
PLAYM4_API BOOL __stdcall PlayM4_SetOverlayMode(int nPort,BOOL bOverlay,COLORREF colorKey);
PLAYM4_API LONG __stdcall PlayM4_GetOverlayMode(int nPort);
PLAYM4_API COLORREF __stdcall PlayM4_GetColorKey(int nPort);
PLAYM4_API BOOL __stdcall PlayM4_InitDDrawDevice();
PLAYM4_API void __stdcall PlayM4_ReleaseDDrawDevice();
PLAYM4_API DWORD __stdcall PlayM4_GetDDrawDeviceTotalNums();
PLAYM4_API BOOL __stdcall PlayM4_SetDDrawDevice(int nPort,DWORD nDeviceNum);
PLAYM4_API BOOL __stdcall PlayM4_GetDDrawDeviceInfo(DWORD nDeviceNum,LPSTR  lpDriverDescription,DWORD nDespLen,LPSTR lpDriverName ,DWORD nNameLen,HMONITOR *hhMonitor);
PLAYM4_API int  __stdcall PlayM4_GetCapsEx(DWORD nDDrawDeviceNum);
PLAYM4_API BOOL __stdcall PlayM4_SetDDrawDeviceEx(int nPort,DWORD nRegionNum,DWORD nDeviceNum);
PLAYM4_API BOOL __stdcall PlayM4_OpenStreamEx(int nPort,PBYTE pFileHeadBuf,DWORD nSize,DWORD nBufPoolSize);
PLAYM4_API BOOL __stdcall PlayM4_CloseStreamEx(int nPort);
PLAYM4_API BOOL __stdcall PlayM4_InputVideoData(int nPort,PBYTE pBuf,DWORD nSize);
PLAYM4_API BOOL __stdcall PlayM4_InputAudioData(int nPort,PBYTE pBuf,DWORD nSize);
PLAYM4_API BOOL __stdcall PlayM4_SetTimerType(int nPort,DWORD nTimerType,DWORD nReserved);
PLAYM4_API BOOL __stdcall PlayM4_GetTimerType(int nPort,DWORD *pTimerType,DWORD *pReserved);
PLAYM4_API BOOL __stdcall PlayM4_SetVerifyCallBack(int nPort, DWORD nBeginTime, DWORD nEndTime, void (__stdcall * funVerify)(int nPort, FRAME_POS * pFilePos, DWORD bIsVideo, DWORD nUser),  DWORD  nUser);
PLAYM4_API BOOL __stdcall PlayM4_SetAudioCallBack(int nPort, void (__stdcall * funAudio)(int nPort, char * pAudioBuf, long nSize, long nStamp, long nType, long nUser), long nUser);
PLAYM4_API BOOL __stdcall PlayM4_SetEncChangeMsg(int nPort,int hWnd,UINT nMsg);
PLAYM4_API BOOL __stdcall PlayM4_GetOriginalFrameCallBack(int nPort, BOOL bIsChange,BOOL bNormalSpeed,long nStartFrameNum,long nStartStamp,long nFileHeader,void(CALLBACK *funGetOrignalFrame)(int nPort,FRAME_TYPE *frameType, long nUser),long nUser);
PLAYM4_API BOOL __stdcall PlayM4_GetFileSpecialAttr(int nPort, DWORD *pTimeStamp,DWORD *pFileNum ,DWORD *pReserved);
PLAYM4_API BOOL __stdcall PlayM4_SetPlayMode(int nPort,BOOL bNormal);
PLAYM4_API BOOL __stdcall PlayM4_SetOverlayFlipMode(int nPort,BOOL bTrue);
PLAYM4_API BOOL __stdcall PlayM4_SetDeflash(int nPort,BOOL bDefalsh);
PLAYM4_API BOOL __stdcall PlayM4_SetPicQuality(int nPort,BOOL bHighQuality);
PLAYM4_API BOOL __stdcall PlayM4_GetPictureQuality(int nPort,BOOL *bHighQuality);

int PlayM4_WndResolutionChange(int nPort);//new add
int  PlayM4_SetVideoWindow(int nPort, unsigned int nRegionNum, PLAYM4_HWND hWnd);
#endif //_PLAYM4_H_
