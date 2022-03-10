////////////////////////////////////////////////////////////////////////////////
//File:PlayerDlg.cpp 
/////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Player.h"
#include "PlayerDlg.h"
#include "vkey.h"
#include "info.h"
#include "cutfile.h"
#include "HelpDlg.h"
#include "CConvertAVI.h"
#include "watermarkinfo.h"
#include "SetKeyDlg.h"
#include <stdio.h>

#define GET_YEAR(_time_)      (((_time_)>>26) + 2000) 
#define GET_MONTH(_time_)     (((_time_)>>22) & 15)   
#define GET_DAY(_time_)       (((_time_)>>17) & 31)   
#define GET_HOUR(_time_)      (((_time_)>>12) & 31)   
#define GET_MINUTE(_time_)    (((_time_)>>6)  & 63)
#define GET_SECOND(_time_)    (((_time_)>>0)  & 63)   
void RunInfo(char *szFormat, ...)
{	
	char szInfo[512];
	va_list	ArgumentList;
	
	va_start(ArgumentList, szFormat); 
	vsprintf(szInfo, szFormat, ArgumentList);
	va_end(ArgumentList);
	OutputDebugString(szInfo);
}
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// class used to convert file to avi
CConvertAVI g_classAVI;

/************************************************************************/				
UINT	WM_FILE_END			= WM_USER +33;	 // User message. Be posted when the file is end.
UINT	WM_ENC_CHANGE		= WM_USER +100;  // User message. Be posted when the image size is changed.
UINT	PLAY_TIMER			= 1;			 // TIMER ID;
//UINT	m_lPort				= 0;			 // uesed port;can be from 0 to 15;
/************************************************************************/
LONG	m_lPort = -1;
WATERMARK_VER1_INFO m_strWaterMark;

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
/////////////////////////////////////////////////////////////////////////////
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	// display the version information
	CString strVer;

	DWORD dwVer	   = NAME(PlayM4_GetSdkVersion)();
	DWORD dwVersion = dwVer & 0x0000ffff;
	DWORD dwBuild   = (dwVer >> 16) & 0x0000ffff;
#ifdef _FOR_HIKPLAYM4_DLL_
	strVer.Format("HikPlayer ver is %.1f\n\rHikPlayM4.dll ver is %04x build is %04x", APP_VERSION, dwVersion, dwBuild);
#else
	strVer.Format("Player ver is %.1f\n\rPlayM4.dll ver is %04x build is %04x", APP_VERSION, dwVersion, dwBuild);
#endif
	GetDlgItem(IDC_VER)->SetWindowText(strVer);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlayerDlg dialog

CPlayerDlg::CPlayerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPlayerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPlayerDlg)
	m_strPlayStateText       = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon	                  =	AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hTestFile				  = INVALID_HANDLE_VALUE;	
	m_bInited				  = FALSE;
}

void CPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPlayerDlg)
	DDX_Control(pDX, IDC_PLAYSTATETEXT, m_ctrlPlayText);
	DDX_Control(pDX, IDC_CAPPIC, m_ctrlBtnCapPic);
	DDX_Control(pDX, IDC_SOUND, m_ctrlBtnSound);
	DDX_Control(pDX, IDC_STEPBACK, m_ctrlStepBackward);
	DDX_Control(pDX, IDC_STEP, m_ctrlStepForward);
	DDX_Control(pDX, IDC_SLOW, m_ctrlBtnSlow);
	DDX_Control(pDX, IDC_FAST, m_ctrlBtnFast);
	DDX_Control(pDX, IDC_GOTOSTART, m_ctrlBtnGStart);
	DDX_Control(pDX, IDC_GOTOEND, m_ctrlBtnGEnd);
	DDX_Control(pDX, IDC_STOP, m_ctrlBtnStop);
	DDX_Control(pDX, IDC_PLAY, m_ctrlBtnPlay);
	DDX_Control(pDX, IDC_PAUSE, m_ctrlBtnPause);
	DDX_Control(pDX, IDC_SHOW, m_ctrlVideoPic);
	DDX_Text(pDX, IDC_PLAYSTATETEXT, m_strPlayStateText);
	DDX_Control(pDX, IDC_SOUND_SLIDER, m_SoundSlider);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPlayerDlg, CDialog)
	//{{AFX_MSG_MAP(CPlayerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_KEYDOWN()
	ON_WM_HSCROLL()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_DROPFILES()
	ON_WM_MOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_GETMINMAXINFO()
	ON_WM_DESTROY()
	ON_WM_WINDOWPOSCHANGED()
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(IDM_VIEW_FULLSCREEN, IDM_SET_KEY, OnMenuItem)
	ON_COMMAND_RANGE(IDC_PLAY, IDC_SOUND, OnButtonItem)
	/*
	ON_MESSAGE(WM_FILE_END, PlayMessage)
	ON_MESSAGE(WM_ENC_CHANGE,EncChangeMessage)
	ON_MESSAGE(WM_SEEKOK, SeekOk)
	ON_MESSAGE(WM_DISPLAY_OK, DisplayOk)
	ON_MESSAGE(WM_VIDEOCTRL_OK, VideoCtrlOK)
	ON_MESSAGE(WM_WATERMARK_OK, WatermarkOk)
	*/
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlayerDlg message handlers

BOOL CPlayerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_pMainMenu = GetMenu();
	if(m_pMainMenu == NULL)
	{
		TRACE("GetMenu is Error!");
		return FALSE;
	}

	m_dwScreenWidth  = GetSystemMetrics(SM_CXSCREEN);
	m_dwScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	m_rcScreen.left  = 0;
	m_rcScreen.right = m_dwScreenWidth;
	m_rcScreen.top   = 0;
	m_rcScreen.bottom= m_dwScreenHeight;

#if (WINVER > 0x0400)
	// If do not support multi monitor,may not call!
	HMONITOR hMonitor;
	char chDriverDesp[50];
	char chDriverName[50];

	NAME(PlayM4_InitDDrawDevice)();
	DWORD nVal=NAME(PlayM4_GetDDrawDeviceTotalNums)();
	if(nVal > 1)
	{
		nVal = min(nVal, MAX_DISPLAY_DEVICE);
		for(DWORD nCount = 0; nCount < nVal; nCount++) 
		{
			ZeroMemory(chDriverDesp, 50);
			NAME(PlayM4_GetDDrawDeviceInfo)(nCount, chDriverDesp, 50, chDriverName, 50, &hMonitor);
			m_pMainMenu->GetSubMenu(1)->InsertMenu(IDM_TEMP_DEVICE, MF_BYCOMMAND, IDM_DEVICE0 + nCount + 1, chDriverDesp);
		}
	}
	else
	{
		ZeroMemory(chDriverDesp, 50);
		NAME(PlayM4_GetDDrawDeviceInfo)(0, chDriverDesp, 50, chDriverName, 50, &hMonitor);
		m_pMainMenu->GetSubMenu(1)->InsertMenu(IDM_TEMP_DEVICE, MF_BYCOMMAND, IDM_DEVICE0, chDriverDesp);
	}

	m_pMainMenu->GetSubMenu(1)->RemoveMenu(IDM_TEMP_DEVICE, MF_BYCOMMAND);
	m_pMainMenu->CheckMenuItem(IDM_DEVICE0, MF_CHECKED);
	
	TestCapability(0);
#endif

	
	// Init Play Slider
	VERIFY(m_PlaySlider.CreateFromStatic(
		   SBS_HORZ|WS_CHILD|SS_LEFT|SS_NOTIFY|WS_VISIBLE,
		   this, IDC_PLAY_RECT_SLIDER, IDC_PLAY_SLIDER));

	m_PlaySlider.SetScrollRange(0, PLAYER_SLIDER_MAX);
	m_PlaySlider.EnableThumbColor(FALSE);
	m_PlaySlider.EnableThumbGripper(TRUE);
	m_PlaySlider.EnableChannelColor(FALSE);

	NAME(PlayM4_GetPort)(&m_lPort);
	
	// Init Sound Slider
#ifdef _WAVE_ADJ
	m_SoundSlider.SetRangeMin(MIN_WAVE_COEF);
	m_SoundSlider.SetRangeMax(MAX_WAVE_COEF);
#else
	m_SoundSlider.SetRangeMin(0);
	m_SoundSlider.SetRangeMax(0xffff-1);
#endif

	m_SoundSlider.SetLineSize(1);
	m_SoundSlider.SetPageSize(5);

// 	

	// Init the parameters
	m_StartPoint.x		 =	0;
	m_StartPoint.y		 =	0;
	m_dwTotalFrames		 =	0;
	m_dwMaxFileTime		 =	0;
	m_dwDisplayHour		 =	0;
	m_dwDisplayMinute	 =	0;
	m_dwDisplaySecond	 =	0;
	m_dwImageSharpenLevel=  0;
	m_nPrePlayPos		 =  0;
	m_dwOldDeviceNum     =  0;

	m_npic_bmp			 =	0;
	m_npic_jpeg			 =	0;
	m_nCapPicType		 =	0;
	m_strCapPicPath		 =  _T("");

	m_bStartDraw         =	FALSE;
	m_bSound			 =	FALSE;
	m_bOpen			     =	FALSE;
	m_bFullScreen		 =	FALSE;
	m_bRepeatPlay		 =	FALSE;
	m_bStreamType		 =	FALSE;
	m_bConvertAVI		 =  FALSE;
	m_bPicQuality	     =  TRUE;
	m_bDeflash			 =  TRUE;
#ifdef _HIGH_FRUID
	m_bHighFluid		 =  TRUE;
#else
	m_bHighFluid		 =  FALSE;
#endif

	m_bFileRefCreated	 =  FALSE;

	m_strPlayStateText	 =	_T("Ready");
	m_ctrlPlayText.ShowText(m_strPlayStateText);
	m_strSaveAVIPath	 =  _T("");
	
	m_pQcifTempBuf		 =  NULL;

	// Init state text control
	m_ctrlPlayText.SetTextColor(RGB(255,255,255), FALSE);
	m_ctrlPlayText.SetBackgroundColor(RGB(0,0,0), FALSE);
	m_ctrlPlayText.SetBold(TRUE, FALSE);

	m_nWidth	= 352;
	m_nHeight   = 288;

	// init the sub dialogs
	m_pSeek				 =	new CSeek(this);
	m_pDisplayRegion	 =	new CDisplayRect(this);
	m_pVideoControl		 =  new CVideoCtrlDlg(this);

	// bitmap
	m_BlackBmp.LoadBitmap(IDB_BLACK);
	m_OverlayBmp.LoadBitmap(IDB_OVERLAY);
#ifdef _FOR_HIKPLAYM4_DLL_
	m_HikvisionBmp.LoadBitmap(IDB_HIKVISION);
	SetWindowText("Hikvision Player");
#else
	m_HikvisionBmp.LoadBitmap(IDB_BLACK);
	SetWindowText("Player");
#endif

	// init sub dlg
	m_pWatermarkDlg	     =  new CWatermarkDlg(this);
	
	// init state
	m_enumState = State_Close;
	SetState();

	// set the capture picture call back function;
//	NAME(PlayM4_SetDisplayCallBack)(m_lPort, DisplayCBFun);
	// set the wave audio call back funtion;
//	NAME(PlayM4_SetAudioCallBack)(m_lPort, WaveCBFun, (long)this);



	NAME(PlayM4_SetDDrawDevice)(m_lPort, 0);
	// Test adapter Capability;

	
	// used for command line
	if(m_strPlayFileName.Compare(""))
	{
		Open();
		SetState();
	}

	UpdateData(FALSE);
	SortControl();
	//NAME(PlayM4_SetVolume)(m_lPort,WORD(m_SoundSlider.GetPos()));
	m_bInited = TRUE;
	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CPlayerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPlayerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

	//	SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
	this->UpdateWindow();
	NAME2(PlayM4_RefreshPlay)(m_lPort);
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPlayerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//////////////////////////////////////////////////////////////////////////////
//Function: called when u drag a file to the playing window
//////////////////////////////////////////////////////////////////////////////
void CPlayerDlg::OnDropFiles(HDROP hDropInfo) 
{
	// TODO: Add your message handler code here and/or call default
	
	DWORD nFileNameSize = DragQueryFile(hDropInfo, 0, NULL, 0);
	char * sFileName	= new char[nFileNameSize + 1];
	DragQueryFile(hDropInfo, 0, sFileName, nFileNameSize + 1);
	m_strPlayFileName   = sFileName;
	
	Open();
	SetState();

	delete []sFileName;
	CDialog::OnDropFiles(hDropInfo);
}


//////////////////////////////////////////////////////////////////////////////
//Function: overload to handle keydown message
//////////////////////////////////////////////////////////////////////////////
BOOL CPlayerDlg::PreTranslateMessage(MSG* lpmsg)
{
	
	switch (lpmsg->message)
	{
	case WM_KEYDOWN:
		OnKeyDown(lpmsg->wParam, LOWORD(lpmsg ->lParam), HIWORD(lpmsg->lParam));
		return TRUE;
		
	default:
		break;
	} 

	return CDialog::PreTranslateMessage(lpmsg); 
}


//////////////////////////////////////////////////////////////////////////////
//Function: hot key
//////////////////////////////////////////////////////////////////////////////
void CPlayerDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	UINT  nMenuState;

	if(nChar == VK_F2) 
		ViewFullScreen();


	if(GetKeyState(VK_CONTROL) & 0xFF00)
	{
		switch(nChar)
		{
		case VK_C:
			if(!m_bFullScreen)
			{
				nMenuState = m_pMainMenu->GetMenuState(IDM_FILE_CLOSE, MF_BYCOMMAND);
				if(nMenuState != MF_GRAYED)
				{
					Close();
					SetState();
				}
			}
			break;

		case VK_O:
			if(!m_bFullScreen)
			{
				nMenuState = m_pMainMenu->GetMenuState(IDM_FILE_OPEN, MF_BYCOMMAND);
				if(nMenuState != MF_GRAYED)		
				{
					if(BrowseFile(&m_strPlayFileName))
					{
						Open();
						SetState();
					}
				}
			}
			break;
		
		case VK_K:
			if (MF_GRAYED != m_pMainMenu->GetMenuState(IDM_SET_KEY, MF_BYCOMMAND))
			{
				SetSecretKey();
			}
			break;

/*		case VK_T:
			nMenuState = m_pMainMenu->GetMenuState(IDM_CUT_FILE, MF_BYCOMMAND);
			if(nMenuState != MF_GRAYED)
			{
				CutFile();
			}
			break;
*/
		case VK_L:
			Locate();
			break;

		case VK_LEFT:
			GotoStart();
			break;

		case VK_RIGHT:
			GotoEnd();
			break;
		
		case VK_ONE:
			if(m_enumState != State_Stop)
			{
				ViewZoom(IDM_VIEW_ZOOM_50);
			}
			break;

		case VK_TWO:
			if(m_enumState != State_Stop)
			{
				ViewZoom(IDM_VIEW_ZOOM_100);
			}
			break;

		case VK_THREE:
			if(m_enumState != State_Stop)
			{
				ViewZoom(IDM_VIEW_ZOOM_200);
			}
			break;

		default:
			break;
		}
	}

	else
	{
		switch(nChar)
		{
		case VK_ESCAPE:
			if(m_bFullScreen)
			{
				ViewFullScreen();
			}
			break;

		case VK_SPACE:
			if(m_enumState == State_Play)
			{
				Pause();
				SetState();
			}
			else if(m_enumState == State_Pause || m_enumState == State_Stop)
			{
				Play();
				SetState();
			}
			break;
		
		case VK_LEFT:
			if(m_enumState != State_Stop)
			{
				StepBackward();
				SetState();
			}
			break;

		case VK_RIGHT:
			if(m_enumState != State_Stop)
			{
				StepForward();
				SetState();
			}
			break;

		case VK_UP:
			AdjustSound(TRUE);
			break;

		case VK_DOWN:
			AdjustSound(FALSE);
			break;
		
		case VK_DOT:
			if(m_enumState != State_Close)
			{
				if(m_bFullScreen)
				{
					ViewFullScreen();
				}
				
				Stop();
				SetState();
			}
			break;

		default:
			break;
		}
	}

	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

///////////////////////////////////////////////////////////////////////////
//Function: Voluem and progress.
///////////////////////////////////////////////////////////////////////////
void CPlayerDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	DWORD nTime;
	int   nPlayPos;
	switch(GetWindowLong(pScrollBar->m_hWnd, GWL_ID))
	{
	/////////////////////////////////////////////////////////////////
	//progress slider
	case IDC_PLAY_SLIDER:
		if(m_bStreamType)
		{
			break;
		}
		nPlayPos = m_PlaySlider.GetScrollPos();
		if( abs(nPlayPos - m_nPrePlayPos) < 3 )
		{
			break;
		}

#ifdef _FILE_POS
		NAME(PlayM4_SetPlayPos)(m_lPort, (float)nPlayPos/(float)PLAYER_SLIDER_MAX);
#else
 		nTime = nPlayPos * 1000 / PLAYER_SLIDER_MAX * m_dwMaxFileTime;
 		NAME2(PlayM4_SetPlayedTimeEx)(m_lPort,nTime);
#endif
//		DrawStatus();
		break;

	///////////////////////////////////////////////////////////////
	//sound slider;
	case IDC_SOUND_SLIDER:
#ifdef _WAVE_ADJ
		NAME(PlayM4_AdjustWaveAudio)(m_lPort, m_SoundSlider.GetPos());
#else
		//NAME(PlayM4_SetVolume)(m_lPort, WORD(m_SoundSlider.GetPos()));
#endif
		break;

	default:
		break;
	}
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

/////////////////////////////////////////////////////////////////////
//Called when u double click the left button
/////////////////////////////////////////////////////////////////////
void CPlayerDlg::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CPoint dpoint;
	CRect  vwrect;
	GetCursorPos(&dpoint);	

	m_ctrlVideoPic.GetWindowRect(&vwrect);
	if( ( m_enumState == State_Play || m_enumState == State_Pause) && vwrect.PtInRect(dpoint))
	{
		ViewFullScreen();
	}
		
	CDialog::OnLButtonDblClk(nFlags, point);
}


/////////////////////////////////////////////////////////////////////
//Timer funtion
/////////////////////////////////////////////////////////////////////
void CPlayerDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent==PLAY_TIMER)
	{
		DrawStatus();
	}
	CDialog::OnTimer(nIDEvent);
}




/////////////////////////////////////////////////////////////////////
//Adjust control allign function
/////////////////////////////////////////////////////////////////////
void CPlayerDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here

	if(m_ctrlVideoPic.GetSafeHwnd())
	{
		SortControl();
	}
}


/////////////////////////////////////////////////////////////////////
// Function: Limit the window size.
/////////////////////////////////////////////////////////////////////
void CPlayerDlg::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default
	lpMMI->ptMinTrackSize.x = WIDTH/2 + m_dwDlgEdge * 2;				// width
	lpMMI->ptMinTrackSize.y = PANNEL_HEIGHT + m_dwDlgTopSize ;			// height

	CDialog::OnGetMinMaxInfo(lpMMI);
}


///////////////////////////////////////////////////////////////////////////
//Function:Redraw the image when move window.
///////////////////////////////////////////////////////////////////////////
void CPlayerDlg::OnMove(int x, int y) 
{
	CDialog::OnMove(x, y);
	
	if (!m_bInited)
	{
		return;
	}
	// TODO: Add your message handler code here
	NAME2(PlayM4_RefreshPlay)(m_lPort);

	GetWindowRect(&m_rcWindow);
	DWORD dwNewDeviceNum;

	if(m_rcWindow.left < 0)
	{
		return;
	}
	
	if( (DWORD)m_rcWindow.right + (DWORD)m_rcWindow.left > 2 * m_dwScreenWidth )
	{
		if( (DWORD)m_rcWindow.top + (DWORD)m_rcWindow.bottom > 2 * m_dwScreenHeight)
		{
			dwNewDeviceNum = 3;
			m_rcScreen.left  = m_dwScreenWidth;
			m_rcScreen.right = m_dwScreenWidth  * 2;
			m_rcScreen.top   = m_dwScreenHeight;
			m_rcScreen.bottom= m_dwScreenHeight * 2;
		}
		else
		{
			dwNewDeviceNum = 1;
			m_rcScreen.left  = m_dwScreenWidth;
			m_rcScreen.right = m_dwScreenWidth  * 2;
			m_rcScreen.top   = 0;
			m_rcScreen.bottom= m_dwScreenHeight;
		}
	}
	else
	{
		if( (DWORD)m_rcWindow.top + (DWORD)m_rcWindow.bottom > 2 * m_dwScreenHeight)
		{
			dwNewDeviceNum = 2;
			m_rcScreen.left  = 0;
			m_rcScreen.right = m_dwScreenWidth;
			m_rcScreen.top   = m_dwScreenHeight;
			m_rcScreen.bottom= m_dwScreenHeight * 2;
		}
		else
		{
			dwNewDeviceNum = 0;
			m_rcScreen.left  = 0;
			m_rcScreen.right = m_dwScreenWidth;
			m_rcScreen.top   = 0;
			m_rcScreen.bottom= m_dwScreenHeight;
		}
	}
	
	if(dwNewDeviceNum != m_dwOldDeviceNum)
	{
		if(SetDevice(IDM_DEVICE0 + dwNewDeviceNum))
		{
			m_dwOldDeviceNum = dwNewDeviceNum;
		}
	}
}



///////////////////////////////////////////////////////////////////////////
//Function: Called when u click the left mouse button
///////////////////////////////////////////////////////////////////////////
void CPlayerDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
//	CRect rcShow;
//	m_ctrlVideoPic.GetWindowRect(rcShow);
//	ScreenToClient(rcShow);
//
//	if(rcShow.PtInRect(point))
//	{
//		m_StartPoint = point;
//		m_bStartDraw = TRUE;
//		InvalidateRect(m_rcDraw, TRUE);
//		ZeroMemory(&m_rcDraw, sizeof(m_rcDraw));
//	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CPlayerDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
//	if(m_bStartDraw)
//	{
//		if(m_StartPoint != point)
//		{
//
//			CRect rcShow;
//			m_ctrlVideoPic.GetWindowRect(rcShow);
//			ScreenToClient(rcShow);
//			
//			if( rcShow.PtInRect(point) && (point.x >= m_StartPoint.x) && (point.y >= m_StartPoint.y))
//			{
//				m_rcDraw.left   = m_StartPoint.x;
//				m_rcDraw.top    = m_StartPoint.y;
//				m_rcDraw.right  = point.x;
//				m_rcDraw.bottom = point.y;
//				
//				CDC* pDC = GetDC();
//				pDC->DrawFocusRect(m_rcDraw);
//				ReleaseDC(pDC);
//			
//				LONG nPicWidth  = 352;
//				LONG nPicHeight = 288;
//				NAME(PlayM4_GetPictureSize)(m_lPort, &nPicWidth, &nPicHeight);
//				m_rcDisplay = GetOnPicRect(rcShow, m_rcDraw, nPicWidth, nPicHeight);
//
//				if(m_pDisplayRegion->m_hWnd)
//					m_pDisplayRegion->PostMessage(WM_DISPLAY_RECT, 0, (long)(&m_rcDisplay));
//			}
//
//		}
//		m_bStartDraw = FALSE;
//	}
	
	CDialog::OnLButtonUp(nFlags, point);
}


//////////////////////////////////////////////////////////////////////
//Function: Close.
//////////////////////////////////////////////////////////////////////
void CPlayerDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	NAME(PlayM4_Stop)(m_lPort);
	NAME(PlayM4_CloseFile)(m_lPort);
	NAME(PlayM4_RealeseDDraw)();

#if (WINVER > 0x0400)
	NAME(PlayM4_ReleaseDDrawDevice)();
#endif
	
	if(m_pSeek != NULL)
	{
		m_pSeek->DestroyWindow();
		delete m_pSeek;
		m_pSeek = NULL;
	}

	if(m_pDisplayRegion != NULL)
	{
		m_pDisplayRegion->DestroyWindow();
		delete m_pDisplayRegion;
		m_pDisplayRegion = NULL;
	}

	if(m_pVideoControl != NULL)
	{
		m_pVideoControl->DestroyWindow();
		delete m_pVideoControl;
		m_pVideoControl = NULL;
	}

	if(m_bConvertAVI)
	{
		g_classAVI.ReleaseResource();
		m_bConvertAVI = FALSE;
	}

	if(m_pWatermarkDlg != NULL)
	{
		m_pWatermarkDlg->DestroyWindow();
		delete m_pWatermarkDlg;
		m_pWatermarkDlg = NULL;
	}
	
	NAME(PlayM4_FreePort)(m_lPort);
	CDialog::OnClose();
}


/*************************************************************************/
/*************************************************************************/
/*************************************************************************/
/*  user defined message handle begin
/*************************************************************************/
/*************************************************************************/
/*************************************************************************/

// Function: process the received WM_FILE_END message.
// Para: wParam channel No. lParam reserved£»
void CPlayerDlg::PlayMessage(WPARAM /*wParam*/, LPARAM lParam)
{
//	if(m_bFullScreen)
//		ViewFullScreen();
	
	//remove the same message.
	MSG msgVal;
	while(PeekMessage(&msgVal, m_hWnd, WM_FILE_END, WM_FILE_END, PM_REMOVE));

	if(m_bRepeatPlay)
	{
		TRACE("Process message %d\n", lParam);
		GotoStart();
	}
	
	if(m_bConvertAVI)
	{  
		Stop();
		SetState();
	}
}

// when enc changed got this message
void CPlayerDlg::EncChangeMessage(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	OutputDebugString("Get Message/n!");
	MSG msgVal;
	while(PeekMessage(&msgVal,m_hWnd,WM_ENC_CHANGE,WM_ENC_CHANGE,PM_REMOVE));

	if(m_bFullScreen)
	{
		ViewFullScreen();
	}

	SetWindowSize();
	
	if(m_pDisplayRegion)
	{
		if(m_pDisplayRegion->m_bValid)
		{
			m_pDisplayRegion->SetResolution(m_nHeight, m_nWidth);
			m_pDisplayRegion->InitShow();
			
			if(m_enumState == State_Pause)
			{
				NAME(PlayM4_OneByOne)(m_lPort);
			}
		}
	}

	Sleep(1);
}

void CPlayerDlg::VideoCtrlOK(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	m_pVideoControl->DestroyWindow();
	if(m_enumState == State_Pause || m_enumState == State_Play)
	{
		m_pMainMenu->EnableMenuItem(IDM_VIDEO_CONTROL, MF_ENABLED);
	}
}

// when u click the OK button on the seeking dialog, got this message
void CPlayerDlg::SeekOk(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	m_pSeek->DestroyWindow();
	if(m_enumState == State_Pause || m_enumState == State_Play)
	{
		m_pMainMenu->EnableMenuItem(IDM_SEEK,MF_ENABLED);
	}
}

// when u click the OK button on the display dialog, got this message
void CPlayerDlg::DisplayOk(WPARAM wParam, LPARAM /*lParam*/)
{
	if (wParam == 0) 
	{
		m_ctrlBtnCapPic.EnableWindow(TRUE);
		m_ctrlBtnSound.EnableWindow(TRUE);	
		m_ctrlBtnSlow.EnableWindow(TRUE);
		m_ctrlBtnFast.EnableWindow(TRUE);
		m_ctrlBtnGStart.EnableWindow(TRUE);
		m_ctrlBtnGEnd.EnableWindow(TRUE);
		m_ctrlBtnStop.EnableWindow(TRUE);
		m_ctrlBtnPlay.EnableWindow(TRUE);
		m_ctrlBtnPause.EnableWindow(TRUE);	
		m_ctrlStepBackward.EnableWindow(TRUE);
		m_ctrlStepForward.EnableWindow(TRUE);
 		m_pMainMenu->EnableMenuItem(2, MF_ENABLED|MF_BYPOSITION);
		Play();
		SetState();
	}
	if (wParam == 1) 
	{
		Pause();
		SetState();
	}
}
/*************************************************************************/
/*************************************************************************/
/*************************************************************************/
/*  user defined message handle over
/*************************************************************************/
/*************************************************************************/
/*************************************************************************/


// Functon:Watermark call back function.
void CALLBACK funCheckWatermark(long /*nPort*/, WATERMARK_INFO *pFrameType, DWORD /*nUser*/)
{
//	CPlayerDlg* pOwner = (CPlayerDlg*)nUser;
	char* pBuf = (char*)pFrameType->pDataBuf;
	m_strWaterMark.global_time = *(DWORD*)pBuf;
	pBuf += sizeof(DWORD);
	m_strWaterMark.device_sn = *(DWORD*)pBuf;
	pBuf += sizeof(DWORD);
	for(int i = 0; i < 6; i ++)
	{
		m_strWaterMark.mac_addr[i] = *(unsigned char*)pBuf;
		pBuf += sizeof(unsigned char);
	}

	m_strWaterMark.device_type = *(unsigned char*)pBuf;
	pBuf += sizeof(unsigned char);
	m_strWaterMark.device_info = *(unsigned char*)pBuf;
	pBuf += sizeof(unsigned char);
	m_strWaterMark.channel_num = *(unsigned char*)pBuf;
	pBuf += sizeof(unsigned char);
}

/*************************************************************************/
/*************************************************************************/
/*************************************************************************/
/*  callback function begin
/*************************************************************************/
/*************************************************************************/
/*************************************************************************/	

// Function:The call back funtion for capture image!
void CALLBACK DisplayCBFun(long /*nPort*/,\
						   char * pBuf,long /*nSize*/,\
						   long /*nWidth*/,long /*nHeight*/,\
						   long /*nStamp*/,long /*nType*/,long /*nReceaved*/)
{
	UNREFERENCED_PARAMETER(pBuf);
}

// Function: The dec call back funtion.
void CALLBACK DecCBFun(long nPort,char * pBuf,long nSize,
					   FRAME_INFO * pFrameInfo, 
					   long nReserved1,long /*nReserved2*/)
{
	OutputDebugString("½âÂë»Øµ÷");
	DWORD dwTime = PlayM4_GetSpecialData(nPort);
//	TRACE("nPort=%d, TYPE=%d; Width=%d; Height=%d\n", nPort, pFrameInfo->nType, pFrameInfo->nWidth, pFrameInfo->nHeight);
	TRACE("wptest==============Time: Year is %d, Month is %d, Day is %d, Hour is %d, %d, %d", GET_YEAR(dwTime),
			GET_MONTH(dwTime), GET_DAY(dwTime), GET_HOUR(dwTime), GET_MINUTE(dwTime), GET_SECOND(dwTime));
/*	
	CPlayerDlg* pDlg = (CPlayerDlg *)nReserved1;

	if ( pFrameInfo->nType == T_YV12 ) 
	{   
		if(g_classAVI.IsWriteAVIHdr())
		{
			g_classAVI.SetFPS(pFrameInfo->nFrameRate);
			g_classAVI.WriteHeaders();
		}

		// ntsc qcif
		if(pFrameInfo->nHeight == 128)
		{
			if(pDlg->m_pQcifTempBuf == NULL)
			{
				pDlg->m_pQcifTempBuf = new BYTE[nSize];
			}
		
			int nPos = 0;		
			// Y ·ÖÁ¿
			for(int i = 0; i < 4; i++)
			{
				CopyMemory(pDlg->m_pQcifTempBuf + i * pFrameInfo->nWidth, pBuf, pFrameInfo->nWidth);
			}

			CopyMemory(pDlg->m_pQcifTempBuf + 4 * pFrameInfo->nWidth, pBuf, pFrameInfo->nWidth * 120);
			for(i = 0; i < 4; i++)
			{
				CopyMemory(pDlg->m_pQcifTempBuf + (124 + i) * pFrameInfo->nWidth, pBuf + pFrameInfo->nWidth * 119, pFrameInfo->nWidth);
			}

			nPos += nSize*2/3;
			
			int w = pFrameInfo->nWidth/2; 
			// U/V·ÖÁ¿
			for(int j = 0; j < 2; j++)
			{
				for(i = 0; i < 2; i++)
				{
					CopyMemory(pDlg->m_pQcifTempBuf + i * w + nPos,  pBuf + nPos, w);
				}
				CopyMemory(pDlg->m_pQcifTempBuf + w * 2 + nPos, pBuf + nPos, w * 60);
				for(i = 0; i < 2; i++)
				{
					CopyMemory(pDlg->m_pQcifTempBuf + w * (62 + i) + nPos, pBuf + w * 59 + nPos, w);
				}
				nPos += nSize*1/6;
			}

			g_classAVI.AddFileToAVI((char*)pDlg->m_pQcifTempBuf, nSize);
		}
		else
		{		
			g_classAVI.AddFileToAVI(pBuf, nSize);
		}

		if(g_classAVI.IsExceedMaxFileLen())
		{
			SendMessage(AfxGetApp()->GetMainWnd()->m_hWnd,WM_FILE_END,m_lPort,0);		   
		}	
	}
*/
	Sleep(1);
}

// Funtion:The source buffer call back funtion.
void CALLBACK SourceBufFun(long nPort,DWORD nBufSize,DWORD dwUser,void*pContext)
{
	UNREFERENCED_PARAMETER(pContext);

	CPlayerDlg* pOwner = (CPlayerDlg*)dwUser;
	// do after play!
	RunInfo("wptest======================Src Remain %d", nBufSize);
	if( (pOwner->m_enumState == State_Close) || (pOwner->m_enumState == State_Stop) )
	{
		return ;
	}
	NAME(PlayM4_ResetSourceBufFlag)(nPort);
	if(pOwner->m_bFileEnd)
	{
		
		PostMessage(pOwner->m_hWnd, WM_FILE_END, m_lPort, 0);
		pOwner->m_bFileEnd = FALSE;
	}
	else
	{
		SetEvent(pOwner->m_hEventInput);
	}
}

// Functon:File reference call back function.
void CALLBACK FileRefDone(DWORD /*nReserved*/,DWORD nUser)
{
	CPlayerDlg* pOwner = (CPlayerDlg*)nUser;
	pOwner->m_bFileRefCreated = TRUE;
//	pOwner->m_pMainMenu->EnableMenuItem(IDM_CUT_FILE,MF_ENABLED);
	pOwner->m_pMainMenu->EnableMenuItem(IDM_SEEK,MF_ENABLED);
	TRACE("File reference created!\n");
	FRAME_POS stFramePos = {0};
	PlayM4_GetKeyFramePos(m_lPort, 100, BY_FRAMENUM, &stFramePos);
}

// Functon:File Verify call back function.
/*
void CALLBACK VerifyFun(long nPort, FRAME_POS * pFilePos, DWORD bIsVideo,  DWORD nUser)
{
//	TRACE("File have been changed at: pos = 0x%X; time(s) = %d, frameNum = %d; IsVideo = %d\n",
//		pFilePos->nFilePos, pFilePos->nFrameTime/1000, pFilePos->nFrameNum, bIsVideo);
	CString abstime;
	CString str;
	if (pFilePos->pErrorTime )
	{
		abstime.Format("%02d-%02d-%02d %02d:%02d:%02d",pFilePos->pErrorTime->wYear, pFilePos->pErrorTime->wMonth,
			pFilePos->pErrorTime->wDay, pFilePos->pErrorTime->wHour, pFilePos->pErrorTime->wMinute, pFilePos->pErrorTime->wSecond);
	}
	str.Format("file error at pos=0x%X, time(s) =%d,frameNum=%d,isVideo=%d, \
		errorframe at%d,lostframe=%d,lostdata=%d,time=%s\n",	\
		pFilePos->nFilePos, pFilePos->nFrameTime/1000, pFilePos->nFrameNum, bIsVideo,	\
		pFilePos->nErrorFrameNum, pFilePos->nErrorLostFrameNum, pFilePos->nErrorFrameSize, abstime);
	TRACE(str); 
}
*/

// Functon:Wave data call back function.
void CALLBACK WaveCBFun(long /*nPort*/, char * pAudioBuf, long /*nSize*/, long /*nStamp*/, long /*nType*/, long /*nUser*/)
{
	UNREFERENCED_PARAMETER(pAudioBuf);
	//TRACE("Wave data, nPort = %d, nSize = %d, nStamp = %d, nType = %d\n", nPort, nSize, nStamp, nType);
}
/*************************************************************************/
/*************************************************************************/
/*************************************************************************/
/*  callback function begin
/*************************************************************************/
/*************************************************************************/
/*************************************************************************/

void CALLBACK EncChange(LONG lPort, LONG dwUser)
{
	CPlayerDlg* pOwner = (CPlayerDlg*)dwUser;
	pOwner->SetWindowSize();
}

///////////////////////////////////////////////////////////////////////
// Function: input stream thread.
////////////////////////////////////////////////////////////////////////
#define BUF_SIZE 3008
DWORD WINAPI InputStreamThread( LPVOID lpParameter)
{
	CPlayerDlg* pOwner = (CPlayerDlg*)lpParameter;
	HANDLE hMulEvents[2];
	hMulEvents[0] = pOwner->m_hEventKill;
	hMulEvents[1] = pOwner->m_hEventInput;
	BYTE pBuf[BUF_SIZE];
	DWORD nRealRead;
	BOOL bBufFull = FALSE;
    DWORD dwSize = BUF_SIZE;
	unsigned char chType;
	DWORD	dwDataLen = 0;
    if(pOwner->m_dwSysFormat == SYSTEM_RTP)
    {
		SetFilePointer(pOwner->m_hStreamFile, 0, 0, FILE_BEGIN);
    }

	while(WAIT_OBJECT_0 != WaitForMultipleObjects(2, hMulEvents, FALSE, INFINITE))
	{	
		if(!bBufFull)
		{
			// TRACE("Read file and put it input into the stream buffer.\n");
            if(pOwner->m_dwSysFormat != SYSTEM_RTP)
            {
				if(!(ReadFile(pOwner->m_hStreamFile, pBuf, dwSize, &nRealRead, NULL) && (nRealRead == dwSize)))
				{
					//File end;
					pOwner->m_bFileEnd = TRUE;
					bBufFull = FALSE;
					ResetEvent(pOwner->m_hEventInput);
				}
				dwDataLen = dwSize;
            }
            else
            {
                //ÏÈ¶Á³ö12×Ö½ÚrtpÍ·
                if (!(ReadFile(pOwner->m_hStreamFile, pBuf, 12, &nRealRead, NULL) && (nRealRead == 12)))
                {
                    //File end;
					pOwner->m_bFileEnd = TRUE;
					bBufFull = FALSE;
					ResetEvent(pOwner->m_hEventInput);
                }
				dwSize = pBuf[11] + (pBuf[10] << 8) + (pBuf[9] << 16) + (pBuf[8] << 24);
				chType = BYTE(pBuf[1] & 0x7f);
		
                if (!(ReadFile(pOwner->m_hStreamFile, pBuf + 12, dwSize, &nRealRead, NULL) && (nRealRead == dwSize)))
                {
                    //File end;
					pOwner->m_bFileEnd = TRUE;
					bBufFull = FALSE;
					ResetEvent(pOwner->m_hEventInput);
                }
                dwDataLen = dwSize + 12;
            }
			
			while ( !NAME(PlayM4_InputData)(m_lPort, pBuf, dwDataLen) )
			{
				if ( PlayM4_GetLastError(m_lPort) == PLAYM4_BUF_OVER )
				{
					Sleep(10);
					continue;
				}
					
				bBufFull = TRUE;
				ResetEvent(pOwner->m_hEventInput);
				break;
			}
			
//			if(!NAME(PlayM4_InputData)(m_lPort, pBuf, dwDataLen)) //Èç¹ûÊý¾Ý³¤¶ÈÊäÈëdwSize,²¥·ÅÆ÷ÂíÉÏ±ÀÀ£
//			{
//				bBufFull = TRUE;
//				ResetEvent(pOwner->m_hEventInput);
//			}
//			else
//			{
//				bBufFull = FALSE;
//			}
			
		}
	}
	return 1;
}



/*************************************************************************/
/*************************************************************************/
/*************************************************************************/
/*  assistant operation begin
/*************************************************************************/
/*************************************************************************/
/*************************************************************************/
// open/close file or stream
// browse file and get the file path
BOOL CPlayerDlg::BrowseFile(CString *strFileName)
{
#ifdef _FOR_HIKPLAYM4_DLL_
		CFileDialog dlg(TRUE, 
						"mpg",
						NULL, 
						OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
						"Hikvision File(*.mp4;*.264)|*.mp4;*.264|All Files(*.*)|*.*||", this);
#else
		CFileDialog dlg(TRUE, 
						"mpg",
						NULL, 
						OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
						"File(*.mp4;*.264)|*.mp4;*.264|All Files(*.*)|*.*||", this);
#endif

	if(dlg.DoModal() == IDCANCEL)
	{
		return FALSE;
	}
	*strFileName = dlg.GetPathName();
	return TRUE;
}

// open file and play
void CPlayerDlg::OpenFile()
{
	CString strError;
	
	NAME(PlayM4_SetFileEndMsg)(m_lPort, m_hWnd, WM_FILE_END);

	NAME(PlayM4_SetFileRefCallBack)(m_lPort, FileRefDone, (DWORD)this);
//	NAME(PlayM4_SetVerifyCallBack)(m_lPort, 0, 0xffffffff, VerifyFun, (DWORD) this); // verify the whole file;
//	NAME(PlayM4_SetDecCallBackMend)(m_lPort, DecCBFun, (long)this);	

	if(!NAME(PlayM4_OpenFile)(m_lPort, m_strPlayFileName.GetBuffer(m_strPlayFileName.GetLength())))
	{
		m_strPlayFileName = _T("");
		strError.Format("Open file failed(%s)", MyErrorToString(NAME(PlayM4_GetLastError)(m_lPort)));
		MessageBox(strError);
		throw 0;
	}
	
	// try overlay mode!
//	NAME(PlayM4_SetOverlayMode)(m_lPort, TRUE, RGB(255,0,255));
	// try flip overlay mode
//	NAME(PlayM4_SetOverlayFlipMode)(m_lPort, TRUE);

	m_dwMaxFileTime = NAME(PlayM4_GetFileTime)(m_lPort);
	if(!m_dwMaxFileTime)
	{
		strError.Format("File seconds is zero");
		MessageBox(strError);
		throw 0;
	}

	m_dwDisplayHour		= m_dwMaxFileTime/3600;
	m_dwDisplayMinute   = (m_dwMaxFileTime % 3600) / 60;
	m_dwDisplaySecond   = m_dwMaxFileTime % 60;
	m_dwTotalFrames		= NAME(PlayM4_GetFileTotalFrames)(m_lPort);

	if(m_bConvertAVI)
	{
		AVI_CONVERT_PARA strParam;
		long dwHeight, dwWidth;
		NAME(PlayM4_GetPictureSize)(m_lPort, &dwWidth, &dwHeight);
		strParam.dwFrameWidth  = MOD16(dwWidth);
		strParam.dwFrameHeight = MOD16(dwHeight);
		strParam.dwFrameSize   = strParam.dwFrameWidth * strParam.dwFrameHeight *3/2;
		strParam.dwTotalFrames = m_dwTotalFrames;

		BOOL bInitAVI = g_classAVI.InitResource(&strParam, m_strSaveAVIPath);
		if(!bInitAVI)
		{
			strError.Format("Init AVI Resource failed!");
			MessageBox(strError);
			g_classAVI.ReleaseResource();
			m_bConvertAVI = FALSE;
		}
	}

	// If you want to change the display buffer. Do hear!
//	NAME(PlayM4_SetDisplayBuf)(m_lPort,50);
//	NAME(PlayM4_SetDisplayBufAddtionalLen)(m_lPort, 6);

	Play();
//	PlayM4_SetPlayedTimeEx(m_lPort, 3000000);
// 	BOOL b = NAME(PlayM4_SetDisplayType)(m_lPort, 3);
//	PlayM4_SetCurrentFrameNum(m_lPort, 10000);
//	PlayM4_SetPlayPos(m_lPort, 1.2);
// 	DWORD dw = PlayM4_GetLastError(m_lPort);
	SetWindowText((LPCTSTR) m_strPlayFileName);
}

// close file
void CPlayerDlg::CloseFile()
{
	if (m_pDisplayRegion->m_bValid) 
	{
		m_pDisplayRegion->DestroyWindow();
		m_pMainMenu->EnableMenuItem(2, MF_ENABLED|MF_BYPOSITION);
		m_pMainMenu->EnableMenuItem(IDM_SETDISPLAY, MF_ENABLED);
		
	}

	Stop();
	NAME(PlayM4_CloseFile)(m_lPort);
//	NAME(PlayM4_FreePort)(m_lPort);
	m_pMainMenu->GetSubMenu(3)->EnableMenuItem(10, MF_ENABLED|MF_BYPOSITION);
	m_enumState = State_Close;
	m_bOpen = FALSE;
	m_bFileRefCreated =	FALSE;	
}

// Funtion: Open the file by stream type and play it
void CPlayerDlg::OpenStream()
{
	m_dwDisplayHour		= 0;
	m_dwDisplayMinute   = 0;
	m_dwDisplaySecond   = 0;
	m_dwTotalFrames     = 0;
	m_bFileEnd          = FALSE;
	HIK_MEDIAINFO		stInfo;
	ZeroMemory(&stInfo,sizeof(HIK_MEDIAINFO));
	
	m_hStreamFile = CreateFile(m_strPlayFileName,
							   GENERIC_READ,
							   FILE_SHARE_READ,
							   NULL,
							   OPEN_EXISTING,
							   FILE_ATTRIBUTE_NORMAL,
							   NULL);
	if(m_hStreamFile == INVALID_HANDLE_VALUE)
	{
		MessageBox("Open file failed");
		throw 0;
	}
	m_dwMaxFileSize = ::GetFileSize(m_hStreamFile, NULL);
//	NAME(PlayM4_SetSourceBufCallBack)(m_lPort, 6000000, SourceBufFun, (DWORD)this, NULL);
	NAME(PlayM4_SetStreamOpenMode)(m_lPort, STREAME_FILE);
	m_dwHeadSize = NAME(PlayM4_GetFileHeadLength)();
	PBYTE pBuf = NULL;
	char csError[50];
	
	pBuf = new BYTE[m_dwHeadSize];
	if(!pBuf)
	{
		MessageBox("Alloc memory failed");
		throw 0;
	}
	DWORD nRealRead;
	SetFilePointer(m_hStreamFile, 0, 0, FILE_BEGIN);
	ReadFile(m_hStreamFile, pBuf, m_dwHeadSize, &nRealRead, NULL);
	if(nRealRead != m_dwHeadSize)
	{
		MessageBox("File is too small");
		delete []pBuf;
		pBuf = NULL;
		throw 0;
	}

#ifdef SPLIT_INPUT
	if(!NAME(PlayM4_OpenStreamEx)(m_lPort, (BYTE*)&stInfo, m_dwHeadSize, 6*1000*1024))
	{
		sprintf(csError, "Open stream failed(%s)", MyErrorToString(NAME(PlayM4_GetLastError)(m_lPort)));
		MessageBox(csError);
		delete []pBuf;
		pBuf = NULL;
		throw 0;
	}
	PlayM4_SyncToAudio(m_lPort, FALSE);
#else defined MIX_INPUT	

	if(!NAME(PlayM4_OpenStream)(m_lPort, pBuf, sizeof(stInfo),  6*1000*1024))
	{
		sprintf(csError, "Open stream failed(%s)", MyErrorToString(NAME(PlayM4_GetLastError)(m_lPort)));
		MessageBox(csError);
		delete []pBuf;
		pBuf = NULL;
		throw 0;
	}
	NAME(PlayM4_SyncToAudio)(m_lPort, FALSE);
#endif

	delete []pBuf;
	pBuf = NULL;

	DWORD dw;
	m_hThread	  = NULL;
	m_hEventInput = NULL;
	m_hEventKill  = NULL;

	m_hThread	  =	CreateThread(NULL,0, LPTHREAD_START_ROUTINE (InputStreamThread),this,0,&dw);
	m_hEventInput =	CreateEvent(NULL,TRUE,FALSE,NULL);
	m_hEventKill  =	CreateEvent(NULL,FALSE,FALSE,NULL);
	if( !(m_hThread&&m_hEventInput&&m_hEventKill) )
	{
		MessageBox("Create thread failed");
		throw 0;
	}

	// try overlay mode!
//	NAME(PlayM4_SetOverlayMode)(m_lPort, TRUE, RGB(255,0,255));
	// try flip overlay mode
//	NAME(PlayM4_SetOverlayFlipMode)(m_lPort, TRUE);
	
	Play();
}


// Function:Close the stream.
void CPlayerDlg::CloseStream()
{
	Stop();
	NAME(PlayM4_CloseStream)(m_lPort);
	if(m_hStreamFile)
	{
		CloseHandle(m_hStreamFile);
		m_hStreamFile = NULL;
	}
	
	if(m_hThread)
	{
		SetEvent(m_hEventKill);
		DWORD dwStatus;
		for(int i = 0; i < 5; i++)
		{
			if(!::GetExitCodeThread(m_hThread, &dwStatus)|| (i == 4) )
			{
				TerminateThread(m_hThread, 0);
				TRACE("GetExitCode option error!-decodethread\n");
			}
			else
			{
				if(dwStatus == STILL_ACTIVE)
				{
					SetEvent(m_hEventKill);
					Sleep(2);
				}
				else
				{
					break;
				}
			}
		}
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}

	if(m_hEventInput)
	{
		CloseHandle(m_hEventInput);
		m_hEventInput = NULL;
	}

	if(m_hEventKill)
	{
		CloseHandle(m_hEventKill);
		m_hEventKill = NULL;
	}
	
	m_enumState = State_Close;
	m_bOpen = FALSE;
	m_bFileRefCreated =	FALSE;		
}

// set play state
// Funtion: Draw the status .
void CPlayerDlg::DrawStatus()
{
	DWORD nCurrentTime = NAME2(PlayM4_GetPlayedTime)(m_lPort);

	//TRACE("hytest: play position = %f!\n", NAME(PlayM4_GetPlayPos)(m_lPort));
	
	//TRACE("Get time is:%d\n",nCurrentTime);			
	DWORD nHour   = (nCurrentTime / 3600) % 24;
	DWORD nMinute = (nCurrentTime % 3600) / 60;
	DWORD nSecond = nCurrentTime % 60;
	int nPos = 0;
	if(m_bStreamType)
	{
		DWORD nFilePos = SetFilePointer(m_hStreamFile, 0, 0, FILE_CURRENT);
		nPos = nFilePos * PLAYER_SLIDER_MAX / m_dwMaxFileSize;
	}
	else
	{
#ifdef _FILE_POS
	float fPos = NAME(PlayM4_GetPlayPos)(m_lPort);
	nPos = int(fPos * PLAYER_SLIDER_MAX);
	
#else
	nPos = nCurrentTime * PLAYER_SLIDER_MAX / m_dwMaxFileTime;
			
#endif
	}
	
	DWORD nCurrentFrame = NAME2(PlayM4_GetCurrentFrameNum)(m_lPort);

	//TRACE("nCurrentFrame %d----------nCurrentTime %d--------------\n", nCurrentFrame, nCurrentTime);
	if(m_nSpeed > 0)
	{
		m_strPlayStateText.Format("speed X %d            %d/%d  %02d:%02d:%02d/%02d:%02d:%02d", GetSpeedModulus(), nCurrentFrame, m_dwTotalFrames, nHour, nMinute, nSecond, m_dwDisplayHour,m_dwDisplayMinute,m_dwDisplaySecond);
	}
	else if(m_nSpeed == 0)
	{
		m_strPlayStateText.Format("speed normal          %d/%d  %02d:%02d:%02d/%02d:%02d:%02d", nCurrentFrame, m_dwTotalFrames, nHour, nMinute, nSecond, m_dwDisplayHour,m_dwDisplayMinute,m_dwDisplaySecond);
	}
	else
	{
		m_strPlayStateText.Format("speed / %d            %d/%d  %02d:%02d:%02d/%02d:%02d:%02d", GetSpeedModulus(), nCurrentFrame, m_dwTotalFrames, nHour, nMinute, nSecond, m_dwDisplayHour,m_dwDisplayMinute,m_dwDisplaySecond);
	}

	if(m_bConvertAVI)
	{
		m_strPlayStateText.Format("Converting¡­¡­        %d/%d  %02d:%02d:%02d/%02d:%02d:%02d", nCurrentFrame, m_dwTotalFrames, nHour, nMinute, nSecond, m_dwDisplayHour, m_dwDisplayMinute, m_dwDisplaySecond);     
	}

	m_ctrlPlayText.ShowText(m_strPlayStateText);



	//adjust the slider.
//	INT nAdjust = (m_nPrePlayPos < nPos) ? 1 : 0;
	int nScrollPos = nCurrentFrame * PLAYER_SLIDER_MAX / (m_dwTotalFrames - 1);
	if (m_nPrePlayPos == nScrollPos)
	{
		return;
	}
	m_PlaySlider.SetScrollPos(nScrollPos);
	m_nPrePlayPos = nScrollPos;

	// test
	// TRACE("Current frame rate:%d\n",NAME(PlayM4_GetCurrentFrameRate)(m_lPort));
	// TRACE("Current time use ms:%d\n",NAME(PlayM4_GetPlayedTimeEx)(m_lPort));
	
}

// set state
void CPlayerDlg::SetState()
{
	int i;
	switch(m_enumState)
	{
	case State_Close:
		
		// button state

		m_ctrlBtnPlay.SetIcon(IDI_PLAY_DISABLE);
		m_ctrlBtnPlay.EnableWindow(FALSE);
		m_ctrlBtnPlay.SetCheck(0);

		m_ctrlBtnPause.SetIcon(IDI_PAUSE_DISABLE);
		m_ctrlBtnPause.EnableWindow(FALSE);
		m_ctrlBtnPause.SetCheck(0);

		m_ctrlBtnStop.SetIcon(IDI_STOP_DISABLE);
		m_ctrlBtnStop.EnableWindow(FALSE);
		m_ctrlBtnStop.SetCheck(0);

		m_ctrlBtnGStart.SetIcon(IDI_GOTOSTART_DISABLE);
		m_ctrlBtnGStart.EnableWindow(FALSE);

		m_ctrlBtnSlow.SetIcon(IDI_FASTBACKWARD_DISABLE);
		m_ctrlBtnSlow.EnableWindow(FALSE);

		m_ctrlBtnFast.SetIcon(IDI_FASTFORWARD_DISABLE);
		m_ctrlBtnFast.EnableWindow(FALSE);

		m_ctrlBtnGEnd.SetIcon(IDI_GOTOEND_DISABLE);
		m_ctrlBtnGEnd.EnableWindow(FALSE);

		m_ctrlStepBackward.SetIcon(IDI_STEPBACKWARD_DISABLE);
		m_ctrlStepBackward.EnableWindow(FALSE);

		m_ctrlStepForward.SetIcon(IDI_STEPFORWARD_DISABLE);
		m_ctrlStepForward.EnableWindow(FALSE);
		
		m_ctrlBtnCapPic.SetIcon(IDI_CAPPIC_DISABLE);
		m_ctrlBtnCapPic.EnableWindow(FALSE);
		
		m_ctrlBtnSound.SetIcon(IDI_SOUND_DISABLE);
		m_ctrlBtnSound.EnableWindow(FALSE);
		
		// menu state
		m_pMainMenu->EnableMenuItem(IDM_FILE_CLOSE, MF_GRAYED | MF_DISABLED);
//		m_pMainMenu->EnableMenuItem(IDM_CUT_FILE, MF_GRAYED | MF_DISABLED);
		m_pMainMenu->EnableMenuItem(IDM_SET_KEY, MF_ENABLED);

		for(i = 0; i < 3; i++)
		{
			m_pMainMenu->CheckMenuItem(IDM_VIEW_ZOOM_50 + i, MF_UNCHECKED);
		}
		m_pMainMenu->CheckMenuItem(IDM_VIEW_ZOOM_100, MF_CHECKED);
		m_pMainMenu->GetSubMenu(1)->EnableMenuItem(1, MF_DISABLED|MF_GRAYED|MF_BYPOSITION);
		m_pMainMenu->EnableMenuItem(IDM_VIEW_FULLSCREEN, MF_GRAYED | MF_DISABLED);
		m_pMainMenu->EnableMenuItem(IDM_INFO, MF_GRAYED | MF_DISABLED);
		m_pMainMenu->EnableMenuItem(IDM_SETDISPLAY, MF_GRAYED | MF_DISABLED);

		m_pMainMenu->EnableMenuItem(IDM_SEEK, MF_GRAYED | MF_DISABLED);
		m_pMainMenu->EnableMenuItem(IDM_VIDEO_CONTROL, MF_GRAYED | MF_DISABLED);
		m_pMainMenu->EnableMenuItem(IDM_PLAY_PAUSE, MF_GRAYED | MF_DISABLED);
		m_pMainMenu->EnableMenuItem(IDM_STOP, MF_GRAYED | MF_DISABLED);
		m_pMainMenu->EnableMenuItem(IDM_GOTOEND, MF_GRAYED | MF_DISABLED);
		m_pMainMenu->EnableMenuItem(IDM_GOTOSTART, MF_GRAYED | MF_DISABLED);
		m_pMainMenu->EnableMenuItem(IDM_STEPBACKWARD, MF_GRAYED | MF_DISABLED);
		m_pMainMenu->EnableMenuItem(IDM_STEPFORWARD, MF_GRAYED | MF_DISABLED);
		m_pMainMenu->EnableMenuItem(IDM_WATERMARK, MF_GRAYED | MF_DISABLED);

		m_pMainMenu->EnableMenuItem(IDM_PREVIEW50, MF_ENABLED);
		if(m_bHighFluid)
		{
			m_pMainMenu->CheckMenuItem(IDM_PREVIEW50, MF_CHECKED);
		}
		else
		{
			m_pMainMenu->CheckMenuItem(IDM_PREVIEW50, MF_UNCHECKED);
		}

		m_pMainMenu->EnableMenuItem(IDM_STREAM_TYPE, MF_ENABLED);
		for(i=0; i<2; i++)
		{
			m_pMainMenu->CheckMenuItem(IDM_TIMER1 + i, MF_UNCHECKED);
		}
		m_pMainMenu->CheckMenuItem(IDM_TIMER1, MF_CHECKED);
		m_pMainMenu->GetSubMenu(3)->EnableMenuItem(10, MF_ENABLED|MF_BYPOSITION);

		
		// scroll state	
		m_PlaySlider.SetScrollPos(0);
		m_PlaySlider.EnableWindow(FALSE);

#ifdef _WAVE_ADJ
		m_SoundSlider.SetPos(0);
#else
		m_SoundSlider.SetPos(0xffff>>1);
#endif
		m_SoundSlider.EnableWindow(FALSE);

		if((HBITMAP)m_HikvisionBmp != m_ctrlVideoPic.GetBitmap())
		{
			m_ctrlVideoPic.SetBitmap(m_HikvisionBmp);
		}
		
		InitWindowSize(WIDTH,HEIGHT_PAL);

		break;

	case State_Play:
		m_ctrlBtnPlay.SetIcon(IDI_PLAY_CHECK);
		m_ctrlBtnPlay.EnableWindow(TRUE);
		m_ctrlBtnPlay.SetCheck(1);

		m_ctrlBtnPause.SetIcon(IDI_PAUSE_ENABLE);
		m_ctrlBtnPause.EnableWindow(TRUE);
		m_ctrlBtnPause.SetCheck(0);

		m_ctrlBtnStop.SetIcon(IDI_STOP_ENABLE);
		m_ctrlBtnStop.EnableWindow(TRUE);
		m_ctrlBtnStop.SetCheck(0);

		if(m_bStreamType)
		{
			m_ctrlBtnGStart.SetIcon(IDI_GOTOSTART_DISABLE);
			m_ctrlBtnGStart.EnableWindow(FALSE);

			m_ctrlBtnGEnd.SetIcon(IDI_GOTOEND_DISABLE);
			m_ctrlBtnGEnd.EnableWindow(FALSE);

			m_ctrlStepBackward.SetIcon(IDI_STEPBACKWARD_DISABLE);
			m_ctrlStepBackward.EnableWindow(FALSE);
		}
		else
		{
			m_ctrlBtnGStart.SetIcon(IDI_GOTOSTART_ENABLE);
			m_ctrlBtnGStart.EnableWindow(TRUE);

			m_ctrlBtnGEnd.SetIcon(IDI_GOTOEND_ENABLE);
			m_ctrlBtnGEnd.EnableWindow(TRUE);

			m_ctrlStepBackward.SetIcon(IDI_STEPBACKWARD_ENABLE);
			m_ctrlStepBackward.EnableWindow(TRUE);
		}
	

		m_ctrlBtnSlow.SetIcon(IDI_FASTBACKWARD_ENABLE);
		m_ctrlBtnSlow.EnableWindow(TRUE);

		m_ctrlBtnFast.SetIcon(IDI_FASTFORWARD_ENABLE);
		m_ctrlBtnFast.EnableWindow(TRUE);

		m_ctrlStepForward.SetIcon(IDI_STEPFORWARD_ENABLE);
		m_ctrlStepForward.EnableWindow(TRUE);

		m_ctrlBtnCapPic.SetIcon(IDI_CAPPIC_AT, IDI_CAPPIC_ENABLE);
		m_ctrlBtnCapPic.EnableWindow(TRUE);

		m_ctrlBtnSound.EnableWindow(TRUE);
		if(m_bSound)
		{
			m_ctrlBtnSound.SetIcon(IDI_SOUND_ENABLE);
		}
		else
		{
			m_ctrlBtnSound.SetIcon(IDI_SOUND_DISABLE);
		}

		// menu state
		m_pMainMenu->EnableMenuItem(IDM_FILE_CLOSE, MF_ENABLED);
		m_pMainMenu->EnableMenuItem(IDM_SET_KEY, MF_GRAYED | MF_DISABLED);

		if(m_bFileRefCreated)
		{
//			m_pMainMenu->EnableMenuItem(IDM_CUT_FILE, MF_ENABLED);
			m_pMainMenu->EnableMenuItem(IDM_SEEK, MF_ENABLED);
		}
		else
		{
//			m_pMainMenu->EnableMenuItem(IDM_CUT_FILE, MF_GRAYED | MF_DISABLED);
			m_pMainMenu->EnableMenuItem(IDM_SEEK, MF_GRAYED | MF_DISABLED);
		}

		m_pMainMenu->GetSubMenu(1)->EnableMenuItem(1, MF_ENABLED | MF_BYPOSITION);
		m_pMainMenu->EnableMenuItem(IDM_VIEW_FULLSCREEN, MF_ENABLED);
		m_pMainMenu->EnableMenuItem(IDM_INFO, MF_ENABLED);
		m_pMainMenu->EnableMenuItem(IDM_SETDISPLAY, MF_ENABLED);
		
		m_pMainMenu->EnableMenuItem(IDM_VIDEO_CONTROL, MF_ENABLED);
		m_pMainMenu->EnableMenuItem(IDM_PLAY_PAUSE, MF_ENABLED);
		m_pMainMenu->EnableMenuItem(IDM_STOP, MF_ENABLED);
		m_pMainMenu->EnableMenuItem(IDM_STEPFORWARD, MF_ENABLED);
		m_pMainMenu->EnableMenuItem(IDM_WATERMARK, MF_ENABLED);

		m_pMainMenu->EnableMenuItem(IDM_PREVIEW50, MF_GRAYED | MF_DISABLED);
		m_pMainMenu->EnableMenuItem(IDM_STREAM_TYPE, MF_GRAYED | MF_DISABLED);
		m_pMainMenu->GetSubMenu(3)->EnableMenuItem(10, MF_DISABLED|MF_GRAYED|MF_BYPOSITION);
	
		if(m_bStreamType)
		{
			m_pMainMenu->EnableMenuItem(IDM_GOTOEND, MF_GRAYED | MF_DISABLED);
			m_pMainMenu->EnableMenuItem(IDM_GOTOSTART, MF_GRAYED | MF_DISABLED);
			m_pMainMenu->EnableMenuItem(IDM_STEPBACKWARD, MF_GRAYED | MF_DISABLED);
		}
		else
		{
			m_pMainMenu->EnableMenuItem(IDM_GOTOEND, MF_ENABLED);
			m_pMainMenu->EnableMenuItem(IDM_GOTOSTART, MF_ENABLED);
			m_pMainMenu->EnableMenuItem(IDM_STEPBACKWARD, MF_ENABLED);
		}

		// scroll state			
		if(m_bStreamType)
		{
			m_PlaySlider.EnableWindow(FALSE);
		}
		else
		{
			m_PlaySlider.EnableWindow(TRUE);
		}
		m_SoundSlider.EnableWindow(TRUE);
	
		if((HBITMAP)m_OverlayBmp != m_ctrlVideoPic.GetBitmap())
		{
			m_ctrlVideoPic.SetBitmap(m_OverlayBmp);
		}

		break;

	case State_Pause:
	case State_Step:
		m_ctrlBtnPlay.SetIcon(IDI_PLAY_ENABLE);
		m_ctrlBtnPlay.EnableWindow(TRUE);
		m_ctrlBtnPlay.SetCheck(0);

		m_ctrlBtnPause.SetIcon(IDI_PAUSE_CHECK);
		m_ctrlBtnPause.EnableWindow(TRUE);
		m_ctrlBtnPause.SetCheck(1);

		m_ctrlBtnStop.SetIcon(IDI_STOP_ENABLE);
		m_ctrlBtnStop.EnableWindow(TRUE);
		m_ctrlBtnStop.SetCheck(0);

		if(m_bStreamType)
		{
			m_ctrlBtnGStart.SetIcon(IDI_GOTOSTART_DISABLE);
			m_ctrlBtnGStart.EnableWindow(FALSE);

			m_ctrlBtnGEnd.SetIcon(IDI_GOTOEND_DISABLE);
			m_ctrlBtnGEnd.EnableWindow(FALSE);

			m_ctrlStepBackward.SetIcon(IDI_STEPBACKWARD_DISABLE);
			m_ctrlStepBackward.EnableWindow(FALSE);
		}
		else
		{
			m_ctrlBtnGStart.SetIcon(IDI_GOTOSTART_ENABLE);
			m_ctrlBtnGStart.EnableWindow(TRUE);

			m_ctrlBtnGEnd.SetIcon(IDI_GOTOEND_ENABLE);
			m_ctrlBtnGEnd.EnableWindow(TRUE);

			m_ctrlStepBackward.SetIcon(IDI_STEPBACKWARD_ENABLE);
			m_ctrlStepBackward.EnableWindow(TRUE);
		}
	
		m_ctrlBtnSlow.SetIcon(IDI_FASTBACKWARD_ENABLE);
		m_ctrlBtnSlow.EnableWindow(TRUE);

		m_ctrlBtnFast.SetIcon(IDI_FASTFORWARD_ENABLE);
		m_ctrlBtnFast.EnableWindow(TRUE);

		m_ctrlStepForward.SetIcon(IDI_STEPFORWARD_ENABLE);
		m_ctrlStepForward.EnableWindow(TRUE);

		m_ctrlBtnCapPic.SetIcon(IDI_CAPPIC_AT, IDI_CAPPIC_ENABLE);
		m_ctrlBtnCapPic.EnableWindow(TRUE);
		
		break;

	case State_Stop:
		m_ctrlBtnPlay.SetIcon(IDI_PLAY_ENABLE);
		m_ctrlBtnPlay.EnableWindow(TRUE);
		m_ctrlBtnPlay.SetCheck(0);

		m_ctrlBtnPause.SetIcon(IDI_PAUSE_ENABLE);
		m_ctrlBtnPause.EnableWindow(TRUE);
		m_ctrlBtnPause.SetCheck(0);

		m_ctrlBtnStop.SetIcon(IDI_STOP_CHECK);
		m_ctrlBtnStop.EnableWindow(TRUE);
		m_ctrlBtnStop.SetCheck(1);

		m_ctrlBtnGStart.SetIcon(IDI_GOTOSTART_DISABLE);
		m_ctrlBtnGStart.EnableWindow(FALSE);

		m_ctrlBtnSlow.SetIcon(IDI_FASTBACKWARD_ENABLE);
		m_ctrlBtnSlow.EnableWindow(TRUE);

		m_ctrlBtnFast.SetIcon(IDI_FASTFORWARD_ENABLE);
		m_ctrlBtnFast.EnableWindow(TRUE);

		m_ctrlBtnGEnd.SetIcon(IDI_GOTOEND_DISABLE);
		m_ctrlBtnGEnd.EnableWindow(FALSE);

		m_ctrlStepBackward.SetIcon(IDI_STEPBACKWARD_DISABLE);
		m_ctrlStepBackward.EnableWindow(FALSE);

		m_ctrlStepForward.SetIcon(IDI_STEPFORWARD_DISABLE);
		m_ctrlStepForward.EnableWindow(FALSE);
		
		m_ctrlBtnCapPic.SetIcon(IDI_CAPPIC_DISABLE);
		m_ctrlBtnCapPic.EnableWindow(FALSE);
		
		// menu state
		m_pMainMenu->EnableMenuItem(IDM_FILE_CLOSE, MF_ENABLED);

		if(m_bFileRefCreated)
		{
//			m_pMainMenu->EnableMenuItem(IDM_CUT_FILE, MF_ENABLED);
			m_pMainMenu->EnableMenuItem(IDM_SEEK, MF_ENABLED);
		}
		else
		{
//			m_pMainMenu->EnableMenuItem(IDM_CUT_FILE, MF_DISABLED);
			m_pMainMenu->EnableMenuItem(IDM_SEEK, MF_DISABLED);
		}

		m_pMainMenu->GetSubMenu(1)->EnableMenuItem(1, MF_DISABLED|MF_GRAYED|MF_BYPOSITION);
		m_pMainMenu->EnableMenuItem(IDM_VIEW_FULLSCREEN, MF_GRAYED | MF_DISABLED);
		m_pMainMenu->EnableMenuItem(IDM_INFO, MF_GRAYED | MF_DISABLED);
		m_pMainMenu->EnableMenuItem(IDM_SETDISPLAY, MF_GRAYED | MF_DISABLED);

		m_pMainMenu->EnableMenuItem(IDM_SEEK, MF_GRAYED | MF_DISABLED);
		m_pMainMenu->EnableMenuItem(IDM_VIDEO_CONTROL, MF_GRAYED | MF_DISABLED);
		m_pMainMenu->EnableMenuItem(IDM_PLAY_PAUSE, MF_ENABLED);
		m_pMainMenu->EnableMenuItem(IDM_STOP, MF_GRAYED | MF_DISABLED);
		m_pMainMenu->EnableMenuItem(IDM_GOTOEND, MF_GRAYED | MF_DISABLED);
		m_pMainMenu->EnableMenuItem(IDM_GOTOSTART, MF_GRAYED | MF_DISABLED);
		m_pMainMenu->EnableMenuItem(IDM_STEPBACKWARD, MF_GRAYED | MF_DISABLED);
		m_pMainMenu->EnableMenuItem(IDM_STEPFORWARD, MF_GRAYED | MF_DISABLED);
		m_pMainMenu->EnableMenuItem(IDM_WATERMARK, MF_GRAYED | MF_DISABLED);

		m_pMainMenu->EnableMenuItem(IDM_PREVIEW50, MF_ENABLED);
		m_pMainMenu->EnableMenuItem(IDM_STREAM_TYPE, MF_GRAYED | MF_DISABLED);
		m_pMainMenu->GetSubMenu(3)->EnableMenuItem(10, MF_DISABLED|MF_GRAYED|MF_BYPOSITION);

		// scroll state
		m_PlaySlider.SetScrollPos(0);
		m_PlaySlider.EnableWindow(FALSE);
		
#ifdef _WAVE_ADJ
		m_SoundSlider.SetPos(0);
#else
		m_SoundSlider.SetPos(0xffff>>1);
#endif
		m_SoundSlider.EnableWindow(FALSE);
		
		if((HBITMAP)m_HikvisionBmp != m_ctrlVideoPic.GetBitmap())
		{
			m_ctrlVideoPic.SetBitmap(m_HikvisionBmp);
		}

		break;
		
	default:
		break;
	}
	
	SortControl();
	
	UpdateData(FALSE);
	NAME2(PlayM4_RefreshPlay)(m_lPort);
}

// set avi state
void CPlayerDlg::SetAVIState()
{
	// button state

	m_ctrlBtnPlay.SetIcon(IDI_PLAY_DISABLE);
	m_ctrlBtnPlay.EnableWindow(FALSE);
	m_ctrlBtnPlay.SetCheck(0);

	m_ctrlBtnPause.SetIcon(IDI_PAUSE_DISABLE);
	m_ctrlBtnPause.EnableWindow(FALSE);
	m_ctrlBtnPause.SetCheck(0);

	m_ctrlBtnStop.SetIcon(IDI_STOP_ENABLE);
	m_ctrlBtnStop.EnableWindow(TRUE);
	m_ctrlBtnStop.SetCheck(0);

	m_ctrlBtnGStart.SetIcon(IDI_GOTOSTART_DISABLE);
	m_ctrlBtnGStart.EnableWindow(FALSE);

	m_ctrlBtnSlow.SetIcon(IDI_FASTBACKWARD_DISABLE);
	m_ctrlBtnSlow.EnableWindow(FALSE);

	m_ctrlBtnFast.SetIcon(IDI_FASTFORWARD_DISABLE);
	m_ctrlBtnFast.EnableWindow(FALSE);

	m_ctrlBtnGEnd.SetIcon(IDI_GOTOEND_DISABLE);
	m_ctrlBtnGEnd.EnableWindow(FALSE);

	m_ctrlStepBackward.SetIcon(IDI_STEPBACKWARD_DISABLE);
	m_ctrlStepBackward.EnableWindow(FALSE);

	m_ctrlStepForward.SetIcon(IDI_STEPFORWARD_DISABLE);
	m_ctrlStepForward.EnableWindow(FALSE);
	
	m_ctrlBtnCapPic.SetIcon(IDI_CAPPIC_DISABLE);
	m_ctrlBtnCapPic.EnableWindow(FALSE);
	
	m_ctrlBtnSound.SetIcon(IDI_SOUND_DISABLE);
	m_ctrlBtnSound.EnableWindow(FALSE);
	
	// menu state
	m_pMainMenu->EnableMenuItem(IDM_FILE_CLOSE, MF_ENABLED);
//	m_pMainMenu->EnableMenuItem(IDM_CUT_FILE, MF_GRAYED | MF_DISABLED);

	m_pMainMenu->GetSubMenu(1)->EnableMenuItem(1, MF_DISABLED | MF_GRAYED | MF_BYPOSITION);
	m_pMainMenu->EnableMenuItem(IDM_VIEW_FULLSCREEN, MF_GRAYED | MF_DISABLED);
	m_pMainMenu->EnableMenuItem(IDM_INFO, MF_GRAYED | MF_DISABLED);
	m_pMainMenu->EnableMenuItem(IDM_SETDISPLAY, MF_GRAYED | MF_DISABLED);

	m_pMainMenu->EnableMenuItem(IDM_SEEK, MF_GRAYED | MF_DISABLED);
	m_pMainMenu->EnableMenuItem(IDM_VIDEO_CONTROL, MF_GRAYED | MF_DISABLED);
	m_pMainMenu->EnableMenuItem(IDM_PLAY_PAUSE, MF_GRAYED | MF_DISABLED);
	m_pMainMenu->EnableMenuItem(IDM_STOP, MF_ENABLED);
	m_pMainMenu->EnableMenuItem(IDM_GOTOEND, MF_GRAYED | MF_DISABLED);
	m_pMainMenu->EnableMenuItem(IDM_GOTOSTART, MF_GRAYED | MF_DISABLED);
	m_pMainMenu->EnableMenuItem(IDM_STEPBACKWARD, MF_GRAYED | MF_DISABLED);
	m_pMainMenu->EnableMenuItem(IDM_STEPFORWARD, MF_GRAYED | MF_DISABLED);

	m_pMainMenu->EnableMenuItem(IDM_PREVIEW50, MF_GRAYED | MF_DISABLED);
	m_pMainMenu->EnableMenuItem(IDM_STREAM_TYPE, MF_GRAYED | MF_DISABLED);	
	m_pMainMenu->GetSubMenu(3)->EnableMenuItem(10, MF_GRAYED | MF_DISABLED | MF_BYPOSITION);

	
	// scroll state	
	m_PlaySlider.SetScrollPos(0);
	m_PlaySlider.EnableWindow(FALSE);


	if((HBITMAP)m_OverlayBmp != m_ctrlVideoPic.GetBitmap())
	{
		m_ctrlVideoPic.SetBitmap(m_OverlayBmp);
	}

}

// get speed modulus to show int the static text control
DWORD CPlayerDlg::GetSpeedModulus()
{
	DWORD dwValue = 0;
	switch(m_nSpeed)
	{
	case 1:
	case -1:
		dwValue = 2;
		break;

	case 2:
	case -2:
		dwValue = 4;
		break;

	case 3:
	case -3:
		dwValue = 8;
		break;

	case 4:
	case -4:
		dwValue = 16;
		break;

	default:
		break;
	}

	return dwValue;
}

// resize the dialog
void CPlayerDlg::SetWindowSize()
{
	CRect rcWin, rcClient;
	GetWindowRect(&rcWin);
	GetClientRect(&rcClient);
	m_dwDlgEdge = (rcWin.Width() - rcClient.Width()) >> 1;
	ClientToScreen(&rcClient);
	m_dwDlgTopSize = rcClient.top - rcWin.top;

	TRACE("init window size!\n");
	NAME(PlayM4_GetPictureSize)(m_lPort, &m_nWidth, &m_nHeight);
	m_pDisplayRegion->SetResolution(m_nHeight, m_nWidth);
	TRACE("get window size ok\n");

	if (m_nWidth == 704 && (m_nHeight == 288 || m_nHeight == 240))
	{
		m_nHeight <<= 1;
	}
	
	DWORD nWindowHeight = m_nHeight + PANNEL_HEIGHT + m_dwDlgTopSize + m_dwDlgEdge + 10;
	DWORD nWindowWidth  = m_nWidth + (m_dwDlgEdge << 1);

	nWindowHeight = min(nWindowHeight, m_dwScreenHeight);
	nWindowWidth  = min(nWindowWidth,  m_dwScreenWidth);
	if (nWindowWidth < 200)
	{
		nWindowHeight += 20;
	}
	MoveWindow(
		(m_dwScreenWidth  -nWindowWidth)  / 2 + m_rcScreen.left,
		(m_dwScreenHeight -nWindowHeight) / 2 + m_rcScreen.top,
		nWindowWidth,
		nWindowHeight,
		TRUE);
	TRACE("exit window size\n");

}

// Funtion: sort the controls
void CPlayerDlg::SortControl()
{
	//TRACE("init SORT\n");

	// if the dialog doesn't support resizing, return;
	if( !(GetStyle() & WS_SIZEBOX) )
	{
		return;
	}

	CRect rcClient, rcVideo;
	
	GetClientRect(&rcClient);
	DWORD x=LEFT_EDGE+10,y=rcClient.bottom-STATE_HEIGHT;
	
	//STATE 
	m_ctrlPlayText.MoveWindow(0,y,rcClient.Width()+1,STATE_HEIGHT,TRUE);


	x=LEFT_EDGE;
	y-=BUTTON_SIZE+STATE_UP;

	//BUTTON
	m_ctrlBtnPlay.MoveWindow(x,y,BUTTON_SIZE,BUTTON_SIZE,TRUE);
	x+=BUTTON_SIZE;
	m_ctrlBtnPause.MoveWindow(x,y,BUTTON_SIZE,BUTTON_SIZE,TRUE);
	x+=BUTTON_SIZE;
	m_ctrlBtnStop.MoveWindow(x,y,BUTTON_SIZE,BUTTON_SIZE,TRUE);
	x+=BUTTON_SIZE;

	x+=INTERVAL;
	GetDlgItem(IDC_INTER1)->MoveWindow(x,y,INTERVAL_SIZE,BUTTON_SIZE,TRUE);
	x+=INTERVAL_SIZE;
	x+=INTERVAL;

	m_ctrlBtnGStart.MoveWindow(x,y,BUTTON_SIZE,BUTTON_SIZE,TRUE);
	x+=BUTTON_SIZE;
	m_ctrlBtnSlow.MoveWindow(x,y,BUTTON_SIZE,BUTTON_SIZE,TRUE);
	x+=BUTTON_SIZE;
	m_ctrlBtnFast.MoveWindow(x,y,BUTTON_SIZE,BUTTON_SIZE,TRUE);
	x+=BUTTON_SIZE;
	m_ctrlBtnGEnd.MoveWindow(x,y,BUTTON_SIZE,BUTTON_SIZE,TRUE);
	x+=BUTTON_SIZE;

	x+=INTERVAL;
	GetDlgItem(IDC_INTER2)->MoveWindow(x,y,INTERVAL_SIZE,BUTTON_SIZE,TRUE);
	x+=INTERVAL_SIZE;
	x+=INTERVAL;

	m_ctrlStepBackward.MoveWindow(x,y,BUTTON_SIZE,BUTTON_SIZE,TRUE);
	x+=BUTTON_SIZE;

	m_ctrlStepForward.MoveWindow(x,y,BUTTON_SIZE,BUTTON_SIZE,TRUE);
	x+=BUTTON_SIZE;

	x+=INTERVAL;
	GetDlgItem(IDC_INTER3)->MoveWindow(x,y,INTERVAL_SIZE,BUTTON_SIZE,TRUE);
	x+=INTERVAL_SIZE;
	x+=INTERVAL;

	m_ctrlBtnCapPic.MoveWindow(x,y,BUTTON_SIZE,BUTTON_SIZE,TRUE);
	x+=BUTTON_SIZE;

	
	//SOUND
	x=rcClient.right-SOUND_SLIDER_WIDTH;
	m_SoundSlider.MoveWindow(x,y+BUTTON_SIZE-SOUND_SLIDER_HEIGHT,SOUND_SLIDER_WIDTH,SOUND_SLIDER_HEIGHT,TRUE);
	x-=BUTTON_SIZE;

	m_ctrlBtnSound.MoveWindow(x,y,BUTTON_SIZE,BUTTON_SIZE,TRUE);

	y-=BUTTON_SIZE+BUTTON_UP;
	//PLAY SLIDER
	x=0;
	GetDlgItem(IDC_PLAY_RECT_SLIDER)->MoveWindow(x,y,rcClient.Width(),PLAY_SLIDER_HEIGHT,TRUE);
	m_PlaySlider.MoveWindowEx(x,y,rcClient.Width(),PLAY_SLIDER_HEIGHT,TRUE);
	y-=PLAY_SLIDER_UP;

	//pic show
	m_ctrlVideoPic.MoveWindow(0,0,rcClient.Width(),y,TRUE);

	NAME2(PlayM4_RefreshPlay)(m_lPort);

	// redraw whole the dialog rect except the video show rect
	m_ctrlVideoPic.GetClientRect(&rcVideo);
	CRect rect;
	rect.left   = rcClient.left;
	rect.top    = rcVideo.bottom;
	rect.right  = rcClient.right;
	rect.bottom = rcClient.bottom;
	InvalidateRect(&rect);
	//TRACE("exit SORT\n");
}


// Funtion: Init window size.
void CPlayerDlg::InitWindowSize(DWORD cx,DWORD cy)
{
	TRACE("init window\n");
	CRect rcWin, rcClient;
	GetWindowRect(&rcWin);
	GetClientRect(&rcClient);
	m_dwDlgEdge = (rcWin.Width() - rcClient.Width()) >> 1;
	ClientToScreen(&rcClient);
	m_dwDlgTopSize = rcClient.top - rcWin.top;
	
	DWORD nWindowWidth  = cx + (m_dwDlgEdge << 1);
	DWORD nWindowHeight = cy + PANNEL_HEIGHT + m_dwDlgTopSize + m_dwDlgEdge + 10;
	
	nWindowHeight = min(nWindowHeight, m_dwScreenHeight);
	nWindowWidth  = min(nWindowWidth,  m_dwScreenWidth);

	MoveWindow(
		(m_dwScreenWidth  - nWindowWidth)/2 + m_rcScreen.left,
		(m_dwScreenHeight - nWindowHeight)/2+ m_rcScreen.top,
		nWindowWidth,
		nWindowHeight,
		TRUE);
	
	SortControl();

	//for the small picture size.The menu will be high. 
	GetWindowRect(&rcWin);
	GetClientRect(&rcClient);
	m_dwDlgEdge = (rcWin.Width() - rcClient.Width()) >> 1;
	ClientToScreen(&rcClient);
	DWORD nTopSize = rcClient.top - rcWin.top;
	if(nTopSize != m_dwDlgTopSize)
	{
		TRACE("re init window!!!!!!\n");
		m_dwDlgTopSize = nTopSize;
		nWindowHeight  = cy + PANNEL_HEIGHT + m_dwDlgTopSize + m_dwDlgEdge + 10;
		nWindowWidth   = cx + (m_dwDlgEdge << 1);

		nWindowHeight = min(nWindowHeight, m_dwScreenHeight);
		nWindowWidth  = min(nWindowWidth,  m_dwScreenWidth);

		MoveWindow(
				  (m_dwScreenWidth  - nWindowWidth)  / 2 + m_rcScreen.left,
				  (m_dwScreenHeight - nWindowHeight) / 2 + m_rcScreen.top,
				  nWindowWidth,
				  nWindowHeight,
				  TRUE);
		SortControl();
	}
}


// others
// geton pic rect
CRect CPlayerDlg::GetOnPicRect(CRect rcWnd, CRect rcOnWnd, LONG nPicWidth, LONG nPicHeight)
{
	CRect rcOnPic;
	//LONG nScaleX=rcWnd.Width()
	long nWndX = rcOnWnd.left - rcWnd.left;
	long nWndY = rcOnWnd.top  - rcWnd.top;
	if(nWndX < 0)
	{
		nWndX = 0;
	}

	if(nWndY < 0)
	{
		nWndY = 0;
	}

	rcOnPic.left   = nWndX * nPicWidth  / rcWnd.Width();
	rcOnPic.top    = nWndY * nPicHeight / rcWnd.Height();
	rcOnPic.right  = rcOnPic.left + rcOnWnd.Width()  * nPicWidth/rcWnd.Width();
	rcOnPic.bottom = rcOnPic.top  + rcOnWnd.Height() * nPicHeight/rcWnd.Height();
	
	if(rcOnPic.right > nPicWidth)
	{
		rcOnPic.right = nPicWidth;
	}

	if(rcOnPic.bottom > nPicHeight)
	{
		rcOnPic.bottom = nPicHeight;
	}
	
	return rcOnPic;
}
	
// Funtion: test the capability of your system.
void CPlayerDlg::TestCapability(DWORD nDeviceNum)
{
	CString csCap="";
#if (WINVER > 0x0400)
	int nFlag=NAME(PlayM4_GetCapsEx)(nDeviceNum);
#else
	int nFlag=NAME(PlayM4_GetCaps());
#endif

	if(!(nFlag&SUPPORT_SSE))
	{
		csCap+="Don't support SSE instruction set;\r\n";
	}

	if(!(nFlag&SUPPORT_DDRAW))
	{
		csCap+="Create DirectDraw faild;\r\n";
	}

	if(!(nFlag&SUPPORT_BLT))
	{
		csCap+="Error when blitting overlay or offscreen;Error when blitting overlay or offscreen;\r\n";
	}

	if(!(nFlag&SUPPORT_BLTFOURCC))
	{
		csCap+="Don't support color-space conversions;\r\n";
	}

	if(!(nFlag&SUPPORT_BLTSHRINKX))
	{
		csCap+="Don't support arbitrary shrinking of a surface along the x-axis\r\n";
	}

	if(!(nFlag&SUPPORT_BLTSHRINKY))
	{
		csCap+="Don't supports arbitrary shrinking of a surface along the y-axis (vertically);\r\n";
	}

	if(!(nFlag&SUPPORT_BLTSTRETCHX))
	{
		csCap+="Don't supports arbitrary stretching of a surface along the x-axis;\r\n";
	}

	if(!(nFlag&SUPPORT_BLTSTRETCHY))
	{
		csCap+="Don't supports arbitrary stretching of a surface along the y-axis;\r\n";
	}
	
	if(csCap.GetLength()>0)
	{
		csCap+="If your video adapter chip is made by nVidia,please update the new driver!\r\n";
		MessageBox(csCap,"Warning",MB_OK);
	}
}

void CPlayerDlg::SetDisplayRegion(RECT Rect)
{
	if (m_enumState != State_Step && m_enumState != State_Pause) 
	{
		return;
	}

	NAME(PlayM4_SetDisplayRegion)(m_lPort, 0, &Rect, GetDlgItem(IDC_SHOW)->m_hWnd, TRUE);
	UpdateWindow();
 	NAME2(PlayM4_RefreshPlayEx)(m_lPort, 0);
}
/*************************************************************************/
/*************************************************************************/
/*************************************************************************/
/*  assistant operation over
/*************************************************************************/
/*************************************************************************/
/*************************************************************************/




/*************************************************************************/
/*************************************************************************/
/*************************************************************************/
/* button operation begin
/*************************************************************************/
/*************************************************************************/
/*************************************************************************/
// play / pause / stop
void CPlayerDlg::Play()
{
	if(m_enumState == State_Play)
	{
		return;
	}
	
	if(m_enumState == State_Pause)
	{
//		DWORD nPreSpeed = m_nSpeed;

		NAME(PlayM4_Pause)(m_lPort, FALSE);

		m_enumState = State_Play;

		// when u called this api, the speed is adjust to normal speed, so we must resume it.
//		NAME(PlayM4_Play)(m_lPort, m_ctrlVideoPic.GetSafeHwnd());
//		m_nSpeed = 0;
//		ThrowB(IDM_THROW0);
//		AdjustSpeed(nPreSpeed);
	}
	else if (m_enumState == State_Stop && m_bStreamType)
	{
		Open();
	}
	else if(m_enumState == State_Step)
	{
		NAME(PlayM4_Play)(m_lPort, m_ctrlVideoPic.GetSafeHwnd()); // sorin: just to test behavior
		m_nSpeed = 0;
		ThrowB(IDM_THROW0);

		m_enumState = State_Play;
	}
	else
	{
		ZeroMemory(&m_strWaterMark, sizeof(WATERMARK_VER1_INFO));
		//NAME(PlayM4_SetCheckWatermarkCallBack)(m_lPort, funCheckWatermark, (DWORD)this);
		
		m_nSpeed = 0;
		ThrowB(IDM_THROW0);

		// set image sharpen level, default is 0
		//NAME(PlayM4_SetImageSharpen)(m_lPort, m_dwImageSharpenLevel);
		// set play mode, high fluid
		//NAME(PlayM4_SetPlayMode)(m_lPort, m_bHighFluid);
		
		// set decode type
		UINT nMenuState;
		for(int i = IDM_DECODE_NORMAL; i <= IDM_DECODE_NONE; i ++ )
		{
			nMenuState = m_pMainMenu->GetMenuState(i, MF_BYCOMMAND);
			if(nMenuState & MF_CHECKED)
			{
				//NAME(PlayM4_SetDecodeFrameType)(m_lPort, i - IDM_DECODE_NORMAL);
				break;
			}
		}

		if(m_bConvertAVI)
		{
			NAME(PlayM4_SetDecCallBackMend)(m_lPort, DecCBFun, DWORD(this));
			m_bFileRefCreated = FALSE;
		}

		if(m_bStreamType)
		{
            if(m_dwSysFormat == SYSTEM_HIK)
            {
			    ::SetFilePointer(m_hStreamFile, m_dwHeadSize, 0, FILE_BEGIN);
            }
            else
            {
			    ::SetFilePointer(m_hStreamFile, 0, 0, FILE_BEGIN);
            }
			NAME(PlayM4_ResetSourceBuffer)(m_lPort);
			SetEvent(m_hEventInput);
		}

		if(NAME(PlayM4_Play)(m_lPort, m_ctrlVideoPic.m_hWnd)) // sorin
		{
			m_enumState = State_Play;

			SetTimer(PLAY_TIMER, 500, NULL);

			NAME2(PlayM4_RefreshPlay)(m_lPort);
		}

		m_bSound = NAME(PlayM4_PlaySound)(m_lPort);	
	}	

	if (m_pDisplayRegion->m_bValid) 
	{
		m_pDisplayRegion->Enable(FALSE);
 	}
}

void CPlayerDlg::Pause()
{
	if(m_enumState == State_Play)
	{
		NAME(PlayM4_Pause)(m_lPort, TRUE);
		m_enumState = State_Pause;
	}

	if (m_pDisplayRegion->m_bValid) 
	{
		Sleep(50);	
		m_pDisplayRegion->Enable(TRUE);
		m_pDisplayRegion->DrawRectangle();
	}
}

void CPlayerDlg::Stop()
{
	if(m_enumState == State_Stop)
	{
		return;
	}
	
	KillTimer(PLAY_TIMER);

	// stop sound
	if(NAME(PlayM4_StopSound)())
	{
		m_bSound = FALSE;
		m_ctrlBtnSound.SetIcon(IDI_SOUND_DISABLE);
	}
	
	// stop video
	if(NAME(PlayM4_Stop)(m_lPort))	
	{
		m_strPlayStateText = "Stop";
		m_ctrlPlayText.ShowText(m_strPlayStateText);
	
		if(m_bStreamType)
		{
			ResetEvent(m_hEventInput);
		}
	}	

	if(m_bConvertAVI)
	{
	   g_classAVI.ReleaseResource();
		
	   m_strSaveAVIPath = _T("");
	   m_bConvertAVI = FALSE;    
	}

	ZeroMemory(&m_strWaterMark, sizeof(WATERMARK_VER1_INFO));
	if(m_pWatermarkDlg->m_hWnd)
	{
		m_pWatermarkDlg->Clear();
	}

	m_enumState = State_Stop;
}

// gotostart / slow / fast / gotoend
void CPlayerDlg::GotoStart() 
{
	// TODO: Add your control notification handler code here

	if(m_bFileRefCreated)
	{
		NAME(PlayM4_SetPlayedTimeEx)(m_lPort, 0);
	}
	else
	{
		NAME(PlayM4_SetPlayPos)(m_lPort, 0);
	}
}

void CPlayerDlg::GotoEnd() 
{
	// TODO: Add your control notification handler code here
	
	if(m_bFileRefCreated)
	{
		//Note: May create many WM_FILE_END message. The best way is to synchronize the option;
		
//		int nEndFrame = m_dwTotalFrames;
//		int nCurFrame = NAME2(PlayM4_GetCurrentFrameNum)(m_lPort);
		while( !NAME(PlayM4_SetPlayedTimeEx)(m_lPort, m_dwMaxFileTime * 1000 ) )
		{
			//TRACE("FrameNum is :%d\n",nEndFrame);
//			if(nEndFrame <= int(max(0, m_dwTotalFrames - 3)))
//			{
//				NAME2(PlayM4_SetCurrentFrameNum)(m_lPort, nCurFrame);
//				break;
//			}
		}
	}
	else
	{
		NAME(PlayM4_SetPlayPos)(m_lPort, 1);
	}

}

void CPlayerDlg::Fast() 
{
	// TODO: Add your control notification handler code here

	// Throw B-Frame ,improve the performance;
	
	if(NAME(PlayM4_Fast)(m_lPort))
	{
		m_nSpeed ++;
		if(m_nSpeed > 0)
		{
			ThrowB(IDM_THROW2);	
		}
	}	
}

void CPlayerDlg::Slow() 
{
	// TODO: Add your control notification handler code here
	
	if(NAME(PlayM4_Slow)(m_lPort))
	{
		m_nSpeed --;
		if(m_nSpeed <= 0)
		{
			ThrowB(IDM_THROW0);
		}
	}
}

void CPlayerDlg::AdjustSpeed(int nSpeed)
{
	int nCyc = 0;
	while(m_nSpeed != nSpeed)
	{
		if(nSpeed > m_nSpeed)
		{
			Fast();	
		}
		else if(nSpeed < m_nSpeed)
		{
			Slow();
		}

		nCyc ++;
		if(nCyc >= 10)
		{
			break;
		}
	}
}

// stepback / stepfore / cap picture
void CPlayerDlg::StepBackward() 
{
	// TODO: Add your control notification handler code here

	if(m_bFileRefCreated)
	{
		//NAME(PlayM4_OneByOneBack)(m_lPort);
		DWORD nCurrentFrame = NAME2(PlayM4_GetCurrentFrameNum)(m_lPort);
		NAME2(PlayM4_SetCurrentFrameNum)(m_lPort,nCurrentFrame-1);
	
		m_enumState = State_Step;
		if (m_pDisplayRegion->m_bValid) 
		{
			Sleep(50);	
			m_pDisplayRegion->Enable(TRUE);
			m_pDisplayRegion->DrawRectangle();
		}
	}
	else
	{
		MessageBox("File reference hasn't been created.", NULL, MB_OK);
	}
}

void CPlayerDlg::StepForward() 
{
	// TODO: Add your control notification handler code here
	
	// you can do it like the followed too.
	DWORD nCurrentFrame = NAME2(PlayM4_GetCurrentFrameNum)(m_lPort);
	NAME2(PlayM4_SetCurrentFrameNum)(m_lPort,nCurrentFrame+1);
	
	/*
	ThrowB(IDM_THROW0);          // when step forward one by one, don't throw B frame;
	NAME(PlayM4_OneByOne)(m_lPort);
	*/
	m_enumState = State_Step;
	if (m_pDisplayRegion->m_bValid) 
	{
		Sleep(50);	
		m_pDisplayRegion->Enable(TRUE);
		m_pDisplayRegion->DrawRectangle();
	}
}

void CPlayerDlg::GetPic(PBYTE pImage, DWORD nBufSize)
{
	CString sFilePath;
	CFile	clsFile;

	DWORD   pImageSize	= 0;
	
	if(m_nCapPicType == 1)
	{
		if( !NAME(PlayM4_GetJPEG)(m_lPort, pImage, nBufSize, &pImageSize) )
		{
			return;
		}

		if(m_strCapPicPath.Compare(""))
		{
			sFilePath.Format("%s\\capture%02d.jpeg", m_strCapPicPath, m_npic_jpeg);
		}
		else
		{
			sFilePath.Format("C:\\capture%02d.jpeg", m_npic_jpeg);
		}
	}
	else
	{
		if( !NAME(PlayM4_GetBMP)(m_lPort, pImage, nBufSize, &pImageSize) )
		{
			return;
		}

		if(m_strCapPicPath.Compare(""))
		{
			sFilePath.Format("%s\\capture%02d.bmp", m_strCapPicPath, m_npic_bmp);
		}
		else
		{
			sFilePath.Format("C:\\capture%02d.bmp", m_npic_bmp);
		}
	}
	
	if(!clsFile.Open(sFilePath,CFile::modeCreate|CFile::modeWrite))
	{
		return;
	}

	try							
	{
		clsFile.Write(pImage, pImageSize);
		clsFile.Close();

		if(m_nCapPicType == 0)
		{
			m_npic_bmp++;
		}
		else
		{
			m_npic_jpeg++;
		}
	}
	catch (CFileException* e) 
	{
		e->ReportError();
		e->Delete();
	}
}

void CPlayerDlg::Cappic() 
{
	// TODO: Add your control notification handler code here
	PBYTE	pImage		= NULL;
	DWORD   nBufSize	= m_nWidth * m_nHeight * 5;  // ±£Ö¤×ã¹»´ó¼´¿É(²»»áÐ¡ÓÚÒ»·ùbmp»òÕßjpegÍ¼Ïñ´óÐ¡)

	try
	{
		pImage = new BYTE[nBufSize];
		if(NULL == pImage)
		{
			throw 0;
		}
		
		GetPic(pImage, nBufSize);
	}
	catch(...)
	{
	}

	if(pImage != NULL)
	{
		delete []pImage;
		pImage = NULL;
	}
}

// close or open sound
void CPlayerDlg::Sound() 
{
	// TODO: Add your control notification handler code here
	if(m_bSound)
	{
		if(NAME(PlayM4_StopSound)())
		{
			m_bSound = FALSE;
			m_ctrlBtnSound.SetIcon(IDI_SOUND_DISABLE);
		}
	}
	else
	{
		if(NAME(PlayM4_PlaySound)(m_lPort))
		{
			m_ctrlBtnSound.SetIcon(IDI_SOUND_ENABLE);
			m_bSound = TRUE;
		}
	}
}

// adjust sound
void  CPlayerDlg::AdjustSound(BOOL bFlag)
{
	int nSoundPos = m_SoundSlider.GetPos();	

	if(bFlag)
	{
#ifdef _WAVE_ADJ
		nSoundPos += (MAX_WAVE_COEF - MIN_WAVE_COEF)/0xf;
		nSoundPos = min(nSoundPos, MAX_WAVE_COEF);
#else
		nSoundPos += 0xffff/0xf;
		nSoundPos = min(nSoundPos, 0xffff-1);
#endif
	}
	else
	{
#ifdef _WAVE_ADJ
		nSoundPos -= (MAX_WAVE_COEF - MIN_WAVE_COEF)/0xf;
		nSoundPos = max(nSoundPos, MIN_WAVE_COEF);
#else
		nSoundPos -= 0xffff/0xf;
		nSoundPos = max(nSoundPos, 0);
#endif
	}

#ifdef _WAVE_ADJ
		NAME(PlayM4_AdjustWaveAudio)(m_lPort, nSoundPos);
#else
		//NAME(PlayM4_SetVolume)(m_lPort, WORD(nSoundPos));
#endif

	m_SoundSlider.SetPos(nSoundPos);
}
/*************************************************************************/
/*************************************************************************/
/*************************************************************************/
/* button operation over
/*************************************************************************/
/*************************************************************************/
/*************************************************************************/




/*************************************************************************/
/*************************************************************************/
/*************************************************************************/
/* menu operation begin
/*************************************************************************/
/*************************************************************************/
/*************************************************************************/
// file operation:
void CPlayerDlg::Open() 
{
	Close();
	SetState();

	if (m_lPort == -1)
	{
		PlayM4_GetPort(&m_lPort);
	}

	try
	{
		NAME(PlayM4_SetEncChangeMsg)(m_lPort, m_hWnd, WM_ENC_CHANGE);
		NAME(PlayM4_SetEncTypeChangeCallBack(m_lPort, EncChange, (long)this));

		if(m_bStreamType)
		{
			OpenStream();
		}
		else
		{
			OpenFile();
		}
		
		NAME(PlayM4_SetPicQuality)(m_lPort, m_bPicQuality);
		//NAME(PlayM4_SetDeflash)(m_lPort, m_bDeflash);
//		NAME(PlayM4_SetPlayMode)(m_lPort, m_bHighFluid);

		m_bOpen = TRUE;
		
		NAME(PlayM4_GetPictureSize)(m_lPort, &m_nWidth, &m_nHeight);
		//NAME(PlayM4_SetVolume)(m_lPort,WORD(m_SoundSlider.GetPos()));
		m_pDisplayRegion->SetResolution(m_nHeight, m_nWidth);
		m_pMainMenu->ModifyMenu(IDM_SETDISPLAY, MF_BYCOMMAND, IDM_SETDISPLAY, "Multi Display");
		// if video format is HCIF, then double the height
		if( (m_nWidth == WIDTH*2) && (m_nHeight <= HEIGHT_PAL) )
		{
			m_nHeight *= 2;
		}

		InitWindowSize(m_nWidth, m_nHeight);
	}
	catch(...)
	{
		Close();
	}
}


void CPlayerDlg::Close() 
{
	// TODO: Add your command handler code here
	if(m_bOpen)
	{
		if(m_bStreamType)
		{
			CloseStream();
		}
		else
		{
			CloseFile();	
		}

		if(m_pWatermarkDlg)
		{
			m_pWatermarkDlg->DestroyWindow();
		}

		m_nWidth = 352;
		m_nHeight = 288;

		
//		m_HikvisionBmp.LoadBitmap(IDB_HIKVISION);
// 		m_ctrlVideoPic.SetBitmap(m_HikvisionBmp);
// 		m_ctrlVideoPic.ShowWindow(SW_SHOW);
		SetWindowText("Player");
		m_ctrlVideoPic.Invalidate();
	}
}

void CPlayerDlg::CutFile() 
{
	// TODO: Add your command handler code here
	CCutFile cutDlg;
	if(cutDlg.SetFileName(m_strPlayFileName))
	{
		cutDlg.DoModal();
	}
}

void CPlayerDlg::SetSecretKey()
{
	CSetKeyDlg keyDlg(m_lPort);

	keyDlg.DoModal();
}

// view operation:
void CPlayerDlg::ViewFullScreen() 
{
	// TODO: Add your command handler code here
	m_bFullScreen = !m_bFullScreen;
	
	if(m_bFullScreen)
	{
		//Save the pre info;
		GetWindowPlacement(&m_OldWndpl);
		//Remove WS_SIZEBOX windows style. or not the window can't be full-creen.
		ModifyStyle(WS_SIZEBOX, 0, 0);
		
		CRect WindowRect, ClientRect;
		RECT  m_FullScreenRect;
		//ReDraw the window. Get the correct edge value.
//		GetWindowRect(&WindowRect);
//		WindowRect.left  += 1;
//		WindowRect.right += 1;
//		MoveWindow(CRect(0, 0, 352, 288), TRUE);
		
		GetWindowRect(&WindowRect);
		GetClientRect(&ClientRect);
		ClientToScreen(&ClientRect);
		
		//get the dest window rect.
		m_FullScreenRect.left   = WindowRect.left   - ClientRect.left + m_rcScreen.left;
		m_FullScreenRect.top    = WindowRect.top    - ClientRect.top  + m_rcScreen.top;
		m_FullScreenRect.right  = WindowRect.right  - ClientRect.right + m_rcScreen.right;
		m_FullScreenRect.bottom = WindowRect.bottom - ClientRect.bottom + m_rcScreen.bottom;
		//Move the main window to the dest rect.
		WINDOWPLACEMENT wndpl;
		wndpl.length  = sizeof(WINDOWPLACEMENT);
		wndpl.flags   = 0;
		wndpl.showCmd = SW_SHOWNORMAL;
		wndpl.rcNormalPosition = m_FullScreenRect;
		SetWindowPlacement(&wndpl);
		//Move the view winow to full-screen.
		RECT rc;
		GetClientRect(&rc);
		m_ctrlVideoPic.MoveWindow(&rc,TRUE);
	
		//Remove WS_VISIBLE window style.
		m_ctrlBtnPlay.ModifyStyle(WS_VISIBLE, 0, 0);
		m_ctrlBtnPause.ModifyStyle(WS_VISIBLE, 0, 0);
		m_ctrlBtnStop.ModifyStyle(WS_VISIBLE, 0, 0);
		m_ctrlBtnFast.ModifyStyle(WS_VISIBLE, 0, 0);
		m_ctrlBtnGEnd.ModifyStyle(WS_VISIBLE, 0, 0);
		m_ctrlBtnSlow.ModifyStyle(WS_VISIBLE, 0, 0);
		m_ctrlBtnGStart.ModifyStyle(WS_VISIBLE, 0, 0);
		m_ctrlStepBackward.ModifyStyle(WS_VISIBLE, 0, 0);
		m_ctrlStepForward.ModifyStyle(WS_VISIBLE, 0, 0);
		m_ctrlBtnCapPic.ModifyStyle(WS_VISIBLE, 0, 0);
		m_ctrlBtnSound.ModifyStyle(WS_VISIBLE, 0, 0);

		GetDlgItem(IDC_INTER1)->ModifyStyle(WS_VISIBLE, 0, 0);
		GetDlgItem(IDC_INTER2)->ModifyStyle(WS_VISIBLE, 0, 0);
		GetDlgItem(IDC_INTER3)->ModifyStyle(WS_VISIBLE, 0, 0);

		m_SoundSlider.ModifyStyle(WS_VISIBLE,0,0);
		m_PlaySlider.ModifyStyle(WS_VISIBLE,0,0);

		m_ctrlPlayText.ModifyStyle(WS_VISIBLE,0,0);

	}
	else
	{
		//Visible the control.
		m_ctrlBtnPlay.ModifyStyle(0, WS_VISIBLE, 0);
		m_ctrlBtnPause.ModifyStyle(0, WS_VISIBLE, 0);
		m_ctrlBtnStop.ModifyStyle(0, WS_VISIBLE, 0);
		m_ctrlBtnFast.ModifyStyle(0, WS_VISIBLE, 0);
		m_ctrlBtnGEnd.ModifyStyle(0, WS_VISIBLE, 0);
		m_ctrlBtnSlow.ModifyStyle(0, WS_VISIBLE, 0);
		m_ctrlBtnGStart.ModifyStyle(0, WS_VISIBLE, 0);
		m_ctrlStepForward.ModifyStyle(0, WS_VISIBLE, 0);
		m_ctrlStepBackward.ModifyStyle(0, WS_VISIBLE, 0);
		m_ctrlBtnCapPic.ModifyStyle(0, WS_VISIBLE, 0);			
		m_ctrlBtnSound.ModifyStyle(0, WS_VISIBLE, 0);

		GetDlgItem(IDC_INTER1)->ModifyStyle(0, WS_VISIBLE, 0);
		GetDlgItem(IDC_INTER2)->ModifyStyle(0, WS_VISIBLE, 0);
		GetDlgItem(IDC_INTER3)->ModifyStyle(0, WS_VISIBLE, 0);

		m_PlaySlider.ModifyStyle(0,WS_VISIBLE,0);
		m_SoundSlider.ModifyStyle(0,WS_VISIBLE,0);

		m_ctrlPlayText.ModifyStyle(0,WS_VISIBLE,0);
		
		//make the window can be resize.
		ModifyStyle(0, WS_SIZEBOX, 0);
		//change the window pos to pre rect.
		SetWindowPlacement(&m_OldWndpl);

	}
	this->RedrawWindow();

	NAME2(PlayM4_RefreshPlay)(m_lPort);
}

void CPlayerDlg::ViewZoom(UINT nID)
{
	if(m_bFullScreen)
	{
		ViewFullScreen();
	}

	for(int i = 0; i < 3; i++)
	{
		m_pMainMenu->CheckMenuItem(IDM_VIEW_ZOOM_50 + i, MF_UNCHECKED);
	}

	int nItem = nID - IDM_VIEW_ZOOM_100;

	switch(nItem)
	{
	case 0:
		InitWindowSize(m_nWidth, m_nHeight);
		break;

	case -1:
		InitWindowSize(m_nWidth >> 1, m_nHeight >> 1);
		break;

	case 1:
		InitWindowSize(m_nWidth << 1, m_nHeight << 1);
		break;

	default:
		break;
	}
	m_pMainMenu->CheckMenuItem(nID, MF_CHECKED);
}

void CPlayerDlg::Infomation() 
{
	// TODO: Add your command handler code here

	CInfo infoDlg;
	infoDlg.DoModal();
}

void CPlayerDlg::SetDisplay() 
{
	// TODO: Add your command handler code here

	RECT RectP;
	RECT RectS;

	if (!m_pDisplayRegion->m_bValid) 
	{	
		if(m_enumState == State_Play)
		{
			NAME(PlayM4_Pause)(m_lPort, TRUE);
			m_enumState = State_Pause;
			SetState();
		}
		
//		m_ctrlBtnCapPic.EnableWindow(FALSE);
//		m_ctrlBtnSound.EnableWindow(FALSE);	
//		m_ctrlBtnSlow.EnableWindow(FALSE);
//		m_ctrlBtnFast.EnableWindow(FALSE);
//		m_ctrlBtnGStart.EnableWindow(FALSE);
//		m_ctrlBtnGEnd.EnableWindow(FALSE);
//		m_ctrlBtnStop.EnableWindow(FALSE);
//		m_ctrlBtnPlay.EnableWindow(FALSE);
//		m_ctrlBtnPause.EnableWindow(FALSE);	
//		m_ctrlStepBackward.EnableWindow(FALSE);
//		m_ctrlStepForward.EnableWindow(FALSE);
// 		m_pMainMenu->EnableMenuItem(2, MF_GRAYED|MF_BYPOSITION|MF_DISABLED);
		m_pMainMenu->ModifyMenu(IDM_SETDISPLAY, MF_BYCOMMAND, IDM_SETDISPLAY, "Cancel Multi Display");
		
		m_pDisplayRegion->Create(IDD_RANGE);
		GetWindowRect(&RectP);
		
		m_pDisplayRegion->GetWindowRect(&RectS);
		
		RectS.right = RectS.right - RectS.left + RectP.right;
		RectS.left = RectP.right;
		RectS.bottom = RectS.bottom - RectS.top + RectP.top;
		RectS.top = RectP.top; 
		if (RectP.right + 10 > (int)m_dwScreenWidth) 
		{
			RectS.left = RectP.left - (RectS.right - RectS.left);
			RectS.right = RectP.left;
		}
		m_pDisplayRegion->MoveWindow(&RectS);
		
		
		m_pDisplayRegion->InitShow();
		ThrowB(IDM_THROW0);          // when step forward one by one, don't throw B frame;
		NAME(PlayM4_OneByOne)(m_lPort);
		m_enumState = State_Step;
		
		Sleep(50);
		m_pDisplayRegion->DrawRectangle();
	}
	else
	{
//		m_ctrlBtnCapPic.EnableWindow(TRUE);
//		m_ctrlBtnSound.EnableWindow(TRUE);	
//		m_ctrlBtnSlow.EnableWindow(TRUE);
//		m_ctrlBtnFast.EnableWindow(TRUE);
//		m_ctrlBtnGStart.EnableWindow(TRUE);
//		m_ctrlBtnGEnd.EnableWindow(TRUE);
//		m_ctrlBtnStop.EnableWindow(TRUE);
//		m_ctrlBtnPlay.EnableWindow(TRUE);
//		m_ctrlBtnPause.EnableWindow(TRUE);	
//		m_ctrlStepBackward.EnableWindow(TRUE);
//		m_ctrlStepForward.EnableWindow(TRUE);
// 		m_pMainMenu->EnableMenuItem(2, MF_ENABLED|MF_BYPOSITION);
		m_pMainMenu->ModifyMenu(IDM_SETDISPLAY, MF_BYCOMMAND, IDM_SETDISPLAY, "Multi Display"); 
		Play();
		SetState();
		m_pDisplayRegion->DestroyWindow();
	}
}

BOOL CPlayerDlg::SetDevice(UINT nID)
{
	BOOL bFunctionOK = FALSE;

#if (WINVER > 0x0400)
	DWORD nVal = NAME(PlayM4_GetDDrawDeviceTotalNums)();
	if(nVal >= 1)
	{
		UINT nDeviceSeq = nID - IDM_DEVICE0;
		for(int i = 0; i < MAX_DISPLAY_DEVICE; i++)
		{
			m_pMainMenu->CheckMenuItem(IDM_DEVICE0 + i, MF_UNCHECKED);
		}

		if(NAME(PlayM4_SetDDrawDevice)(m_lPort, nDeviceSeq + 1))
		{
			bFunctionOK = TRUE;
		}
		
		NAME(PlayM4_SetDDrawDeviceEx)(m_lPort, 1, nDeviceSeq + 1);
		m_pMainMenu->CheckMenuItem(nID, MF_CHECKED);
	}
#endif

	return bFunctionOK;
}

// control operation:
void CPlayerDlg::VideoControl()
{
	// TODO: Add your command handler code here

	m_pVideoControl->Create(IDD_VIDEOCTRL);
	m_pVideoControl->ShowWindow(SW_SHOW);
	m_pMainMenu->EnableMenuItem(IDM_VIDEO_CONTROL, MF_GRAYED | MF_DISABLED);
}

void CPlayerDlg::Repeat() 
{
	// TODO: Add your command handler code here

	m_bRepeatPlay = !m_bRepeatPlay;
	UINT nFlag = m_bRepeatPlay ? MF_CHECKED : MF_UNCHECKED;
	m_pMainMenu->CheckMenuItem(IDM_REPEAT, nFlag);
}

void CPlayerDlg::Locate() 
{
	// TODO: Add your command handler code here

	m_pSeek->Create(IDD_SEEK);
	m_pMainMenu->EnableMenuItem(IDM_SEEK,MF_GRAYED | MF_DISABLED);
}

// option operation:
void CPlayerDlg::StreamType() 
{
	// TODO: Add your command handler code here

	m_bStreamType = !m_bStreamType;
	UINT nFlag = m_bStreamType ? MF_CHECKED : MF_UNCHECKED;
	m_pMainMenu->CheckMenuItem(IDM_STREAM_TYPE, nFlag);
}

void CPlayerDlg::Deflash() 
{
	// TODO: Add your command handler code here

	m_bDeflash = !m_bDeflash;
	UINT nFlag = m_bDeflash ? MF_CHECKED : MF_UNCHECKED;
	m_pMainMenu->CheckMenuItem(IDM_DEFLASH, nFlag);

	//NAME(PlayM4_SetDeflash)(m_lPort, m_bDeflash);
}

void CPlayerDlg::Quality() 
{
	// TODO: Add your command handler code here

	m_bPicQuality = !m_bPicQuality;
	UINT nFlag = m_bPicQuality ? MF_CHECKED : MF_UNCHECKED;
	m_pMainMenu->CheckMenuItem(IDM_QUALITY, nFlag);

	NAME(PlayM4_SetPicQuality)(m_lPort, m_bPicQuality);
}

void CPlayerDlg::HighFluid()
{
	// TODO: Add your command handler code here

	m_bHighFluid = !m_bHighFluid;
	UINT nFlag = m_bHighFluid ? MF_CHECKED : MF_UNCHECKED;
	m_pMainMenu->CheckMenuItem(IDM_PREVIEW50, nFlag);

	//NAME(PlayM4_SetPlayMode)(m_lPort, m_bHighFluid);
}

void CPlayerDlg::ImageSharpenLevel(UINT nID)
{
	UINT nOldID = IDM_SHARPEN_NONE + m_dwImageSharpenLevel;
	m_dwImageSharpenLevel = nID - IDM_SHARPEN_NONE;

	//NAME(PlayM4_SetImageSharpen)(m_lPort, m_dwImageSharpenLevel);
	
	m_pMainMenu->CheckMenuItem(nOldID, MF_UNCHECKED);
	m_pMainMenu->CheckMenuItem(nID, MF_CHECKED);
}

void CPlayerDlg::ThrowB(UINT nID)
{
	// sorin
	/*
	NAME(PlayM4_ThrowBFrameNum)(m_lPort, nID - IDM_THROW0);

	for(int i = IDM_THROW0; i <= IDM_THROW2; i++ )
	{
		m_pMainMenu->CheckMenuItem(i, MF_UNCHECKED);
	}
	m_pMainMenu->CheckMenuItem(nID, MF_CHECKED);
	*/
}

void CPlayerDlg::SetDecodeType(UINT nID)
{
	//NAME(PlayM4_SetDecodeFrameType)(m_lPort, nID - IDM_DECODE_NORMAL);

	for(int i = IDM_DECODE_NORMAL; i <= IDM_DECODE_NONE; i++ )
	{
		m_pMainMenu->CheckMenuItem(i, MF_UNCHECKED);
	}
	m_pMainMenu->CheckMenuItem(nID, MF_CHECKED);
}

void CPlayerDlg::DisplayType(UINT nID)
{
	m_pMainMenu->CheckMenuItem(IDM_DISNORMAL,  MF_UNCHECKED);
	m_pMainMenu->CheckMenuItem(IDM_DISQUARTER, MF_UNCHECKED);
	m_pMainMenu->CheckMenuItem(nID, MF_CHECKED);

	if(nID == IDM_DISNORMAL)
	{
		NAME(PlayM4_SetDisplayType)(m_lPort, DISPLAY_NORMAL);
	}
	else
	{
		NAME(PlayM4_SetDisplayType)(m_lPort, DISPLAY_QUARTER);
	}
}

void CPlayerDlg::SelectTimer(UINT nID)
{
	m_pMainMenu->CheckMenuItem(IDM_TIMER1, MF_UNCHECKED);
	m_pMainMenu->CheckMenuItem(IDM_TIMER2, MF_UNCHECKED);
	m_pMainMenu->CheckMenuItem(nID, MF_CHECKED);

	if(nID == IDM_TIMER1)
	{
		NAME(PlayM4_SetTimerType)(m_lPort, TIMER_1, 0);
	}
	else
	{
		NAME(PlayM4_SetTimerType)(m_lPort, TIMER_2, 0);
	}
}

void CPlayerDlg::ResetBuf() 
{
	// TODO: Add your command handler code here
	// Only test the interface. 
	NAME(PlayM4_ResetBuffer)(m_lPort, BUF_VIDEO_RENDER);
	NAME(PlayM4_ResetBuffer)(m_lPort, BUF_AUDIO_RENDER);
	NAME(PlayM4_ResetBuffer)(m_lPort, BUF_VIDEO_SRC);
	NAME(PlayM4_ResetBuffer)(m_lPort, BUF_AUDIO_SRC);
}

void CPlayerDlg::CapPicType(UINT nID)
{
	m_pMainMenu->CheckMenuItem(IDM_CAP_BMP,  MF_UNCHECKED);
	m_pMainMenu->CheckMenuItem(IDM_CAP_JPEG, MF_UNCHECKED);
	m_pMainMenu->CheckMenuItem(nID, MF_CHECKED);

	if(nID == IDM_CAP_BMP)
	{
		m_nCapPicType = 0;
	}
	else
	{
		m_nCapPicType = 1;
	}
}

void CPlayerDlg::CappicPath() 
{
	char	     szDir[MAX_PATH];
	BROWSEINFO   bi;
	ITEMIDLIST  *pidl;

	bi.hwndOwner	  = this->m_hWnd;
	bi.pidlRoot		  = NULL;
	bi.pszDisplayName = szDir;
	bi.lpszTitle	  = "ÇëÑ¡ÔñÄ¿Â¼";
	bi.ulFlags		  = BIF_RETURNONLYFSDIRS;
	bi.lpfn			  = NULL;
	bi.lParam		  = 0;
	bi.iImage		  = 0;

	pidl = SHBrowseForFolder(&bi);
	if(NULL == pidl)
	{
		return;
	}

	if(!SHGetPathFromIDList(pidl, szDir)) 
	{
		return;
	}

	m_strCapPicPath.Format("%s",szDir);
}

void CPlayerDlg::ConvertToAVI() 
{
	// TODO: Add your command handler code here	
	int ReturnValue;
	CString str, m_csInfo;

	str.Format("Convert  to AVI File will be Failed to Play the MPEG4 File\n\n");
	m_csInfo += str;
	str.Format("The Converted AVI File can't be Larger than 2G!\n\n");
	m_csInfo += str;
	str.Format("Playing the Converted AVI File must Install the Divx!\n\n");
	m_csInfo += str;
	str.Format("if Continued,Click OK Button!\n");
    m_csInfo += str;
	ReturnValue = MessageBox((LPCTSTR)m_csInfo, NULL, MB_YESNO); 
	

	if(ReturnValue == IDYES)
	{
		Close();

		m_bConvertAVI = TRUE;
		str.Format("AVI Movie Files (*.avi)|*.avi||");
	   
		CFileDialog Filedlg(FALSE, "*.avi", "*.avi", OFN_LONGNAMES|OFN_CREATEPROMPT|OFN_OVERWRITEPROMPT, str, this);
	   
		if(Filedlg.DoModal() == IDOK)
		{
			m_strSaveAVIPath = Filedlg.GetPathName();
		  
		    if(BrowseFile(&m_strPlayFileName))
			{
				Open();
			
				if(m_bConvertAVI)
				{
					SetAVIState();
				}
				else
				{
					SetState();
				}
				SortControl();
			}
			else
			{
				m_bConvertAVI = FALSE;
				SetState();
			}
		}
		else
		{
			m_bConvertAVI = FALSE;
			SetState();
		}
	}
}

void CPlayerDlg::AppAbout() 
{
	// TODO: Add your command handler code here
	CAboutDlg AboutDlg;
	AboutDlg.DoModal();
	
}

void CPlayerDlg::AppHelp()
{
	CHelpDlg HelpDlg;
	HelpDlg.DoModal();
}
/*************************************************************************/
/*************************************************************************/
/*************************************************************************/
/* menu operation over
/*************************************************************************/
/*************************************************************************/
/*************************************************************************/



/*************************************************************************/
/*************************************************************************/
/*************************************************************************/
/* when you click the menu item, you come here
/*************************************************************************/
/*************************************************************************/
/*************************************************************************/
void CPlayerDlg::OnMenuItem(UINT nID)
{
	switch(nID)
	{
	case IDM_FILE_OPEN:
		if(BrowseFile(&m_strPlayFileName)) 
		{
			Open();
			SetState();
		}
		break;
		
	case IDM_FILE_CLOSE:
		Close();
		SetState();
		break;
	
/*	case IDM_CUT_FILE:
		CutFile();
		break;
*/
	case IDM_SET_KEY:
		SetSecretKey();
		break;

	case IDM_APP_EXIT:
		SendMessage(WM_CLOSE);
		break;

	case IDM_VIEW_FULLSCREEN:
		ViewFullScreen();
		break;

	case IDM_VIEW_ZOOM_50:
	case IDM_VIEW_ZOOM_100:
	case IDM_VIEW_ZOOM_200:
		ViewZoom(nID);
		break;
	
	case IDM_INFO:
		Infomation();
		break;
	
	case IDM_SETDISPLAY:
		SetDisplay();
		break;
	
	case IDM_DEVICE0:
	case IDM_DEVICE1:
	case IDM_DEVICE2:
	case IDM_DEVICE3:
		SetDevice(nID);
		break;

	case IDM_WATERMARK:
		GetWatermark();
		break;

	case IDM_PLAY_PAUSE:
		if(m_enumState == State_Play)
		{
			Pause();
			SetState();
		}
		else if(m_enumState == State_Pause || m_enumState == State_Stop)
		{
			Play();
			SetState();
		}
		break;

	case IDM_STOP:
		if(m_enumState != State_Close)
		{
			Stop();
			SetState();
		}
		break;

	case IDM_STEPFORWARD:
		if(m_enumState != State_Stop)
		{
			StepForward();
			SetState();
		}
		break;
		
	case IDM_STEPBACKWARD:
		if(m_enumState != State_Stop)
		{
			StepBackward();
			SetState();
		}
		break;

	case IDM_GOTOSTART:
		GotoStart();
		break;

	case IDM_GOTOEND:
		GotoEnd();
		break;

	case IDM_SEEK:
		Locate();
		break;

	case IDM_VIDEO_CONTROL:
		VideoControl();
		break;

	case IDM_REPEAT:
		Repeat();
		break;
	
	case IDM_STREAM_TYPE:
		StreamType();
		break;
	
	case IDM_DEFLASH:
		Deflash();
		break;
		
	case IDM_QUALITY:
		Quality();
		break;
	
	case IDM_PREVIEW50:
		HighFluid();
		break;
		
	case IDM_SHARPEN_NONE:
	case IDM_SHARPEN_LEVEL1:
	case IDM_SHARPEN_LEVEL2:
	case IDM_SHARPEN_LEVEL3:
	case IDM_SHARPEN_LEVEL4:
	case IDM_SHARPEN_LEVEL5:
	case IDM_SHARPEN_LEVEL6:
		ImageSharpenLevel(nID);
		break;

	case IDM_THROW0:
	case IDM_THROW1:                      
	case IDM_THROW2:
		ThrowB(nID);
		break;

	case IDM_DECODE_NORMAL:
	case IDM_DECODE_I:
	case IDM_DECODE_NONE:
		SetDecodeType(nID);
		break;

	case IDM_DISNORMAL:                   
	case IDM_DISQUARTER:
		DisplayType(nID);
		break;

	case IDM_TIMER1:
	case IDM_TIMER2:
		SelectTimer(nID);
		break;

	case IDM_RESET_BUF:
		ResetBuf();
		break;

	case IDM_CAP_BMP:
	case IDM_CAP_JPEG:
		CapPicType(nID);
		break;

	case IDM_CAPPIC_PATH:
		CappicPath();
		break;
		
/*	case IDM_CONVERT:
		ConvertToAVI();
		break;
*/	
	case IDM_APP_HELP:
		AppHelp();
		break;
		
	case IDM_APP_ABOUT:
		AppAbout();
		break;

	default:
		break;
	}
}

void CPlayerDlg::GetWatermark()
{
	// TODO: Add your command handler code here
//	WaterMarkDlg dlg;
//	dlg.DoModal();
	m_pWatermarkDlg->Create(IDD_WATERMARK);
	m_pMainMenu->EnableMenuItem(IDM_WATERMARK, MF_GRAYED);
}

//´íÎóÐÅÏ¢
char* CPlayerDlg::MyErrorToString(DWORD error)
{
	switch(error)
	{
	case 0:
		return "No error.\0";
    case 1:
      return "The parameter that user inputted is invalid.\0";
    case 2:
      return "The order of the function to be called is incorrect.\0";
    case 3:
      return "Failure when creating system multimedia clock.\0";
    case 4:
      return "Failure when decoding video data.\0";
    case 5:
      return "Failure when decoding audio data.\0";
    case 6:
      return "Failure when allocating memory.\0";
    case 7:
      return "File open failed when calling API Hik_PlayM4_OpenFile.\0";
	case 8:
      return "Failure when creating thread or event.\0";
	case 9:
      return "Failure when creating directdraw object.\0";
	case 10:
      return "Failure when creating off-screen surface.\0";
	case 11:
      return "The input source buffer has overflowed when calling API Hik_PlayM4_InputData.\0";
	case 12:
      return "Failure when creating audio device.\0";
	case 13:
      return "Failure when setting audio volume.\0";
	case 14:
      return "The function only supports playing a file.\0";
	case 15:
      return "The function only supports playing a stream.\0";
	case 16:
      return "Neither MMX nor SSE arithmetic is supported by system.\0";
	case 17:
      return "Unknown file header.\0";
	case 18:
      return "The version of video decoder and video encoder is not compatible.\0";
	case 19:
      return "Failure when initializing decoder.\0";
	case 20:
      return "The file data is unknown. No I-frame found.\0";
	case 21:
      return "Failure when initializing multimedia clock.\0";
	case 22:
      return "Failure when blitting overlay.\0";
	case 23:
      return "Failure when updating overlay or offscreen surface.\0";
	case 24:
      return "Open file error, streamtype is multi.\0";
	case 25:
      return "Openfile error, streamtype is video.\0";
	case 26:
      return "JPEG compression error when capturing jpeg file.\0";
	case 27:
      return "Version of this file not supported when extracting h264 video data.\0";
	case 28:
      return "Failure when extracting video data.\0";
   default:
      return "Unrecognized error value.\0";
	}
}


/*************************************************************************/
/*************************************************************************/
/*************************************************************************/
/* when you click the button item, you come here
/*************************************************************************/
/*************************************************************************/
/*************************************************************************/
void CPlayerDlg::OnButtonItem(UINT nID)
{
	switch(nID)
	{
	case IDC_PLAY:
		Play();
		break;

	case IDC_PAUSE:
		Pause();
		break;

	case IDC_STOP:
		Stop();
		break;

	case IDC_GOTOSTART:
		GotoStart();
		break;

	case IDC_SLOW:
		Play();
		Slow();
		break;

	case IDC_FAST:
		Play();
		Fast();
		break;

	case IDC_GOTOEND:
		GotoEnd();
		break;

	case IDC_STEP:
		StepForward();
		break;
		
	case IDC_STEPBACK:
		StepBackward();
		break;
		
	case IDC_CAPPIC:
		Cappic();
		break;
		
	case IDC_SOUND:
		Sound();
		break;
	
	default:
		break;
	}

	SetState();
}

void CPlayerDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	PlayM4_ReleaseDDrawDevice();
	// TODO: Add your message handler code here
	if(m_pQcifTempBuf)
	{
		delete []m_pQcifTempBuf;
		m_pQcifTempBuf = NULL;
	}
}

void CPlayerDlg::WatermarkOk(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	m_pWatermarkDlg->DestroyWindow();
	if(m_enumState == State_Pause || m_enumState == State_Play)
	{
		m_pMainMenu->EnableMenuItem(IDM_WATERMARK, MF_ENABLED);
	}
}


