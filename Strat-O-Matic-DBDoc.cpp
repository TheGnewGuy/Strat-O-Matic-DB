
// Strat-O-Matic-DBDoc.cpp : implementation of the CStratOMaticDBDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Strat-O-Matic-DB.h"
#endif

//#include "Strat-O-Matic-DBSet.h"
#include "Batter.h"
#include "Strat-O-Matic-DBDoc.h"
#include "Batter_MULTI_SET.h"
#include "Batter.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CStratOMaticDBDoc

IMPLEMENT_DYNCREATE(CStratOMaticDBDoc, CDocument)

BEGIN_MESSAGE_MAP(CStratOMaticDBDoc, CDocument)
	ON_COMMAND(ID_DB_SELECT, &CStratOMaticDBDoc::OnDbSelect)
	ON_COMMAND(ID_DB_ADD, &CStratOMaticDBDoc::OnDbAdd)
	ON_COMMAND(ID_DB_CONVERT, &CStratOMaticDBDoc::OnDbConvert)
END_MESSAGE_MAP()


// CStratOMaticDBDoc construction/destruction

CStratOMaticDBDoc::CStratOMaticDBDoc()
{
	CString sDriver = _T("MICROSOFT ACCESS DRIVER (*.mdb, *.accdb)");
	CString sDsn;
	CString sFile = _T("C:\\Family\\SOURCE\\C\\Strat-O-Matic-DB\\Baseball.accdb");
	CString SqlString;

	// Build ODBC connection string
	sDsn.Format(_T("ODBC;DRIVER={%s};DSN='';DBQ=%s"), sDriver, sFile);

	TRY
	{
		// Open the database
		m_pDatabase.Open(NULL, false, false, sDsn);
	}
	CATCH(CDBException, e)
	{
			// If a database exception occured, show error msg
			AfxMessageBox("Database Open error: " + e->m_strError);
	}
	END_CATCH;
	TRY
	{
			// Allocate the recordset
		m_pBatter_set = new CBatter(&m_pDatabase);

		// Build the SQL statement
		//SqlString = "SELECT BatterID, FirstName, LastName "
		//	"FROM Batter";
		SqlString = "SELECT * "
			"FROM Batter";

		// Execute the query
		m_pBatter_set->Open(CRecordset::snapshot, SqlString, CRecordset::none);
	}
	CATCH(CDBException, e)
	{
			// If a database exception occured, show error msg
			AfxMessageBox("Database error: " + e->m_strError);
	}
	END_CATCH;
}

CStratOMaticDBDoc::~CStratOMaticDBDoc()
{
	TRY
	{
		if (m_pBatter_set->IsOpen())
		m_pBatter_set->Close();
	}
		CATCH(CDBException, e)
	{
			// If a database exception occured, show error msg
			AfxMessageBox("Database Close error: " + e->m_strError);
	}
	END_CATCH;

	TRY
	{
		if (m_pDatabase.IsOpen())
		m_pDatabase.Close();
	}
		CATCH(CDBException, e)
	{
			// If a database exception occured, show error msg
			AfxMessageBox("Database Close error: " + e->m_strError);
	}
	END_CATCH;
}

BOOL CStratOMaticDBDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CStratOMaticDBDoc serialization

void CStratOMaticDBDoc::Serialize(CArchive& ar)
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
void CStratOMaticDBDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void CStratOMaticDBDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CStratOMaticDBDoc::SetSearchContent(const CString& value)
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

// CStratOMaticDBDoc diagnostics

#ifdef _DEBUG
void CStratOMaticDBDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CStratOMaticDBDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CStratOMaticDBDoc commands


void CStratOMaticDBDoc::OnDbSelect()
{
	// TODO: Add your command handler code here
	CDatabase database;
	CString SqlString;
	CString sCatID, sCategory;
	CString sDriver = _T("MICROSOFT ACCESS DRIVER (*.mdb, *.accdb)");
	//	CString sDriver = "MS Access Database";  // This does not work
	CString sDsn;
	CString sFile = _T("C:\\Family\\SOURCE\\C\\Strat-O-Matic-DB\\Baseball.accdb");
	// You must change above path if it's different
	int iRec = 0;

	// Build ODBC connection string
	sDsn.Format(_T("ODBC;DRIVER={%s};DSN='';DBQ=%s"), sDriver, sFile);
	TRY
	{
		// Open the database
		if (database.Open(NULL, false, false, sDsn))
		{

			// Allocate the recordset
			CBatter rs(&database);

			// Build the SQL statement
			//SqlString = "SELECT BatterID, FirstName, LastName "
			//	"FROM Batter";
			SqlString = "SELECT * "
				"FROM Batter";

			// Execute the query
			rs.Open(CRecordset::snapshot, SqlString, CRecordset::none);

			// Loop through all records and display popup.
			while (!rs.IsEOF())
			{
				AfxMessageBox("FirstName: " + (CString)rs.m_FirstName + " LastName: " + (CString)rs.m_LastName);
				rs.MoveNext();
			}

			// Close the recordset
			rs.Close();
			// Close the database
			database.Close();
		}
	}
		CATCH(CDBException, e)
	{
			// If a database exception occured, show error msg
			AfxMessageBox("Database error: " + e->m_strError);
	}
	END_CATCH;
}


void CStratOMaticDBDoc::OnDbAdd()
{
	// TODO: Add your command handler code here
	SYSTEMTIME lt;

	CDatabase database;
	CString SqlString;
	CString sCatID, sCategory;
	CString sDriver = _T("MICROSOFT ACCESS DRIVER (*.mdb, *.accdb)");
	//	CString sDriver = "MS Access Database";  // This does not work
	CString sDsn;
	CString sFile = _T("C:\\Family\\SOURCE\\C\\Strat-O-Matic-DB\\Baseball.accdb");
	// You must change above path if it's different
	int iRec = 0;

	// Build ODBC connection string
	sDsn.Format(_T("ODBC;DRIVER={%s};DSN='';DBQ=%s"), sDriver, sFile);
	TRY
	{
		// Open the database
		if (database.Open(NULL, false, false, sDsn))
		{

			// Allocate the recordset
			CBatter rs(&database);

			// Build the SQL statement
			SqlString = "SELECT FirstName, LastName "
				"FROM Batter";

			GetLocalTime(&lt);

			// Execute the query
			rs.Open(CRecordset::snapshot, NULL, CRecordset::none);
			rs.AddNew();
			rs.m_FirstName = "Andy";
			rs.m_LastName = "Gnew";
			rs.m_LastUpdateTime = lt;
			rs.Update();
			rs.AddNew();
			rs.m_FirstName = "Jacob";
			rs.m_LastName = "New";
			rs.m_LastUpdateTime = lt;
			rs.Update();
			rs.Close();
			// Close the database
			database.Close();
		}
	}
		CATCH(CDBException, e)
	{
			// If a database exception occured, show error msg
			AfxMessageBox("Database error: " + e->m_strError);
	}
	END_CATCH;
}

void CStratOMaticDBDoc::OnDbConvert()
{
	// TODO: Add your command handler code here
}
