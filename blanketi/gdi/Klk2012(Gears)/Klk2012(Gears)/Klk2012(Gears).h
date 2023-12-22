
// Klk2012(Gears).h : main header file for the Klk2012(Gears) application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CKlk2012GearsApp:
// See Klk2012(Gears).cpp for the implementation of this class
//

class CKlk2012GearsApp : public CWinApp
{
public:
	CKlk2012GearsApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CKlk2012GearsApp theApp;
