
// Klk2016(Bilijar).h : main header file for the Klk2016(Bilijar) application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CKlk2016BilijarApp:
// See Klk2016(Bilijar).cpp for the implementation of this class
//

class CKlk2016BilijarApp : public CWinApp
{
public:
	CKlk2016BilijarApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CKlk2016BilijarApp theApp;
