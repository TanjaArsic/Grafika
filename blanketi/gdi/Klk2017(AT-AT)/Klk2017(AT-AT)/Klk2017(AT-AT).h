
// Klk2017(AT-AT).h : main header file for the Klk2017(AT-AT) application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CKlk2017ATATApp:
// See Klk2017(AT-AT).cpp for the implementation of this class
//

class CKlk2017ATATApp : public CWinApp
{
public:
	CKlk2017ATATApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CKlk2017ATATApp theApp;
