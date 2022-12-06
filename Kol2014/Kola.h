
// Kola.h : main header file for the Kola application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CKolaApp:
// See Kola.cpp for the implementation of this class
//

class CKolaApp : public CWinApp
{
public:
	CKolaApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CKolaApp theApp;
