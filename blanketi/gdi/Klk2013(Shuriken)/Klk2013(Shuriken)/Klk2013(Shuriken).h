
// Klk2013(Shuriken).h : main header file for the Klk2013(Shuriken) application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CKlk2013ShurikenApp:
// See Klk2013(Shuriken).cpp for the implementation of this class
//

class CKlk2013ShurikenApp : public CWinApp
{
public:
	CKlk2013ShurikenApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CKlk2013ShurikenApp theApp;
