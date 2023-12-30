
// Cactus.h : main header file for the Cactus application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CCactusApp:
// See Cactus.cpp for the implementation of this class
//

class CCactusApp : public CWinApp
{
public:
	CCactusApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CCactusApp theApp;
