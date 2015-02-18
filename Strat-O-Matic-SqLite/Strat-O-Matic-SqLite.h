
// Strat-O-Matic-SqLite.h : main header file for the Strat-O-Matic-SqLite application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CStratOMaticSqLiteApp:
// See Strat-O-Matic-SqLite.cpp for the implementation of this class
//

class CStratOMaticSqLiteApp : public CWinApp
{
public:
	CStratOMaticSqLiteApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnFileOpen();
};

extern CStratOMaticSqLiteApp theApp;
