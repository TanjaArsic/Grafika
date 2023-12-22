
// Klk2019(Transformers).h : main header file for the Klk2019(Transformers) application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CKlk2019TransformersApp:
// See Klk2019(Transformers).cpp for the implementation of this class
//

class CKlk2019TransformersApp : public CWinApp
{
public:
	CKlk2019TransformersApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CKlk2019TransformersApp theApp;
