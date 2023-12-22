
// Klk(Mrezni Saobracaj).h : main header file for the Klk(Mrezni Saobracaj) application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CKlkMrezniSaobracajApp:
// See Klk(Mrezni Saobracaj).cpp for the implementation of this class
//

class CKlkMrezniSaobracajApp : public CWinApp
{
public:
	CKlkMrezniSaobracajApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CKlkMrezniSaobracajApp theApp;
