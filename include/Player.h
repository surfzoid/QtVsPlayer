// Player.h : main header file for the PLAYER application
//

#if !defined(AFX_PLAYER_H__B22F676B_7805_4359_8D74_B5A63BA0B5F9__INCLUDED_)
#define AFX_PLAYER_H__B22F676B_7805_4359_8D74_B5A63BA0B5F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define _SCL_SECURE_NO_WARNINGS

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#ifdef _FOR_HIKPLAYM4_DLL_
	#define NAME(x) Hik_##x
	#include "../SDK/PlayM4.h"
#else
	//#define NAME(x) x
	#define NAME2(x) x
	#define NAME(x) (TRACE("%s\n",#x), x)
//TRACE("call %s:%s", __FILE__, __LINE__); x
	#include "../SDK/HikPlayM4 Version (For Compatibility of Previous SDK)/PlayM4.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlayerApp:
// See Player.cpp for the implementation of this class
//

class CPlayerApp : public CWinApp
{
public:
	CPlayerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlayerApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPlayerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLAYER_H__B22F676B_7805_4359_8D74_B5A63BA0B5F9__INCLUDED_)
