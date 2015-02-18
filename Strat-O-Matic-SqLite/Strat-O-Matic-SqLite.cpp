
// Strat-O-Matic-SqLite.cpp : Defines the class behaviors for the application.
//

#include <afxdlgs.h>  // to support common dialogs
#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "Strat-O-Matic-SqLite.h"
#include "MainFrm.h"

#include "Strat-O-Matic-SqLiteDoc.h"
#include "Strat-O-Matic-SqLiteView.h"

// GLOBAL VARIABLES

CString MyFileName = _T("");

// END GLOBAL VARIABLES

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CStratOMaticSqLiteApp

BEGIN_MESSAGE_MAP(CStratOMaticSqLiteApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CStratOMaticSqLiteApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	//ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
	ON_COMMAND(ID_FILE_OPEN, &CStratOMaticSqLiteApp::OnFileOpen)
END_MESSAGE_MAP()


// CStratOMaticSqLiteApp construction

CStratOMaticSqLiteApp::CStratOMaticSqLiteApp()
{
	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("Strat-O-Matic-SqLite.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CStratOMaticSqLiteApp object

CStratOMaticSqLiteApp theApp;


// CStratOMaticSqLiteApp initialization

BOOL CStratOMaticSqLiteApp::InitInstance()
{
	CWinApp::InitInstance();


	EnableTaskbarInteraction(FALSE);

	// AfxInitRichEdit2() is required to use RichEdit control	
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)


	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CStratOMaticSqLiteDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CStratOMaticSqLiteView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);


	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();
	return TRUE;
}

// CStratOMaticSqLiteApp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// App command to run the dialog
void CStratOMaticSqLiteApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CStratOMaticSqLiteApp message handlers

void CStratOMaticSqLiteApp::OnFileOpen()
{
	// TODO: Add your command handler code here
	CFileDialog* myfiledlg;
	//static char BASED_CODE szFilter[] = "DataBase Files (*.db)|*.db|All Files (*.*)|*.*||";
	LPCTSTR lpszFilter = _T("DataBase Files (*.db)|*.db|All Files (*.*)|*.*||");
	myfiledlg = new CFileDialog(TRUE, _T("*.db"), NULL, NULL, lpszFilter, NULL);
	myfiledlg->m_ofn.lpstrTitle = _T("Load DataBase File");
	myfiledlg->DoModal();
	MyFileName = myfiledlg->GetPathName();
	int temp;
	temp = AfxMessageBox(MyFileName, MB_YESNO | MB_ICONQUESTION, 0);
}
