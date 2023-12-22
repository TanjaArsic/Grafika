
// IND_17549.h : main header file for the IND_17549 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CIND17549App:
// See IND_17549.cpp for the implementation of this class
//

class CIND17549App : public CWinApp
{
public:
	CIND17549App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CIND17549App theApp;
