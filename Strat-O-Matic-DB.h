
// Strat-O-Matic-DB.h : main header file for the Strat-O-Matic-DB application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CStratOMaticDBApp:
// See Strat-O-Matic-DB.cpp for the implementation of this class
//

class CStratOMaticDBApp : public CWinAppEx
{
public:
	CStratOMaticDBApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CStratOMaticDBApp theApp;
