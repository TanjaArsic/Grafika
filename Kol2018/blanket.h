
// blanket.h : main header file for the blanket application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CblanketApp:
// See blanket.cpp for the implementation of this class
//

class CblanketApp : public CWinApp
{
public:
	CblanketApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CblanketApp theApp;
