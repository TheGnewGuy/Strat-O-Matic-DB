
// Strat-O-Matic-SqLiteDoc.cpp : implementation of the CStratOMaticSqLiteDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Strat-O-Matic-SqLite.h"
#endif

#include "Strat-O-Matic-SqLiteDoc.h"
#include "sqlite3.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CStratOMaticSqLiteDoc

IMPLEMENT_DYNCREATE(CStratOMaticSqLiteDoc, CDocument)

BEGIN_MESSAGE_MAP(CStratOMaticSqLiteDoc, CDocument)
	ON_COMMAND(ID_FILE_OPEN, &CStratOMaticSqLiteDoc::OnFileOpen)
END_MESSAGE_MAP()


// CStratOMaticSqLiteDoc construction/destruction

CStratOMaticSqLiteDoc::CStratOMaticSqLiteDoc()
{
	// TODO: add one-time construction code here

	m_DBFileName = _T("");
	m_str_array_logmsgs.Add(_T("Start"));
}

CStratOMaticSqLiteDoc::~CStratOMaticSqLiteDoc()
{
}

BOOL CStratOMaticSqLiteDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CStratOMaticSqLiteDoc serialization

void CStratOMaticSqLiteDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CStratOMaticSqLiteDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CStratOMaticSqLiteDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CStratOMaticSqLiteDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CStratOMaticSqLiteDoc diagnostics

#ifdef _DEBUG
void CStratOMaticSqLiteDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CStratOMaticSqLiteDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CStratOMaticSqLiteDoc commands


void CStratOMaticSqLiteDoc::OnFileOpen()
{
	// TODO: Add your command handler code here
	CFileDialog* myfiledlg;
	LPCTSTR lpszFilter = _T("DataBase Files (*.db)|*.db|All Files (*.*)|*.*||");
	myfiledlg = new CFileDialog(TRUE, _T("*.db"), NULL, NULL, lpszFilter, NULL);
	myfiledlg->m_ofn.lpstrTitle = _T("Load DataBase File");
	myfiledlg->DoModal();
	m_DBFileName = myfiledlg->GetPathName();
	m_str_array_logmsgs.Add(m_DBFileName);

	// The InvalidateRect is to force a window update.
	InvalidateRect(AfxGetMainWnd()->m_hWnd, NULL, FALSE);

	//int temp;
	//temp = AfxMessageBox(m_DBFileName, MB_YESNO | MB_ICONQUESTION, 0);
}
