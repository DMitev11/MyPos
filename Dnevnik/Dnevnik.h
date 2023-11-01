
// Dnevnik.h : main header file for the Dnevnik application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CDnevnikApp:
// See Dnevnik.cpp for the implementation of this class
//

class CDnevnikApp : public CWinAppEx
{
public:
	CDnevnikApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CDnevnikApp theApp;
