
// Klk2015(Pac-Man).h : main header file for the Klk2015(Pac-Man) application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CKlk2015PacManApp:
// See Klk2015(Pac-Man).cpp for the implementation of this class
//

class CKlk2015PacManApp : public CWinApp
{
public:
	CKlk2015PacManApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CKlk2015PacManApp theApp;
