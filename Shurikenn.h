
// Shurikenn.h : main header file for the Shurikenn application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CShurikennApp:
// See Shurikenn.cpp for the implementation of this class
//

class CShurikennApp : public CWinApp
{
public:
	CShurikennApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CShurikennApp theApp;
