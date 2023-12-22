
// Klk2014(Clio).h : main header file for the Klk2014(Clio) application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CKlk2014ClioApp:
// See Klk2014(Clio).cpp for the implementation of this class
//

class CKlk2014ClioApp : public CWinApp
{
public:
	CKlk2014ClioApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CKlk2014ClioApp theApp;
