#ifndef __LINUX_PLAYM4_H__
#define __LINUX_PLAYM4_H__

#ifdef __cplusplus
extern "C"
{
#endif

typedef unsigned int PLAYM4_HWND;
typedef void * PLAYM4_HDC;

#define  BOOL  int
typedef unsigned int   COLORKEY;
typedef unsigned int   COLORREF;
#define PLAYM4_API

//#define __stdcall

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
#define BUF_VIDEO_SRC      (1) //mixed input,total src buffer size;splited input,video src buffer size
#define BUF_AUDIO_SRC      (2) //mixed input,not defined;splited input,audio src buffer size
#define BUF_VIDEO_RENDER   (3) //video render node count or node cout for decoded data
#define BUF_AUDIO_RENDER   (4) //audio render node count
#define BUF_VIDEO_DECODED  (5) //video decoded node count to render
#define BUF_AUDIO_DECODED  (6) //audio decoded node count to render

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
#define  PLAYM4_NOT_FIND      33	//add by cj 20110428
#define  PLAYM4_NEED_LARGER_BUFFER     34  //add by pzj 20130528
#define  PLAYM4_FAIL_UNKNOWN		   99   //Fail, but the reason is unknown;

//ÓãÑÛ¹¦ÄÜ´íÎóÂë
#define PLAYM4_FEC_ERR_ENABLEFAIL				100 // ÓãÑÛÄ£¿é¼ÓÔØÊ§°Ü
#define PLAYM4_FEC_ERR_NOTENABLE				101 // ÓãÑÛÄ£¿éÃ»ÓÐ¼ÓÔØ
#define PLAYM4_FEC_ERR_NOSUBPORT				102 // ×Ó¶Ë¿ÚÃ»ÓÐ·ÖÅä
#define PLAYM4_FEC_ERR_PARAMNOTINIT				103 // Ã»ÓÐ³õÊ¼»¯¶ÔÓ¦¶Ë¿ÚµÄ²ÎÊý
#define PLAYM4_FEC_ERR_SUBPORTOVER				104 // ×Ó¶Ë¿ÚÒÑ¾­ÓÃÍê
#define PLAYM4_FEC_ERR_EFFECTNOTSUPPORT			105 // ¸Ã°²×°·½Ê½ÏÂÕâÖÖÐ§¹û²»Ö§³Ö
#define PLAYM4_FEC_ERR_INVALIDWND				106 // ·Ç·¨µÄ´°¿Ú
#define PLAYM4_FEC_ERR_PTZOVERFLOW				107 // PTZÎ»ÖÃÔ½½ç
#define PLAYM4_FEC_ERR_RADIUSINVALID			108 // Ô²ÐÄ²ÎÊý·Ç·¨
#define PLAYM4_FEC_ERR_UPDATENOTSUPPORT			109 // Ö¸¶¨µÄ°²×°·½Ê½ºÍ½ÃÕýÐ§¹û£¬¸Ã²ÎÊý¸üÐÂ²»Ö§³Ö
#define PLAYM4_FEC_ERR_NOPLAYPORT				110 // ²¥·Å¿â¶Ë¿ÚÃ»ÓÐÆôÓÃ
#define PLAYM4_FEC_ERR_PARAMVALID				111 // ²ÎÊýÎª¿Õ
#define PLAYM4_FEC_ERR_INVALIDPORT				112 // ·Ç·¨×Ó¶Ë¿Ú
#define PLAYM4_FEC_ERR_PTZZOOMOVER				113 // PTZ½ÃÕý·¶Î§Ô½½ç
#define PLAYM4_FEC_ERR_OVERMAXPORT				114 // ½ÃÕýÍ¨µÀ±¥ºÍ£¬×î´óÖ§³ÖµÄ½ÃÕýÍ¨µÀÎªËÄ¸ö
#define PLAYM4_FEC_ERR_ENABLED         115 // ¸Ã¶Ë¿ÚÒÑ¾­ÆôÓÃÁËÓãÑÛÄ£¿é
#define PLAYM4_FEC_ERR_D3DACCENOTENABLE			116 // D3D¼ÓËÙÃ»ÓÐ¿ªÆô-windowsÓãÑÛ´íÎóÂë-LinuxÎÞ
#define PLAYM4_FEC_ERR_PLACETYPE       117 // °²×°·½Ê½²»¶Ô.Ò»¸ö²¥·Å¿âport£¬¶ÔÓ¦Ò»ÖÖ°²×°·½Ê½
#define PLAYM4_FEC_ERR_NULLWND         118 // ÉèÖÃµÄÓãÑÛ´°¿ÚhWndÎªnull»òÕßÓãÑÛportµÄ´°¿Ú¸ü»»
#define PLAYM4_FEC_ERR_CorrectType     119 // ½ÃÕý·½Ê½ÒÑÓÐ,²»ÄÜ¿ª¶à¸ö,Ò»¸ö²¥·Å¿âport,³ýÁËPTZ,ÆäËû½ÃÕý·½Ê½Ö»ÄÜ¿ªÒ»Â·;ÇÒÉèÖÃ180¶È½ÃÕýÊ±Ö»ÄÜÎªµ¥¶ÀµÄÒ»Â·¡£

//Max display regions.
#define MAX_DISPLAY_WND 4

//Display type
#define DISPLAY_NORMAL   0x00000001
#define DISPLAY_QUARTER  0x00000002
#define DISPLAY_YC_SCALE 0x00000004	//add by gb 20091116
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

// ÒÔÏÂºê¶¨ÒåÓÃÓÚHIK_MEDIAINFO½á¹¹
#define FOURCC_HKMI			0x484B4D49	// "HKMI" HIK_MEDIAINFO½á¹¹±ê¼Ç
// ÏµÍ³·â×°¸ñÊ½
#define SYSTEM_NULL			0x0				// Ã»ÓÐÏµÍ³²ã£¬´¿ÒôÆµÁ÷»òÊÓÆµÁ÷
#define SYSTEM_HIK 0x1				// º£¿µÎÄ¼þ²ã
#define SYSTEM_MPEG2_PS     0x2				// PS·â×°
#define SYSTEM_MPEG2_TS     0x3				// TS·â×°
#define SYSTEM_RTP 0x4				// rtp·â×°
#define SYSTEM_RTPHIK       0x401				// rtp·â×°

// ÊÓÆµ±àÂëÀàÐÍ
#define VIDEO_NULL 0x0 // Ã»ÓÐÊÓÆµ
#define VIDEO_H264 0x1 // ±ê×¼H.264ºÍº£¿µH.264¶¼¿ÉÒÔÓÃÕâ¸ö¶¨Òå
#define VIDEO_MPEG2			0x2	// ±ê×¼MPEG2
#define VIDEO_MPEG4         0x3 // ±ê×¼MPEG4
#define VIDEO_MJPEG			0x4
#define VIDEO_AVC264        0x0100

// ÒôÆµ±àÂëÀàÐÍ
#define AUDIO_NULL 0x0000 // Ã»ÓÐÒôÆµ
#define AUDIO_ADPCM         0x1000 // ADPCM
#define AUDIO_MPEG 0x2000 // MPEG ÏµÁÐÒôÆµ£¬½âÂëÆ÷ÄÜ×ÔÊÊÓ¦¸÷ÖÖMPEGÒôÆµ
#define AUDIO_AAC  0x2001 // AAC
#define AUDIO_RAW_DATA8     0x7000 //²ÉÑùÂÊÎª8kµÄÔ­Ê¼Êý¾Ý
#define AUDIO_RAW_UDATA16   0x7001 //²ÉÑùÂÊÎª16kµÄÔ­Ê¼Êý¾Ý£¬¼´L16
// GÏµÁÐÒôÆµ
#define AUDIO_RAW_DATA8		0x7000      //²ÉÑùÂÊÎª8kµÄÔ­Ê¼Êý¾Ý
#define AUDIO_RAW_UDATA16	0x7001      //²ÉÑùÂÊÎª16kµÄÔ­Ê¼Êý¾Ý£¬¼´L16
#define AUDIO_G711_U		0x7110
#define AUDIO_G711_A		0x7111
#define AUDIO_G722_1		0x7221
#define AUDIO_G723_1        0x7231
#define AUDIO_G726_U        0x7260
#define AUDIO_G726_A        0x7261
#define AUDIO_G726_16       0x7262
#define AUDIO_G729 0x7290
#define AUDIO_AMR_NB		0x3000

#define SYNCDATA_VEH	    1 //Í¬²½Êý¾Ý:³µÔØÐÅÏ¢
#define SYNCDATA_IVS	    2 //Í¬²½Êý¾Ý:ÖÇÄÜÐÅÏ¢

//motion flow type
#define	MOTION_FLOW_NONE			0
#define MOTION_FLOW_CPU				1
#define MOTION_FLOW_GPU				2

//ÒôÊÓÆµ¼ÓÃÜÀàÐÍ
#define ENCRYPT_AES_3R_VIDEO     1
#define ENCRYPT_AES_10R_VIDEO    2
#define ENCRYPT_AES_3R_AUDIO     1
#define ENCRYPT_AES_10R_AUDIO    2


#ifndef PLAYM4_ADDITION_INFO_TAG
#define PLAYM4_ADDITION_INFO_TAG
typedef struct _PLAYM4_ADDITION_INFO_     //½»»¥ÐÅÏ¢½á¹¹
{
    unsigned char*  pData;			//¸½¼þÊý¾Ý
    unsigned int    dwDatalen;		//¸½¼þÊý¾Ý³¤¶È
    unsigned int	dwDataType;		//Êý¾ÝÀàÐÍ
    unsigned int	dwTimeStamp;	//Ïà¶ÔÊ±¼ä´Á
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
    unsigned int left;
    unsigned int top;
    unsigned int right;
    unsigned int bottom;
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
}FRAME_POS,*PFRAME_POS;

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
    unsigned int dwDataType;        //ºÍÂëÁ÷Êý¾ÝÍ¬²½µÄ¸½ÊôÐÅÏ¢ÀàÐÍ£¬Ä¿Ç°ÓÐ£ºÖÇÄÜÐÅÏ¢£¬³µÔØÐÅÏ¢
    unsigned int dwDataLen;         //¸½ÊôÐÅÏ¢Êý¾Ý³¤¶È
    unsigned char* pData;  //Ö¸Ïò¸½ÊôÐÅÏ¢Êý¾Ý½á¹¹µÄÖ¸Õë,±ÈÈçIVS_INFO½á¹¹
} SYNCDATA_INFO;

#ifndef _HIK_MEDIAINFO_FLAG_
#define _HIK_MEDIAINFO_FLAG_
typedef struct _HIK_MEDIAINFO_				// modified by gb 080425
{
    unsigned int    media_fourcc;			// "HKMI": 0x484B4D49 Hikvision Media Information
    unsigned short  media_version;			// °æ±¾ºÅ£ºÖ¸±¾ÐÅÏ¢½á¹¹°æ±¾ºÅ£¬Ä¿Ç°Îª0x0101,¼´1.01°æ±¾£¬01£ºÖ÷°æ±¾ºÅ£»01£º×Ó°æ±¾ºÅ¡£
    unsigned short  device_id;				// Éè±¸ID£¬±ãÓÚ¸ú×Ù/·ÖÎö

    unsigned short  system_format; // ÏµÍ³·â×°²ã
    unsigned short  video_format;  // ÊÓÆµ±àÂëÀàÐÍ

    unsigned short  audio_format;  // ÒôÆµ±àÂëÀàÐÍ
    unsigned char   audio_channels;         // Í¨µÀÊý
    unsigned char   audio_bits_per_sample;  // ÑùÎ»ÂÊ
    unsigned int    audio_samplesrate;      // ²ÉÑùÂÊ
    unsigned int    audio_bitrate; // Ñ¹ËõÒôÆµÂëÂÊ,µ¥Î»£ºbit

    unsigned int    reserved[4];   // ±£Áô
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

typedef struct PLAYM4_SYSTEM_TIME //¾ø¶ÔÊ±¼ä
{
    unsigned int dwYear; //Äê
    unsigned int dwMon;  //ÔÂ
    unsigned int dwDay;  //ÈÕ
    unsigned int dwHour; //Ê±
    unsigned int dwMin;  //·Ö
    unsigned int dwSec;  //Ãë
    unsigned int dwMs;   //ºÁÃë
} PLAYM4_SYSTEM_TIME;

#ifndef CROP_PIC_INFO_TAG
#define CROP_PIC_INFO_TAG
typedef struct
{
    unsigned char* pDataBuf;      //×¥Í¼Êý¾Ýbuffer
    unsigned int   dwPicSize;	  //Êµ¼ÊÍ¼Æ¬´óÐ¡
    unsigned int   dwBufSize;      //Êý¾Ýbuffer´óÐ¡
    unsigned int   dwPicWidth;	  //½ØÍ¼¿í
    unsigned int   dwPicHeight;    //½ØÍ¼¸ß
    unsigned int   dwReserve;      //¶à¼ÓÒ»¸öreserve×Ö¶Î
    HKRECT*        pCropRect;     //Ñ¡ÔñÇøÓòNULL, Í¬ÀÏµÄ×¥Í¼½Ó¿Ú
}CROP_PIC_INFO;
#endif


//ENCRYPT Info
typedef struct{
    int nVideoEncryptType;  //ÊÓÆµ¼ÓÃÜÀàÐÍ
    int nAudioEncryptType;  //ÒôÆµ¼ÓÃÜÀàÐÍ
    int nSetSecretKey;      //ÊÇ·ñÉèÖÃ£¬1±íÊ¾ÉèÖÃÃÜÔ¿£¬0±íÊ¾Ã»ÓÐÉèÖÃÃÜÔ¿
}ENCRYPT_INFO;

// Ë½ÓÐÐÅÏ¢Ä£¿éÀàÐÍ
typedef enum _PLAYM4_PRIDATA_RENDER
{
    PLAYM4_RENDER_ANA_INTEL_DATA   = 0x00000001, //ÖÇÄÜ·ÖÎö
    PLAYM4_RENDER_MD      = 0x00000002, //ÒÆ¶¯Õì²â
    PLAYM4_RENDER_ADD_POS = 0x00000004, //POSÐÅÏ¢ºóµþ¼Ó
    PLAYM4_RENDER_ADD_PIC = 0x00000008, //Í¼Æ¬µþ¼ÓÐÅÏ¢
    PLAYM4_RENDER_FIRE_DETCET      = 0x00000010, //ÈÈ³ÉÏñÐÅÏ¢
    PLAYM4_RENDER_TEM     = 0x00000020, //ÎÂ¶ÈÐÅÏ¢
}PLAYM4_PRIDATA_RENDER;

typedef enum _PLAYM4_FIRE_ALARM
{
    PLAYM4_FIRE_FRAME_DIS  = 0x00000001, //»ðµã¿òÏÔÊ¾
    PLAYM4_FIRE_MAX_TEMP   = 0x00000002, //×î¸ßÎÂ¶È
    PLAYM4_FIRE_MAX_TEMP_POSITION   = 0x00000004, //×î¸ßÎÂ¶ÈÎ»ÖÃÏÔÊ¾
    PLAYM4_FIRE_DISTANCE   = 0x00000008, //×î¸ßÎÂ¶È¾àÀë
}PLAYM4_FIRE_ALARM;

typedef enum _PLAYM4_TEM_FLAG
{
    PLAYM4_TEM_REGION_BOX    = 0x00000001, //¿ò²âÎÂ
    PLAYM4_TEM_REGION_LINE   = 0x00000002, //Ïß²âÎÂ
    PLAYM4_TEM_REGION_POINT  = 0x00000004, //µã²âÎÂ
}PLAYM4_TEM_FLAG;


//////////////////////////////////////////////////////////////////////////////
//API-Proposed Interface ½¨ÒéÊ¹ÓÃµÄ½Ó¿Ú
//////////////////////////////////////////////////////////////////////////////
///<³õÊ¼»¯

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
int PlayM4_SetFileEndMsg(int nPort, PLAYM4_HWND hWnd, unsigned int nMsg);
int PlayM4_SetVolume(int nPort,unsigned short nVolume);
int PlayM4_StopSound();
int PlayM4_PlaySound(int nPort);
int PlayM4_OpenStream(int nPort,unsigned char * pFileHeadBuf,unsigned int nSize,unsigned int nBufPoolSize);
int PlayM4_InputData(int nPort,unsigned char * pBuf,unsigned int nSize);
int PlayM4_CloseStream(int nPort);
int PlayM4_GetCaps();
unsigned int  PlayM4_GetFileTime(int nPort);
unsigned int  PlayM4_GetPlayedTime(int nPort);
unsigned int  PlayM4_GetPlayedFrames(int nPort);

//23
////////////////ver 2.0 added///////////////////////////////////////
int  PlayM4_SetDecCallBack(int nPort,void (CALLBACK* DecCBFun)(int nPort,char* pBuf,int nSize,FRAME_INFO * pFrameInfo, void* nReserved1,int nReserved2));
int    PlayM4_SetDisplayCallBack(int nPort,void (CALLBACK* DisplayCBFun)(int nPort,char * pBuf,int nSize,int nWidth,int nHeight,int nStamp,int nType,int nReserved));
int    PlayM4_ConvertToBmpFile(char * pBuf,int nSize,int nWidth,int nHeight,int nType,char *sFileName);
unsigned int     PlayM4_GetFileTotalFrames(int nPort);
unsigned int     PlayM4_GetCurrentFrameRate(int nPort);
unsigned int     PlayM4_GetPlayedTimeEx(int nPort);
int    PlayM4_SetPlayedTimeEx(int nPort,unsigned int nTime);
unsigned int     PlayM4_GetCurrentFrameNum(int nPort);
int    PlayM4_SetStreamOpenMode(int nPort,unsigned int nMode);
unsigned int     PlayM4_GetFileHeadLength();
unsigned int     PlayM4_GetSdkVersion();
//11
////////////////ver 2.2 added///////////////////////////////////////
unsigned int   PlayM4_GetLastError(int nPort);
int PlayM4_RefreshPlay(int nPort);
int PlayM4_SetOverlayMode(int nPort,BOOL bOverlay,COLORREF colorKey);
int PlayM4_GetPictureSize(int nPort,int *pWidth,int *pHeight);
int PlayM4_SetPicQuality(int nPort,BOOL bHighQuality);
int PlayM4_PlaySoundShare(int nPort);
int PlayM4_StopSoundShare(int nPort);
//7
////////////////ver 2.4 added///////////////////////////////////////
int PlayM4_GetStreamOpenMode(int nPort);
int PlayM4_GetOverlayMode(int nPort);
COLORREF PlayM4_GetColorKey(int nPort);
unsigned short PlayM4_GetVolume(int nPort);
int PlayM4_GetPictureQuality(int nPort,BOOL *bHighQuality);
unsigned int  PlayM4_GetSourceBufferRemain(int nPort);
int PlayM4_ResetSourceBuffer(int nPort);
int PlayM4_SetSourceBufCallBack(int nPort,unsigned int nThreShold,void (CALLBACK * SourceBufCallBack)(int nPort,unsigned int nBufSize,unsigned int dwUser,void*pResvered),unsigned int dwUser,void *pReserved);
int PlayM4_ResetSourceBufFlag(int nPort);
int PlayM4_SetDisplayBuf(int nPort,unsigned int nNum);
unsigned int  PlayM4_GetDisplayBuf(int nPort);
int PlayM4_OneByOneBack(int nPort);
int  PlayM4_SetFileRefCallBack(int nPort, void (CALLBACK *pFileRefDone)(unsigned int nPort,void* nUser),void* nUser);
int PlayM4_SetCurrentFrameNum(int nPort,unsigned int nFrameNum);
int PlayM4_GetKeyFramePos(int nPort,unsigned int nValue, unsigned int nType, PFRAME_POS pFramePos);
int PlayM4_GetNextKeyFramePos(int nPort,unsigned int nValue, unsigned int nType, PFRAME_POS pFramePos);
#if (WINVER >= 0x0400)
//Note: These funtion must be builded under win2000 or above with Microsoft Platform sdk.
//        You can download the sdk from "http://www.microsoft.com/msdownload/platformsdk/sdkupdate/";
int PlayM4_InitDDrawDevice();
PLAYM4_API void __stdcall PlayM4_ReleaseDDrawDevice();
unsigned int  PlayM4_GetDDrawDeviceTotalNums();
int PlayM4_SetDDrawDevice(int nPort,unsigned int nDeviceNum);
//int PlayM4_GetDDrawDeviceInfo(unsigned int nDeviceNum,char *  lpDriverDescription,unsigned int nDespLen,char * lpDriverName ,unsigned int nNameLen,HMONITOR *hhMonitor);
int PlayM4_GetCapsEx(unsigned int nDDrawDeviceNum);
#endif
int PlayM4_ThrowBFrameNum(int nPort,unsigned int nNum);
//23
////////////////ver 2.5 added///////////////////////////////////////
int PlayM4_SetDisplayType(int nPort,int nType);
int PlayM4_GetDisplayType(int nPort);
//2
////////////////ver 3.0 added///////////////////////////////////////
int PlayM4_SetDecCBStream(int nPort,unsigned int nStream);
int  PlayM4_SetDisplayRegion(int nPort,unsigned int nRegionNum, HKRECT *pSrcRect, PLAYM4_HWND hDestWnd, int bEnable);///<µç×Ó·Å´ó
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
int PlayM4_OpenStreamEx(int nPort,unsigned char * pFileHeadBuf,unsigned int nSize,unsigned int nBufPoolSize);
int PlayM4_CloseStreamEx(int nPort);
int PlayM4_InputVideoData(int nPort,unsigned char * pBuf,unsigned int nSize);
int PlayM4_InputAudioData(int nPort,unsigned char * pBuf,unsigned int nSize);
int PlayM4_RegisterDrawFun(int nPort,void (CALLBACK* DrawFun)(int nPort,PLAYM4_HDC hDc,int nUser),int nUser);
int PlayM4_RigisterDrawFun(int nPort,void (CALLBACK* DrawFun)(int nPort,PLAYM4_HDC hDc,int nUser),int nUser);
//8
//////////////////v3.4/////////////////////////////////////////////////////
int PlayM4_SetTimerType(int nPort,unsigned int nTimerType,unsigned int nReserved);
int PlayM4_GetTimerType(int nPort,unsigned int *pTimerType,unsigned int *pReserved);
int PlayM4_ResetBuffer(int nPort,unsigned int nBufType);
unsigned int  PlayM4_GetBufferValue(int nPort,unsigned int nBufType);

//////////////////V3.6/////////////////////////////////////////////////////////
int PlayM4_AdjustWaveAudio(int nPort,int nCoefficient);
int PlayM4_SetVerifyCallBack(int nPort, unsigned int nBeginTime, unsigned int nEndTime, void (CALLBACK* funVerify)(int nPort, FRAME_POS* pFilePos, unsigned int bIsVideo, unsigned int nUser), unsigned int nUser);
int PlayM4_SetAudioCallBack(int nPort, void (CALLBACK* funAudio)(int nPort, char* pAudioBuf, int nSize, int nStamp, int nType, int nUser), int nUser);
int PlayM4_SetEncTypeChangeCallBack(int nPort,void(CALLBACK *funEncChange)(int nPort,int nUser),int nUser);
int PlayM4_SetColor(int nPort, unsigned int nRegionNum, int nBrightness, int nContrast, int nSaturation, int nHue);
int PlayM4_GetColor(int nPort, unsigned int nRegionNum, int *pBrightness, int *pContrast, int *pSaturation, int *pHue);
int PlayM4_SetEncChangeMsg(int nPort,PLAYM4_HWND hWnd,unsigned int nMsg);
int PlayM4_GetOriginalFrameCallBack(int nPort, BOOL bIsChange,BOOL bNormalSpeed,int nStartFrameNum,int nStartStamp,int nFileHeader,void(CALLBACK *funGetOrignalFrame)(int nPort,FRAME_TYPE *frameType, int nUser),int nUser);
int PlayM4_GetFileSpecialAttr(int nPort, unsigned int *pTimeStamp,unsigned int *pFileNum ,unsigned int *pReserved);
unsigned int  PlayM4_GetSpecialData(int nPort);
int PlayM4_SetCheckWatermarkCallBack(int nPort,void(CALLBACK* funCheckWatermark)(int nPort,WATERMARK_INFO* pWatermarkInfo,unsigned int nUser),unsigned int nUser);
int PlayM4_SetImageSharpen(int nPort,unsigned int nLevel);
int PlayM4_SetDecodeFrameType(int nPort,unsigned int nFrameType);
int PlayM4_SetPlayMode(int nPort,BOOL bNormal);
int PlayM4_SetOverlayFlipMode(int nPort,BOOL bTrue);
//unsigned int  PlayM4_GetAbsFrameNum(int nPort);

//////////////////V4.7.0.0//////////////////////////////////////////////////////
////convert yuv to jpeg
int PlayM4_ConvertToJpegFile(char * pBuf,int nSize,int nWidth,int nHeight,int nType,char *sFileName);
int PlayM4_SetJpegQuality(int nQuality);
//set deflash
int PlayM4_SetDeflash(int nPort,BOOL bDefalsh);
//int PlayM4_SetDecCallBackEx(int nPort,void (CALLBACK* DecCBFun)(int nPort,char * pBuf,int nSize,FRAME_INFO * pFrameInfo, int nReserved1,int nReserved2), char* pDest, int nDestSize);
//////////////////V4.8.0.0/////////////////////////////////////////////////////////
//check discontinuous frame number as error data?
int PlayM4_CheckDiscontinuousFrameNum(int nPort, BOOL bCheck);
//get bmp or jpeg
int PlayM4_GetBMP(int nPort,unsigned char * pBitmap,unsigned int nBufSize,unsigned int* pBmpSize);
int PlayM4_GetJPEG(int nPort,unsigned char * pJpeg,unsigned int nBufSize,unsigned int* pJpegSize);
//dec call back mend
int  PlayM4_SetDecCallBackMend(int nPort,void (CALLBACK* DecCBFun)(int nPort,char * pBuf,int nSize,FRAME_INFO * pFrameInfo, void* nUser,int nReserved2), void* nUser);
int PlayM4_SetSecretKey(int nPort, int lKeyType, char *pSecretKey, int lKeyLen);

// add by gb 2007-12-23
int PlayM4_SetFileEndCallback(int nPort, void(CALLBACK*FileEndCallback)(int nPort, void *pUser), void *pUser);

// add by gb 080131 version 4.9.0.1
int PlayM4_GetPort(int* nPort);
int PlayM4_FreePort(int nPort);
int PlayM4_SetDisplayCallBackEx(int nPort,void (CALLBACK* DisplayCBFun)(DISPLAY_INFO *pstDisplayInfo), int nUser);
int PlayM4_SkipErrorData(int nPort, BOOL bSkip);
int  PlayM4_SetDecCallBackExMend(int nPort, void (CALLBACK* DecCBFun)(int nPort, char* pBuf, int nSize, FRAME_INFO* pFrameInfo, void* nUser, int nReserved2), char* pDest, int nDestSize, void* nUser);
//reverse play add by chenjie 110609
int PlayM4_ReversePlay(int nPort);
int PlayM4_GetSystemTime(int nPort, PLAYM4_SYSTEM_TIME *pstSystemTime);

//int PlayM4_SetDecodeERC(int nPort, unsigned int nLevel);

#ifndef PLAYM4_SESSION_INFO_TAG
#define PLAYM4_SESSION_INFO_TAG
//nProtocolType
#define PLAYM4_PROTOCOL_RTSP    1
//nSessionInfoType
#define PLAYM4_SESSION_INFO_SDP 1

typedef struct _PLAYM4_SESSION_INFO_     //SessionInfo structure
{
    int   nSessionInfoType;   //Session Info type, e.x. SDP
    int   nSessionInfoLen;    //Data length of Session Info
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
    BYTE*   pData;   //the attachment data
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
    FEC_CORRECT_PTZ = 0x100,        // PTZ viewing mode
    FEC_CORRECT_180 = 0x200,        //180¡ã viewing mode (2-screen-split mode)
    FEC_CORRECT_360 = 0x300,        // 360¡ã panoramic mode

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


#define         FEC_UPDATE_RADIUS    0x1
#define         FEC_UPDATE_PTZZOOM    0x2
#define         FEC_UPDATE_WIDESCANOFFSET     0x4
#define         FEC_UPDATE_PTZPARAM    0x8


typedef struct tagFECParam
{


    unsigned int     nUpDateType;   // Update type

    unsigned int    nPlaceAndCorrect;        // Sum of installation and correction mode   (read ¨Conly, invalid under SetParam mode)

    PTZPARAM        stPTZParam;       // Parameter for PTZ correction

    CYCLEPARAM        stCycleParam;   // Centre info of fisheye image

    float   fZoom;  //PTZ display range

    float   fWideScanOffset;        // Offset angle for 180¡ã or 360¡ã correction

    int       nResver[16];   // Reserved

}FISHEYEPARAM;

typedef void (CALLBACK * FISHEYE_CallBack )( void* pUser  , unsigned int  nSubPort , unsigned int nCBType , void * hDC ,   unsigned int nWidth , unsigned int nHeight);

#endif
// 6.121    Enable Fisheye APIs
int PlayM4_FEC_Enable(int nPort);

// 6.122    Disable Fisheye APIs
int PlayM4_FEC_Disable(int nPort);

// Get Sub-port for Fisheye Correction [1~31]
BOOL PlayM4_FEC_GetPort(int nPort, unsigned int* nSubPort,FECPLACETYPE emPlaceType,FECCORRECTTYPE emCorrectType);

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
int PlayM4_VIE_SetModuConfig(int lPort,int nModuFlag,BOOL bEnable);


//Set image region of image enhancement
int PlayM4_VIE_SetRegion(int lPort,int lRegNum,HKRECT* pRect);

//Get the enable module of image enhancement
int PlayM4_VIE_GetModuConfig(int lPort,int* pdwModuFlag);

//Set the parameter of the enable module of image enhancement
int PlayM4_VIE_SetParaConfig(int lPort,PLAYM4_VIE_PARACONFIG* pParaConfig);

//Get the parameter of the enable module of image enhancement
int PlayM4_VIE_GetParaConfig(int lPort,PLAYM4_VIE_PARACONFIG* pParaConfig);

//synchronization audio and video
int PlayM4_SyncToAudio(int nPort, BOOL bSyncToAudio);

// int PlayM4_EnableIVS(int nPort, BOOL bTrue);


typedef enum _PLAYM4_THERMAL_FLAG
{
    PLAYM4_THERMAL_FIREMASK = 0x00000001,
    PLAYM4_THERMAL_RULEGAS = 0x00000002,
    PLAYM4_THERMAL_TARGETGAS = 0x00000004
}PLAYM4_THERMAL_FLAG;

typedef enum _PLAYM4_TRACK_FLAG
{
    PLAYM4_TRACK_PEOPLE = 0x00000001,
    PLAYM4_TRACK_VEHICLE = 0x00000002
}PLAYM4_TRACK_FLAG;

typedef struct TI_PTZ_INFO_
{
    unsigned short dwDefVer;
    unsigned short dwLength;
    unsigned int dwP;    //P(0~3600)
    unsigned int dwT;         //T(0~3600)
    unsigned int dwZ;         //Z(0~3600)
    unsigned char        chFSMState;
    unsigned char  bClearFocusState;
    unsigned char        reserved[6];
}PTZ_INFO;



int PlayM4_RenderPrivateData(int nPort, int nIntelType, BOOL bTrue);

int PlayM4_RenderPrivateDataEx(int nPort, int nIntelType, int nSubType, BOOL bTrue);

int PlayM4_SetEncryptTypeCallBack(int nPort,void (CALLBACK* EncryptTypeCBFun)(int nPort, ENCRYPT_INFO* pEncryptInfo, int nReserved));

int PlayM4_GetStreamAdditionalInfo(int nPort, int lType, unsigned char* pInfo, int* plLen);


///<´°¿Ú´óÐ¡¸Ä±äÍ¨Öª½Ó¿Ú
int PlayM4_WndResolutionChange(int nPort);//new add
int  PlayM4_SetVideoWindow(int nPort, unsigned int nRegionNum, PLAYM4_HWND hWnd);

#ifdef __cplusplus
}
#endif

#endif //_PLAYM4_H_
