
// Klk2018(Bagercina).h : main header file for the Klk2018(Bagercina) application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CKlk2018BagercinaApp:
// See Klk2018(Bagercina).cpp for the implementation of this class
//

class CKlk2018BagercinaApp : public CWinApp
{
public:
	CKlk2018BagercinaApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CKlk2018BagercinaApp theApp;
