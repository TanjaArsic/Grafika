
// Klk(Sat).h : main header file for the Klk(Sat) application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CKlkSatApp:
// See Klk(Sat).cpp for the implementation of this class
//

class CKlkSatApp : public CWinApp
{
public:
	CKlkSatApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CKlkSatApp theApp;
