#ifndef __LINUX_PLAYM4_H__
#define __LINUX_PLAYM4_H__

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
#define  PLAYM4_NOERROR					0	//no error
#define	 PLAYM4_PARA_OVER				1	//input parameter is invalid;
#define  PLAYM4_ORDER_ERROR				2	//The order of the function to be called is error.
#define	 PLAYM4_TIMER_ERROR				3	//Create multimedia clock failed;
#define  PLAYM4_DEC_VIDEO_ERROR			4	//Decode video data failed.
#define  PLAYM4_DEC_AUDIO_ERROR			5	//Decode audio data failed.
#define	 PLAYM4_ALLOC_MEMORY_ERROR		6	//Allocate memory failed.
#define  PLAYM4_OPEN_FILE_ERROR			7	//Open the file failed.
#define  PLAYM4_CREATE_OBJ_ERROR		8	//Create thread or event failed
//#define  PLAYM4_CREATE_DDRAW_ERROR		9	//Create DirectDraw object failed.
//#define  PLAYM4_CREATE_OFFSCREEN_ERROR 10	//failed when creating off-screen surface.
#define  PLAYM4_BUF_OVER			   11	//buffer is overflow
#define  PLAYM4_CREATE_SOUND_ERROR	   12	//failed when creating audio device.
#define	 PLAYM4_SET_VOLUME_ERROR	   13	//Set volume failed
#define  PLAYM4_SUPPORT_FILE_ONLY	   14	//The function only support play file.
#define  PLAYM4_SUPPORT_STREAM_ONLY	   15	//The function only support play stream.
#define  PLAYM4_SYS_NOT_SUPPORT		   16	//System not support.
#define  PLAYM4_FILEHEADER_UNKNOWN     17	//No file header.
#define  PLAYM4_VERSION_INCORRECT	   18	//The version of decoder and encoder is not adapted.
#define  PLAYM4_INIT_DECODER_ERROR     19	//Initialize decoder failed.
#define  PLAYM4_CHECK_FILE_ERROR	   20	//The file data is unknown.
#define  PLAYM4_INIT_TIMER_ERROR	   21	//Initialize multimedia clock failed.
#define	 PLAYM4_BLT_ERROR		       22	//Display failed.
//#define  PLAYM4_UPDATE_ERROR		   23	//Update failed.
#define  PLAYM4_OPEN_FILE_ERROR_MULTI  24   //openfile error, streamtype is multi
#define  PLAYM4_OPEN_FILE_ERROR_VIDEO  25   //openfile error, streamtype is video
#define  PLAYM4_JPEG_COMPRESS_ERROR    26   //JPEG compress error
#define  PLAYM4_EXTRACT_NOT_SUPPORT    27	//Don't support the version of this file.
#define  PLAYM4_EXTRACT_DATA_ERROR     28	//extract video data failed.
#define  PLAYM4_SECRET_KEY_ERROR       29	//Secret key is error //add 20071218
#define  PLAYM4_DECODE_KEYFRAME_ERROR  30   //add by hy 20090318
#define  PLAYM4_NEED_MORE_DATA         31   //add by hy 20100617
#define  PLAYM4_INVALID_PORT		   32	//add by cj 20100913
#define  PLAYM4_NOT_FIND               33	//add by cj 20110428
#define  PLAYM4_NEED_LARGER_BUFFER     34  //add by pzj 20130528
#define  PLAYM4_FAIL_UNKNOWN		   99   //Fail, but the reason is unknown;

//Error Code of Fisheye Module
#define PLAYM4_FEC_ERR_ENABLEFAIL				100 // Loading fisheye module failed.
#define PLAYM4_FEC_ERR_NOTENABLE				101 // The fisheye module is not loaded.
#define PLAYM4_FEC_ERR_NOSUBPORT				102 // The sub port is not allocated.
#define PLAYM4_FEC_ERR_PARAMNOTINIT				103 // No initialized port parameters.
#define PLAYM4_FEC_ERR_SUBPORTOVER				104 // The sub port is occupied.
#define PLAYM4_FEC_ERR_EFFECTNOTSUPPORT			105 // This function is not supported under this mounting type.
#define PLAYM4_FEC_ERR_INVALIDWND				106 // Illegal window.
#define PLAYM4_FEC_ERR_PTZOVERFLOW				107 // PTZ position is out of limit.
#define PLAYM4_FEC_ERR_RADIUSINVALID			108 // Illegal parameters of the circle center
#define PLAYM4_FEC_ERR_UPDATENOTSUPPORT			109 // Specified mounting type and dewarping effect. Updating parameters is not supported.
#define PLAYM4_FEC_ERR_NOPLAYPORT				110 // The player port is not enabled.
#define PLAYM4_FEC_ERR_PARAMVALID				111 // The parameter is empty.
#define PLAYM4_FEC_ERR_INVALIDPORT				112 // Illegal sub port
#define PLAYM4_FEC_ERR_PTZZOOMOVER				113 // The PTZ dewarping range is out of limit.
#define PLAYM4_FEC_ERR_OVERMAXPORT				114 // No more dewarping channel is allowed. Up to four channels are supported.
#define PLAYM4_FEC_ERR_ENABLED                  115 // The fisheye module of this port is enabled.
#define PLAYM4_FEC_ERR_D3DACCENOTENABLE			116 // D3D acceleration is not enabled-Error code of fisheye in the Windows system-not available for Linux system
#define PLAYM4_FEC_ERR_PLACETYPE                117 // Invalid mounting type, one player port corresponds to one mounting type.
#define PLAYM4_FEC_ERR_NULLWND                  118 // The configured fisheye window hWnd is null or the fisheye port window changed.
#define PLAYM4_FEC_ERR_CorrectType              119 // The dewarping type already exists. Only one dewarping type can be enabled for one player port (except PTZ dewarping). And only one channel is supported when setting 180 panorama dewarping.

//Max display regions.
#define MAX_DISPLAY_WND 4

//Display type
#define DISPLAY_NORMAL            0x00000001
#define DISPLAY_QUARTER           0x00000002
#define DISPLAY_YC_SCALE          0x00000004	//add by gb 20091116
#define DISPLAY_NOTEARING         0x00000008
//Display buffers
#define MAX_DIS_FRAMES 50
#define MIN_DIS_FRAMES 1

//Locate by
#define BY_FRAMENUM  1
#define BY_FRAMETIME 2

//Source buffer
#define SOURCE_BUF_MAX	1024*100000
#define SOURCE_BUF_MIN	1024*50

//Stream type
#define STREAME_REALTIME 0
#define STREAME_FILE	 1

//frame type
#define T_AUDIO16	101
#define T_AUDIO8	100
#define T_UYVY		1
#define T_YV12		3
#define T_RGB32		7

//capability
#define SUPPORT_DDRAW		1
#define SUPPORT_BLT         2
#define SUPPORT_BLTFOURCC   4
#define SUPPORT_BLTSHRINKX  8
#define SUPPORT_BLTSHRINKY  16
#define SUPPORT_BLTSTRETCHX 32
#define SUPPORT_BLTSTRETCHY 64
#define SUPPORT_SSE         128
#define SUPPORT_MMX			256

// The following macro definitions are for HIK_MEDIAINFO structure
#define FOURCC_HKMI			0x484B4D49	// Mark of "HKMI" HIK_MEDIAINFO structure
// System packaging format
#define SYSTEM_NULL			0x0				// No system layer, audio stream or video stream
#define SYSTEM_HIK          0x1				// Hikvision file layer
#define SYSTEM_MPEG2_PS     0x2				// PS packaging
#define SYSTEM_MPEG2_TS     0x3				// TS packaging
#define SYSTEM_RTP          0x4				// RTP packaging
#define SYSTEM_RTPHIK       0x401				// RTP packaging

// Video encoding type
#define VIDEO_NULL          0x0 // No video
#define VIDEO_H264          0x1 // This definition is available for both the standard H.264 and hivision H.264
#define VIDEO_MPEG2			0x2	// Standard MPEG2
#define VIDEO_MPEG4         0x3 // Standard MPEG4
#define VIDEO_MJPEG			0x4
#define VIDEO_AVC264        0x0100

// Audio Encoding Type
#define AUDIO_NULL          0x0000 // No audio
#define AUDIO_ADPCM         0x1000 // ADPCM
#define AUDIO_MPEG          0x2000 // MPEG series audio, the decoder self-adapts to every MPEG audio.
#define AUDIO_AAC           0x2001 // AAC
#define AUDIO_RAW_DATA8     0x7000 // Original data with the sampling rate of 8k
#define AUDIO_RAW_UDATA16   0x7001 // Original data with the sampling rate of 16k, i.e., L16
// G Series Audio
#define AUDIO_RAW_DATA8		0x7000      //Original data with the sampling rate of 8k
#define AUDIO_RAW_UDATA16	0x7001      //Original data with the sampling rate of 16k, i.e., L16
#define AUDIO_G711_U		0x7110
#define AUDIO_G711_A		0x7111
#define AUDIO_G722_1		0x7221
#define AUDIO_G723_1        0x7231
#define AUDIO_G726_U        0x7260
#define AUDIO_G726_A        0x7261
#define AUDIO_G726_16       0x7262
#define AUDIO_G729          0x7290
#define AUDIO_AMR_NB		0x3000

#define SYNCDATA_VEH	    1 //Synchronize data: Mobile information
#define SYNCDATA_IVS	    2 //Synchronize data: VCA information

//motion flow type
#define	MOTION_FLOW_NONE			0
#define MOTION_FLOW_CPU				1
#define MOTION_FLOW_GPU				2

//Encryption Type of Audio and Video
#define ENCRYPT_AES_3R_VIDEO     1
#define ENCRYPT_AES_10R_VIDEO    2
#define ENCRYPT_AES_3R_AUDIO     1
#define ENCRYPT_AES_10R_AUDIO    2


#ifndef PLAYM4_ADDITION_INFO_TAG
#define PLAYM4_ADDITION_INFO_TAG
typedef struct _PLAYM4_ADDITION_INFO_     //Interaction information structure
{
    unsigned char*  pData;			//Attachment data
    unsigned int    dwDatalen;		//Length of attachment data
    unsigned int	dwDataType;		//Data type
    unsigned int	dwTimeStamp;	//Relative time stamp
} PLAYM4_ADDITION_INFO;
#endif

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
typedef struct
{
    long long nFilePos;
    int nFrameNum;
    int nFrameTime;
    int nErrorFrameNum;
    SYSTEMTIME *pErrorTime;
    int nErrorLostFrameNum;
    int nErrorFrameSize;
}FRAME_POS, *PFRAME_POS;

//Frame Info
typedef struct
{
    int nWidth;
    int nHeight;
    int nStamp;
    int nType;
    int nFrameRate;
    unsigned int dwFrameNum;
}FRAME_INFO;

//Frame
typedef struct
{
    char *pDataBuf;
    int  nSize;
    int  nFrameNum;
    int  bIsAudio;
    int  nReserved;
}FRAME_TYPE;

//Watermark Info	//add by gb 080119
typedef struct
{
    char *pDataBuf;
    int  nSize;
    int  nFrameNum;
    int  bRsaRight;
    int  nReserved;
}WATERMARK_INFO;

typedef struct SYNCDATA_INFO
{
    unsigned int dwDataType;        //Attachment information type which is synchronous with the stream data: VCA Information, Mobile Information
    unsigned int dwDataLen;         //Length of attachment information
    unsigned char* pData;           //Pointer of attachment information structure, e.g., IVS_INFO structure
} SYNCDATA_INFO;

#ifndef _HIK_MEDIAINFO_FLAG_
#define _HIK_MEDIAINFO_FLAG_
typedef struct _HIK_MEDIAINFO_				// modified by gb 080425
{
    unsigned int    media_fourcc;			// "HKMI": 0x484B4D49 Hikvision Media Information
    unsigned short  media_version;			// Version No.: The version No. of this information structure, the current version 0x0101, i.e., Version 1.01, 01-Main version                                               No., 01: Sub version No.
    unsigned short  device_id;				// Device ID, which is for tracking or analysis

    unsigned short  system_format;          // System Packaging Layer
    unsigned short  video_format;           // Video Encoding Type

    unsigned short  audio_format;           // Audio Encoding Type
    unsigned char   audio_channels;         // Channel Number
    unsigned char   audio_bits_per_sample;  // Bit Rate
    unsigned int    audio_samplesrate;      // Sampling Rate
    unsigned int    audio_bitrate;          // Compressed audio stream rate, unit: bit

    unsigned int    reserved[4];            // Reserved
}HIK_MEDIAINFO;
#endif

typedef struct
{
    int nPort;
    char* pBuf;
    int nBufLen;
    int nWidth;
    int nHeight;
    int nStamp;
    int nType;
    void* nUser;
}DISPLAY_INFO;

typedef struct
{
    int nPort;
    char *pVideoBuf;
    int nVideoBufLen;
    char *pPriBuf;
    int nPriBufLen;
    int nWidth;
    int nHeight;
    int nStamp;
    int nType;
    void* nUser;
}DISPLAY_INFOEX;

typedef struct PLAYM4_SYSTEM_TIME //Absolute Time
{
    unsigned int dwYear; //Year
    unsigned int dwMon;  //Month
    unsigned int dwDay;  //Day
    unsigned int dwHour; //Time
    unsigned int dwMin;  //Minute
    unsigned int dwSec;  //Second
    unsigned int dwMs;   //Millisecond
} PLAYM4_SYSTEM_TIME;

#ifndef CROP_PIC_INFO_TAG
#define CROP_PIC_INFO_TAG
typedef struct
{
    unsigned char* pDataBuf;      //Buffer for captured data
    unsigned int   dwPicSize;	  //Actual picture size
    unsigned int   dwBufSize;      //Data buffer size
    unsigned int   dwPicWidth;	  //Width of captured picture
    unsigned int   dwPicHeight;    //Height of captured picture
    unsigned int   dwReserve;      //Reserved
    HKRECT*        pCropRect;     //The selected region is NULL, which is same with the old API
}CROP_PIC_INFO;
#endif


//ENCRYPT Info
typedef struct{
    long nVideoEncryptType;  //Video encryption type
    long nAudioEncryptType;  //Audio encryption type
    long nSetSecretKey;      //Set or not? 1- Set key, 0- No key is set
}ENCRYPT_INFO;

// Private Information Module Type
typedef enum _PLAYM4_PRIDATA_RENDER
{
    PLAYM4_RENDER_ANA_INTEL_DATA   = 0x00000001, //VCA
    PLAYM4_RENDER_MD               = 0x00000002, //Motion Detection
    PLAYM4_RENDER_ADD_POS          = 0x00000004, //POS Information Overlay
    PLAYM4_RENDER_ADD_PIC          = 0x00000008, //Picture Information Overlay
    PLAYM4_RENDER_FIRE_DETCET      = 0x00000010, //Thermal Imaging Information
    PLAYM4_RENDER_TEM              = 0x00000020, //Temperature Information
}PLAYM4_PRIDATA_RENDER;

typedef enum _PLAYM4_FIRE_ALARM
{
    PLAYM4_FIRE_FRAME_DIS           = 0x00000001, //Display Fire Source Frame
    PLAYM4_FIRE_MAX_TEMP            = 0x00000002, //Highest Temperature
    PLAYM4_FIRE_MAX_TEMP_POSITION   = 0x00000004, //Display Position of Highest Temperature
    PLAYM4_FIRE_DISTANCE            = 0x00000008, //Distance from Highest Temperature
}PLAYM4_FIRE_ALARM;

typedef enum _PLAYM4_TEM_FLAG
{
    PLAYM4_TEM_REGION_BOX             = 0x00000001, //Measure by rectangle
    PLAYM4_TEM_REGION_LINE            = 0x00000002, //Measure by line
    PLAYM4_TEM_REGION_POINT           = 0x00000004, //Measure by point
}PLAYM4_TEM_FLAG;


//////////////////////////////////////////////////////////////////////////////
//Recommended APIs
//////////////////////////////////////////////////////////////////////////////
///<Initialize
int  PlayM4_GetPort(int* nPort);
int  PlayM4_FreePort(int nPort);
int  PlayM4_OpenFile(int nPort,char * sFileName);
int  PlayM4_CloseFile(int nPort);
int  PlayM4_SetStreamOpenMode(int nPort,unsigned int nMode);
int  PlayM4_GetStreamOpenMode(int nPort);
int  PlayM4_OpenStream(int nPort,unsigned char * pFileHeadBuf,unsigned int nSize,unsigned int nBufPoolSize);
int  PlayM4_CloseStream(int nPort);
int  PlayM4_InputData(int nPort,unsigned char * pBuf,unsigned int nSize);

///<Index
int  PlayM4_SetFileRefCallBack(int nPort, void (CALLBACK *pFileRefDone)(unsigned int nPort,void* nUser),void* nUser);
int  PlayM4_GetRefValue(int nPort,unsigned char  *pBuffer, unsigned int *pSize);
int  PlayM4_GetRefValueEx(int nPort,unsigned char  *pBuffer, unsigned int *pSize);///<Add frame type callback
int  PlayM4_SetRefValue(int nPort,unsigned char  *pBuffer, unsigned int nSize);
int  PlayM4_GetKeyFramePos(int nPort,unsigned int nValue, unsigned int nType, PFRAME_POS pFramePos);
int  PlayM4_GetNextKeyFramePos(int nPort,unsigned int nValue, unsigned int nType, PFRAME_POS pFramePos);

///<Play Control
int  PlayM4_Play(int nPort, PLAYM4_HWND hWnd);
int  PlayM4_Stop(int nPort);
int  PlayM4_Pause(int nPort,unsigned int nPause);
int  PlayM4_Fast(int nPort);
int  PlayM4_Slow(int nPort);
int  PlayM4_OneByOne(int nPort);
int  PlayM4_OneByOneBack(int nPort);
int  PlayM4_ReversePlay(int nPort);
int  PlayM4_RefreshPlay(int nPort);
int  PlayM4_RefreshPlayEx(int nPort, unsigned int nRegionNum);

///<Play Audio
int  PlayM4_PlaySound(int nPort);
int  PlayM4_StopSound();
int  PlayM4_PlaySoundShare(int nPort);
int  PlayM4_StopSoundShare(int nPort);
int  PlayM4_SetVolume(int nPort,unsigned short nVolume);
unsigned short  PlayM4_GetVolume(int nPort);

///<Get Information
int  PlayM4_SetPlayPos(int nPort,float fRelativePos);
float  PlayM4_GetPlayPos(int nPort);
unsigned int  PlayM4_GetFileTime(int nPort);
unsigned int  PlayM4_GetPlayedTime(int nPort);
unsigned int  PlayM4_GetPlayedFrames(int nPort);
unsigned int  PlayM4_GetFileTotalFrames(int nPort);
unsigned int  PlayM4_GetCurrentFrameRate(int nPort);
unsigned int  PlayM4_GetPlayedTimeEx(int nPort);
int           PlayM4_SetPlayedTimeEx(int nPort,unsigned int nTime);
unsigned int  PlayM4_GetCurrentFrameNum(int nPort);
int           PlayM4_SetCurrentFrameNum(int nPort,unsigned int nFrameNum);
int           PlayM4_GetSystemTime(int nPort, PLAYM4_SYSTEM_TIME *pstSystemTime);
unsigned int  PlayM4_GetSpecialData(int nPort);
int           PlayM4_GetPictureSize(int nPort,int *pWidth,int *pHeight);
int           PlayM4_SetFileEndCallback(int nPort, void(CALLBACK*FileEndCallback)(int nPort, void *pUser), void *pUser);

///<Cache Operation
unsigned int  PlayM4_GetSourceBufferRemain(int nPort);
int           PlayM4_ResetSourceBuffer(int nPort);
int           PlayM4_ResetBuffer(int nPort,unsigned int nBufType);
unsigned int  PlayM4_GetBufferValue(int nPort,unsigned int nBufType);
int           PlayM4_SetDisplayBuf(int nPort, unsigned int nNum);
unsigned int  PlayM4_GetDisplayBuf(int nPort);

///<Capture
int  PlayM4_SetJpegQuality(int nQuality);
int  PlayM4_GetBMP(int nPort,unsigned char * pBitmap,unsigned int nBufSize,unsigned int* pBmpSize);
int  PlayM4_GetJPEG(int nPort,unsigned char * pJpeg,unsigned int nBufSize,unsigned int* pJpegSize);
int  PlayM4_ConvertToBmpFile(char * pBuf,int nSize,int nWidth,int nHeight,int nType,char *sFileName);
int  PlayM4_ConvertToJpegFile(char * pBuf,int nSize,int nWidth,int nHeight,int nType,char *sFileName);

///<Decode
int  PlayM4_SetSecretKey(int nPort, int lKeyType, char *pSecretKey, int lKeyLen);
int  PlayM4_ThrowBFrameNum(int nPort,unsigned int nNum);
int  PlayM4_SetDecCBStream(int nPort,unsigned int nStream);
int  PlayM4_SetDecodeFrameType(int nPort,unsigned int nFrameType);
int  PlayM4_SkipErrorData(int nPort, int bSkip);
int  PlayM4_SetDecCallBackMend(int nPort,void (CALLBACK* DecCBFun)(int nPort,char * pBuf,int nSize,FRAME_INFO * pFrameInfo, void* nUser,int nReserved2), void* nUser);
int  PlayM4_SetDecCallBackExMend(int nPort, void (CALLBACK* DecCBFun)(int nPort, char* pBuf, int nSize, FRAME_INFO* pFrameInfo, void* nUser, int nReserved2), char* pDest, int nDestSize, void* nUser);
// Encryption stream callback, nType=0: Call back when the stream encryption mark changed. nType=1: The encryption bit of the stream is called back
int  PlayM4_SetEncryptTypeCallBack(int nPort, unsigned int nType, void (CALLBACK* EncryptTypeCBFun)(int nPort, ENCRYPT_INFO* pEncryptInfo, void* nUser, int nReserved2), void* nUser);

///<Display
int  PlayM4_SetDisplayRegion(int nPort,unsigned int nRegionNum, HKRECT *pSrcRect, PLAYM4_HWND hDestWnd, int bEnable);///<Digital Zoom
int  PlayM4_SetDisplayRegionOnWnd(int nPort,unsigned int nRegionNum, HKRECT *pSrcRect, int bEnable);///<Window Division
int  PlayM4_SetDisplayCallBack(int nPort,void (CALLBACK* DisplayCBFun)(int nPort,char * pBuf,int nSize,int nWidth,int nHeight,int nStamp,int nType,int nReserved));
int  PlayM4_SetDisplayCallBackEx(int nPort,void (CALLBACK* DisplayCBFun)(DISPLAY_INFO *pstDisplayInfo), void* nUser);
int  PlayM4_RegisterDrawFun(int nPort,void (CALLBACK* DrawFun)(int nPort,PLAYM4_HDC hDc,void* nUser),void* nUser);
int  PlayM4_SetEncTypeChangeCallBack(int nPort, void(CALLBACK *funEncChange)(int nPort, void* nUser), void* nUser);
int  PlayM4_SetCheckWatermarkCallBack(int nPort, void(CALLBACK* funCheckWatermark)(int nPort, WATERMARK_INFO* pWatermarkInfo, void* nUser), void* nUser);
int  PlayM4_SetDisplayType(int nPort, int nType);
int  PlayM4_GetDisplayType(int nPort);

// VCA Information Switch
int PlayM4_RenderPrivateData(int nPort, int nIntelType, int bTrue);
int PlayM4_RenderPrivateDataEx(int nPort, int nIntelType, int nSubType, int bTrue);
PLAYM4_API int __stdcall PlayM4_SetConfigFontPath(int nPort, char* pFontPath);///<Set font library path for OSD- Set before playing.

//lType: 1  refers to get the PTZ information of displayed frame. It is stored in the pInfo with specified structure type, and plLen returns length information. First you should set pInfo = null to get the memory length (plLen) needed to allocate.
PLAYM4_API int __stdcall PlayM4_GetStreamAdditionalInfo(int nPort, int lType, unsigned char* pInfo, int* plLen);

///<Process Image
#define R_ANGLE_0   -1  //No rotation
#define R_ANGLE_L90  0  //Rotate left 90 degree
#define R_ANGLE_R90  1  //Rotate right 90 degree
#define R_ANGLE_180  2  //Rotate 180 degree

PLAYM4_API int __stdcall PlayM4_SetRotateAngle(int nPort, unsigned int nRegionNum, unsigned int dwType);

///<Others
unsigned int  PlayM4_GetFileHeadLength();
unsigned int  PlayM4_GetSdkVersion();
unsigned int  PlayM4_GetLastError(int nPort);
//The predefined value of dwGroupIndex is ranging from 0 to 3. For the first version, synchronization function is canceled, so it should be processed in same closestream
PLAYM4_API int __stdcall PlayM4_SetSycGroup(int nPort, unsigned int dwGroupIndex);

///<SDP Streaming//////////////////////////////////////////////////////////////////////////
#ifndef PLAYM4_SESSION_INFO_TAG
#define PLAYM4_SESSION_INFO_TAG
//nProtocolType
#define PLAYM4_PROTOCOL_RTSP    1
//nSessionInfoType
#define PLAYM4_SESSION_INFO_SDP 1

typedef struct _PLAYM4_SESSION_INFO_     //Interaction information structure
{
    int            nSessionInfoType;   //Interaction information type, only SDP is supported.
    int            nSessionInfoLen;    //Length of interaction information
    unsigned char* pSessionInfoData;   //Interaction information data

} PLAYM4_SESSION_INFO;
#endif

PLAYM4_API int __stdcall PlayM4_OpenStreamAdvanced(int nPort, int nProtocolType, PLAYM4_SESSION_INFO* pstSessionInfo, unsigned int nBufPoolSize);

///<Fisheye Related//////////////////////////////////////////////////////////////////////////
#ifndef FISH_EYE_TAG
#define FISH_EYE_TAG

// Mounting Type
typedef enum tagFECPlaceType
{
    FEC_PLACE_WALL    = 0x1, // Wall Mounting (horizontal normal)
    FEC_PLACE_FLOOR   = 0x2, // Ground Mounting (upward normal)
    FEC_PLACE_CEILING = 0x3, // Ceiling Mounting (downward normal)

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
    float	fRadiusLeft;	// X-Coordinate of circle left
    float	fRadiusRight;	// X-Coordinate of circle right
    float   fRadiusTop;		// Y-Coordinate of circle top
    float   fRadiusBottom;	// Y-Coordinate of circle bottom

}CYCLEPARAM;

typedef struct tagPTZParam
{
    float fPTZPositionX;		// Center position of PTZ display, X-Coordinate
    float fPTZPositionY;		// Center position of PTZ display, Y-Coordinate

}PTZPARAM;

// Update Mark Variable Definitions

#define 		FEC_UPDATE_RADIUS			 0x1
#define 		FEC_UPDATE_PTZZOOM			 0x2
#define 		FEC_UPDATE_WIDESCANOFFSET	 0x4
#define 		FEC_UPDATE_PTZPARAM			 0x8
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
    unsigned int 	nUpDateType;			// Updated type

    unsigned int	nPlaceAndCorrect;		// Mounting type and dewarping type, get only. When SetParam is invalid, this value is the sum of mounting type and dewarping

    PTZPARAM		stPTZParam;				// PTZ dewarping parameters

    CYCLEPARAM		stCycleParam;			// Circle center parameter of fisheye image

    float			fZoom;					// PTZ display parameter

    float			fWideScanOffset;		// Offset angle of 180 degree or 360 panoramic dewarping
    FECCOLOR        stPTZColor;             // PTZ Color
    int				nResver[15];			// Reserved field

}FISHEYEPARAM;

// PTZ Display Mode on Original Fisheye Image
typedef enum tagFECShowMode
{
    FEC_PTZ_OUTLINE_NULL, // No display
    FEC_PTZ_OUTLINE_RECT, // Display in rectangle
    FEC_PTZ_OUTLINE_RANGE, // Display real region
}FECSHOWMODE;

#define    FEC_JPEG   0  // JPEG Capture
#define    FEC_BMP    1  // BMP Capture

typedef void (__stdcall * FISHEYE_CallBack )(void* pUser, unsigned int nPort, unsigned int nCBType, void * hDC, unsigned int nWidth, unsigned int nHeight);

#endif



// Enable fisheye
PLAYM4_API int __stdcall PlayM4_FEC_Enable(int nPort);

// Close fisheye module
PLAYM4_API int __stdcall PlayM4_FEC_Disable(int nPort);

// Get the sub port of fisheye dewarping [1~31]
PLAYM4_API int  __stdcall PlayM4_FEC_GetPort(int nPort , unsigned int* nSubPort , FECPLACETYPE emPlaceType , FECCORRECTTYPE emCorrectType);

// Delete the sub port of fisheye dewarping
PLAYM4_API int __stdcall PlayM4_FEC_DelPort(int nPort , unsigned int nSubPort);

// Set fisheye dewarping parameters
PLAYM4_API int __stdcall PlayM4_FEC_SetParam(int nPort , unsigned int nSubPort , FISHEYEPARAM * pPara);

// Get fisheye dewarping parameters
PLAYM4_API int __stdcall PlayM4_FEC_GetParam(int nPort , unsigned int nSubPort , FISHEYEPARAM * pPara);

// Set display window, which can be switched at any time
PLAYM4_API int __stdcall PlayM4_FEC_SetWnd(int nPort , unsigned int nSubPort , void * hWnd);

// Set drawing callback of fisheye window
PLAYM4_API int __stdcall PlayM4_FEC_SetCallBack(int nPort , unsigned int nSubPort , FISHEYE_CallBack cbFunc , void * pUser);

// Fisheye Capture
PLAYM4_API int __stdcall PlayM4_FEC_Capture(int nPort, unsigned int nSubPort, unsigned int nType, char* pFileName);

PLAYM4_API int __stdcall PlayM4_FEC_GetCurrentPTZPort(int nPort, float fPositionX,float fPositionY, unsigned int *pnPort);

PLAYM4_API int __stdcall PlayM4_FEC_SetCurrentPTZPort(int nPort, unsigned int nSubPort);

PLAYM4_API int __stdcall PlayM4_FEC_SetPTZOutLineShowMode(int nPort,FECSHOWMODE nPTZShowMode);


//Image Enhancement Related --Unsupported APIs//////////////////////////////////////////////////////////////////////////
#ifndef PLAYM4_HIKVIE_TAG
#define PLAYM4_HIKVIE_TAG

typedef struct _PLAYM4_VIE_DYNPARAM_
{
    int moduFlag;      //Enabled algorithm processing module, see the definitions in PLAYM4_VIE_MODULES
    //Èç PLAYM4_VIE_MODU_ADJ | PLAYM4_VIE_MODU_EHAN
    //After enabling module, the corresponding parameters should be configured.
    //PLAYM4_VIE_MODU_ADJ
    int brightVal;     //Brightness , [-255, 255]
    int contrastVal;   //Contrast, [-256, 255]
    int colorVal;      //Saturation, [-256, 255]
    //PLAYM4_VIE_MODU_EHAN
    int toneScale;     //Filtering range, [0, 100]
    int toneGain;      //Contrast, Global contrast gain, [-256, 255]
    int toneOffset;    //Brightness, Average brightness offset, [-255, 255]
    int toneColor;     //Color, color fidelity, [-256, 255]
    //PLAYM4_VIE_MODU_DEHAZE
    int dehazeLevel;   //Defog strength, [0, 255]
    int dehazeTrans;   //Dehaze strength, [0, 255]
    int dehazeBright;  //illumination compensation, [0, 255]
    //PLAYM4_VIE_MODU_DENOISE
    int denoiseLevel;  //Denoise intensity, [0, 255]
    //PLAYM4_VIE_MODU_SHARPEN
    int usmAmount;     //Sharpen strength, [0, 255]
    int usmRadius;     //Sharpen radius, [1, 15]
    int usmThreshold;  //Sharpen threshold, [0, 255]
    //PLAYM4_VIE_MODU_DEBLOCK
    int deblockLevel;  //Sharpen strength, [0, 100]
    //PLAYM4_VIE_MODU_LENS
    int lensWarp;      //Distortion, [-256, 255]
    int lensZoom;      //Zoom, [-256, 255]
    //PLAYM4_VIE_MODU_CRB
    //No response parameter
} PLAYM4_VIE_PARACONFIG;

typedef enum _PLAYM4_VIE_MODULES
{
    PLAYM4_VIE_MODU_ADJ      = 0x00000001, //Basic Adjustment of Image
    PLAYM4_VIE_MODU_EHAN     = 0x00000002, //Local Enhancement Module
    PLAYM4_VIE_MODU_DEHAZE   = 0x00000004, //Defog Module
    PLAYM4_VIE_MODU_DENOISE  = 0x00000008, //Denoise Module
    PLAYM4_VIE_MODU_SHARPEN  = 0x00000010, //Sharpen Module
    PLAYM4_VIE_MODU_DEBLOCK  = 0x00000020, //Deblock and Filtering Module
    PLAYM4_VIE_MODU_CRB      = 0x00000040, //Color Balance Module
    PLAYM4_VIE_MODU_LENS     = 0x00000080, //Image Distortion Correction Module
}PLAYM4_VIE_MODULES;
#endif

//Disable/Enable Module
//Macros PLAYM4_VIE_MODULES of dwModuFlag, which can be combined.
//First enable module, and then set module parameters; Before setting, the default parameters will be adopted.
//After disabling the module, the previous settings will be cleared.
//Before calling other APIs, you should enable the module via this API, otherwise, error will be returned.
PLAYM4_API int __stdcall PlayM4_VIE_SetModuConfig(int nPort, int nModuFlag, int bEnable);

//Set image enhancement region, NULL-Full Image. If the image is larger than full image, adopts full image. The minimum region is 16*16 pixels.
//Supports setting region, up to 4 regions are supported. For the first version, only one region is supported. The regions cannot be overlaid, otherwise, error occurs.
PLAYM4_API int __stdcall PlayM4_VIE_SetRegion(int nPort, int nRegNum, HKRECT* pRect);

//Get enabled module
PLAYM4_API int __stdcall PlayM4_VIE_GetModuConfig(int nPort, int* pdwModuFlag);

//Set parameters
//The disabled parameter settings will be ignored.
PLAYM4_API int __stdcall PlayM4_VIE_SetParaConfig(int nPort, PLAYM4_VIE_PARACONFIG* pParaConfig);

//Get enabled module parameters
PLAYM4_API int __stdcall PlayM4_VIE_GetParaConfig(int nPort, PLAYM4_VIE_PARACONFIG* pParaConfig);




#define PLAYM4_MEDIA_HEAD     1   //System header data
#define PLAYM4_VIDEO_DATA     2   //Video stream data
#define PLAYM4_AUDIO_DATA     3   //Audio stream data
#define PLAYM4_PRIVT_DATA     4   //Private stream data

//Pre-record Data Information--Unsupported APIs////////////////////////
typedef struct
{
    long nType;                     // Data type, e.g., file header, video, audio, private data, and so on.
    long nStamp;                    // Time stamp
    long nFrameNum;                 // Frame No.
    long nBufLen;                   // Data length
    char* pBuf;                     // Frame data, callback by frame
    PLAYM4_SYSTEM_TIME  stSysTime;  // Global Time
}RECORD_DATA_INFO;

//Enable pre-record or not, bFlag=1: Enable, bFlag=0: Disable
PLAYM4_API int __stdcall PlayM4_SetPreRecordFlag(int nPort, int bFlag);

//Pre-record data callback
PLAYM4_API int __stdcall PlayM4_SetPreRecordCallBack(int nPort, void (CALLBACK* PreRecordCBfun)(int nPort, RECORD_DATA_INFO* pRecordDataInfo, void* pUser), void* pUser);


typedef struct
{
    long    lDataType;          //Private data type
    long    lDataStrVersion;    //Structure version returned by data, which is mainly for compatibility
    long    lDataTimeStamp;
    long    lDataLength;
    char*   pData;
}AdditionDataInfo;

PLAYM4_API int __stdcall PlayM4_SetAdditionDataCallBack(int nPort, unsigned int nSyncType, void (CALLBACK* AdditionDataCBFun)(int nPort, AdditionDataInfo* pstAddDataInfo, void* pUser), void* pUser);

///<Real-Time Information Callback
#define PLAYM4_SOURCE_MODULE             0 // Data Source Module
#define PLAYM4_DEMUX_MODULE              1 // Analysis Module
#define PLAYM4_DECODE_MODULE             2 // Decoding Module
#define PLAYM4_RENDER_MODULE             3 // Render Module

#define PLAYM4_RTINFO_SOFTDECODE_ERROR   1 // Software decoding error
#define PLAYM4_RTINFO_MEDIAHEADER_ERROR  2 // Media header error
#define PLAYM4_RTINFO_ALLOC_MEMORY_ERROR 4 // Allocating memory failed

typedef struct
{
    int            nRunTimeModule;     //Running module
    int            nStrVersion;        //Structure version returned by data, which is mainly for compatibility. In the first version, it is defined as 0x0001
    int            nFrameTimeStamp;    //Frame No.
    int            nFrameNum;          //Time stamp
    int            nErrorCode;         //Error code
    unsigned char  reserved[12];       //Reserved byte
}RunTimeInfo;

int PlayM4_SetRunTimeInfoCallBackEx(int nPort, int nModule, void (CALLBACK* RunTimeInfoCBFun)(int nPort, RunTimeInfo* pstRunTimeInfo, void* pUser), void* pUser);


///<Window Size Changed Notification
PLAYM4_API int __stdcall PlayM4_WndResolutionChange(int nPort);//new add

/////////////////////////////////////////////////////////////////////////////

//Unrecommended APIs
/////////////////////////////////////////////////////////////////////////////////////////////
int  PlayM4_RigisterDrawFun(int nPort,void (CALLBACK* DrawFun)(int nPort,PLAYM4_HDC hDc,void* nUser),void* nUser);
int  PlayM4_SetDecCallBack(int nPort,void (CALLBACK* DecCBFun)(int nPort,char* pBuf,int nSize,FRAME_INFO * pFrameInfo, void* nReserved1,int nReserved2));
int  PlayM4_SetDecCallBackEx(int nPort,void (CALLBACK* DecCBFun)(int nPort,char * pBuf,int nSize,FRAME_INFO * pFrameInfo, void* nReserved1,int nReserved2), char* pDest, int nDestSize);
int  PlayM4_SetVideoWindow(int nPort, unsigned int nRegionNum, PLAYM4_HWND hWnd);
int  PlayM4_SetColor(int nPort, unsigned int nRegionNum, int nBrightness, int nContrast, int nSaturation, int nHue);
int  PlayM4_GetColor(int nPort, unsigned int nRegionNum, int* pBrightness, int* pContrast, int* pSaturation, int* pHue);
int  PlayM4_GetFileTimeEx(int nPort, unsigned int* pStart, unsigned int* pStop, unsigned int* pRev);
int  PlayM4_GetCurrentFrameRateEx(int nPort, float* pfFrameRate);
int  PlayM4_SyncToAudio(int nPort, int bSyncToAudio);
int  PlayM4_SetImageSharpen(int nPort, unsigned int nLevel);
int  PlayM4_ResetSourceBufFlag(int nPort);
int  PlayM4_SetSourceBufCallBack(int nPort, unsigned int nThreShold, void (CALLBACK* SourceBufCallBack)(int nPort, unsigned int nBufSize, unsigned int dwUser, void* pResvered), unsigned int dwUser, void* pReserved);
unsigned int PlayM4_GetAbsFrameNum(int nPort);
int PlayM4_SetRunTimeInfoCallBack(int nPort, void (CALLBACK* RunTimeInfoCBFun)(int nPort, RunTimeInfo* pstRunTimeInfo, void* pUser), void* pUser);

/////////////////////////////////////////////////////////////////////////////////////////////
////////////////NO SUPPORT///////////////////////////////////////////////////

int          PlayM4_InitDDraw(PLAYM4_HWND hWnd);
int          PlayM4_RealeseDDraw();
#if (WINVER >= 0x0400)
//Note: These function must be built under win2000 or above with Microsoft System SDK.
//You can download the sdk from "http://www.microsoft.com/msdownload/platformsdk/sdkupdate/";
int          PlayM4_InitDDrawDevice();
void         PlayM4_ReleaseDDrawDevice();
int          PlayM4_SetDDrawDevice(int nPort, unsigned int nDeviceNum);
int          PlayM4_SetDDrawDeviceEx(int nPort,unsigned int nRegionNum,unsigned int nDeviceNum);
int          PlayM4_GetDDrawDeviceInfo(unsigned int nDeviceNum, char* lpDriverDescription, unsigned int nDespLen, char* lpDriverName, unsigned int nNameLen, HMONITOR* hhMonitor);
int          PlayM4_GetCapsEx(unsigned int nDDrawDeviceNum);
unsigned int PlayM4_GetDDrawDeviceTotalNums();
#endif
int          PlayM4_SetFileEndMsg(int nPort, PLAYM4_HWND hWnd, unsigned int nMsg);
int          PlayM4_GetCaps();
int          PlayM4_OpenStreamEx(int nPort, unsigned char* pFileHeadBuf, unsigned int nSize, unsigned int nBufPoolSize);
int          PlayM4_CloseStreamEx(int nPort);
int          PlayM4_InputVideoData(int nPort, unsigned char* pBuf, unsigned int nSize);
int          PlayM4_InputAudioData(int nPort, unsigned char* pBuf, unsigned int nSize);
//int          PlayM4_SetOverlayMode(int nPort, int bOverlay, COLORREF colorKey);
int          PlayM4_GetOverlayMode(int nPort);
int          PlayM4_SetOverlayFlipMode(int nPort, int bTrue);
//COLORREF     PlayM4_GetColorKey(int nPort);
int          PlayM4_SetPicQuality(int nPort, int bHighQuality);
int          PlayM4_GetPictureQuality(int nPort, int* bHighQuality);
int          PlayM4_AdjustWaveAudio(int nPort, int nCoefficient);
int          PlayM4_SetPlayMode(int nPort, int bNormal);
int          PlayM4_SetDeflash(int nPort, int bDefalsh);
int          PlayM4_CheckDiscontinuousFrameNum(int nPort, int bCheck);
int          PlayM4_SetVerifyCallBack(int nPort, unsigned int nBeginTime, unsigned int nEndTime, void (__stdcall* funVerify)(int nPort, FRAME_POS* pFilePos, unsigned int bIsVideo, unsigned int nUser), unsigned int nUser);
int          PlayM4_SetEncChangeMsg(int nPort, PLAYM4_HWND hWnd, unsigned int nMsg);
int          PlayM4_SetGetUserDataCallBack(int nPort, void(CALLBACK* funGetUserData)(int nPort, unsigned char* pUserBuf, unsigned int nBufLen, unsigned int nUser), unsigned int nUser);
int          PlayM4_GetOriginalFrameCallBack(int nPort, int bIsChange, int bNormalSpeed, int nStartFrameNum, int nStartStamp, int nFileHeader, void(CALLBACK *funGetOrignalFrame)(int nPort, FRAME_TYPE* frameType, int nUser), int nUser);
int          PlayM4_GetFileSpecialAttr(int nPort, unsigned int* pTimeStamp, unsigned int* pFileNum, unsigned int* pReserved);
int          PlayM4_GetThrowBFrameCallBack(int nPort, void(CALLBACK* funThrowBFrame)(int nPort, unsigned int nBFrame, unsigned int nUser), unsigned int nUser);
int          PlayM4_SetAudioCallBack(int nPort, void (__stdcall* funAudio)(int nPort, char* pAudioBuf, int nSize, int nStamp, int nType, int nUser), int nUser);
//motionflow Multiple Frame
PLAYM4_API int __stdcall PlayM4_MotionFlow(int nPort, unsigned int dwAdjustType);

int  PlayM4_SetTimerType(int nPort,unsigned int nTimerType,unsigned int nReserved);
int  PlayM4_GetTimerType(int nPort,unsigned int *pTimerType,unsigned int *pReserved);
int  PlayM4_SetDisplayMode(int nPort, unsigned int dwType);
//This function is not available.
PLAYM4_API int __stdcall PlayM4_SetSycStartTime(int nPort, PLAYM4_SYSTEM_TIME *pstSystemTime);


//Set hard or soft decoder engine

#define SOFT_DECODE_ENGINE 0 ///<Software Decoding
#define HARD_DECODE_ENGINE 1 ///<Hardware Decoding

int  PlayM4_SetDecodeEngine(int nPort, unsigned int nDecodeEngine);
int  PlayM4_SetDecodeEngineEx(int nPort, unsigned int nDecodeEngine);
unsigned int PlayM4_GetDecodeEngine(int nPort);

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


int PlayM4_GetEngineSupport(int nPort, ENGINESUPPORT* pstEngineSupport);
////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
    }
#endif

#endif //_PLAYM4_H_
