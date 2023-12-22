
// Klk(Tehnicki crtez).h : main header file for the Klk(Tehnicki crtez) application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CKlkTehnickicrtezApp:
// See Klk(Tehnicki crtez).cpp for the implementation of this class
//

class CKlkTehnickicrtezApp : public CWinApp
{
public:
	CKlkTehnickicrtezApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CKlkTehnickicrtezApp theApp;
