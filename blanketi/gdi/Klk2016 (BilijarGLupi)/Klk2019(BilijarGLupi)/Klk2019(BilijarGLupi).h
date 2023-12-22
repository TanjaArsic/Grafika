
// Klk2019(BilijarGLupi).h : main header file for the Klk2019(BilijarGLupi) application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CKlk2019BilijarGLupiApp:
// See Klk2019(BilijarGLupi).cpp for the implementation of this class
//

class CKlk2019BilijarGLupiApp : public CWinApp
{
public:
	CKlk2019BilijarGLupiApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CKlk2019BilijarGLupiApp theApp;
