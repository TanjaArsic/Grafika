
// Klk2018(Bager).h : main header file for the Klk2018(Bager) application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CKlk2018BagerApp:
// See Klk2018(Bager).cpp for the implementation of this class
//

class CKlk2018BagerApp : public CWinApp
{
public:
	CKlk2018BagerApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CKlk2018BagerApp theApp;
