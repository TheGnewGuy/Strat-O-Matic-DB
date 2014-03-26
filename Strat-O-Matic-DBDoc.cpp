
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
#include "BatterStats.h"
#include "Pitcher.h"
#include "PitcherStats.h"
#include "Teams.h"
#include "Leagues.h"
#include "Conferences.h"
#include "Divisions.h"
#include "Strat-O-Matic-DBDoc.h"
#include "Batter.h"
#include "FileStruct.h"
#include <string>

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
	// TODO: 
	//		Open batter file
	//		Read while Batter adding to Access file
	//		Close Batter file

	CFileFind myFileFind;
	CStringArray arrayFileNames;
	CFile myInFile;
	BYTE count;
	char temp[41];
	CString strTemp;
	CString strTeamName;
	BOOL bWorking;
	CWnd* pmyCWnd;
	int i, sortflag;
	HCURSOR hCursorWait;
	HCURSOR hCursorNormal;

	CString strLeague;
	CString strLeagueName;
	CString strLeagueFile;
	CString strLeagueDir;

	//strLeague = GetLeagues(TRUE);

	//strLeagueName = strLeague.Left(30);
	//if (strncmp(strLeagueName, "All", 3))
	//{
	//	strLeagueFile = strLeague.Right(12);
	//	strLeagueDir = strLeagueFile.Left(8);
	//}
	//else
	//{
		// This is the base directory
		strLeagueDir = "data";
	//}

	hCursorWait = AfxGetApp()->LoadStandardCursor(IDC_WAIT);

	CreateDefaultLeague();

	arrayFileNames.RemoveAll();
	bWorking = myFileFind.FindFile(strLeagueDir + "\\TB*.dat", 0);
	if (bWorking)
	{
		while (bWorking)
		{
			bWorking = myFileFind.FindNextFile();
			arrayFileNames.Add(myFileFind.GetFileName());
		}
		myFileFind.Close();

		// Since the FindNextFile does not return the files in any order
		// we must sort the file names
		sortflag = 1;
		while (sortflag)
		{
			sortflag = 0;
			for (i = 0; i<(arrayFileNames.GetSize() - 1); i++)
			{
				if (arrayFileNames[i].Compare(arrayFileNames[i + 1]) == 1)
				{
					strTemp = arrayFileNames[i];
					arrayFileNames[i] = arrayFileNames[i + 1];
					arrayFileNames[i + 1] = strTemp;
					sortflag = 1;
				}
			}
		}

		for (i = 0; i<arrayFileNames.GetSize(); i++)
		{
			myInFile.Open(strLeagueDir + "\\" + arrayFileNames[i], CFile::modeRead);
			myInFile.Read(&count, sizeof(count));
			myInFile.Read(temp, 40);
			myInFile.Close();
			temp[40] = NULL;
			strTeamName = temp;
			strTeamName = strTeamName + "\t" + arrayFileNames[i];
			hCursorNormal = SetCursor(hCursorWait);
			ShowCursor(TRUE);
			ExportFileToDB(strLeagueDir, strTeamName);
			ShowCursor(FALSE);
			SetCursor(hCursorNormal);
		}
	}
	else
	{
		// No teams found so display message
		pmyCWnd = AfxGetMainWnd();
		pmyCWnd->MessageBox("Could not find any Team files. Please create a team",
			"Teams not found!", MB_OK | MB_ICONEXCLAMATION);
	}
}


void CStratOMaticDBDoc::ExportFileToDB(CString strDir, CString strTeamName)
{
	SYSTEMTIME lt;
	CString SqlString;
	CString lLastName;
	std::string strLastName;
	BatterStruct structBatter;
	PitcherStruct structPitcher;
	CString tmpTeamID;
	CString myTeam;
	CString strDivision;
	//CString strTempTeam;
	CString myShortTeam;
	CString myBallpark;
	CString myYear;
	CString strBatterFirstName;
	CString strPitcherFirstName;
	BYTE count;
	CFile exportBatter;
	CFile exportPitcher;
	CString exportFileName;
//	char exportData[200];
	CString strexportData;
//	float fBattingAverage, fSLG, fOnBasePercentage;
	int i;
	CString strTemp;
	CString myFileName;
	LONG lTeamSection = 74;
	LONG lCountSection = 1;
	LONG lPlayerSection = structBatter.m_RecordSize;
	LONG lPitcherSection = structPitcher.m_RecordSize;
	LONG position;
//	double fIP;
//	double fERA;
//	double fTRG;
	char Bunting[5] = { 'A', 'B', 'C', 'D', 'E' };
	char HitRun[4] = { 'A', 'B', 'C', 'D' };
	char Stealing[7] = { '3', '2', 'A', 'B', 'C', 'D', 'E' };
	char BatterHits[3] = { 'L', 'R', 'S' };
	char Power[2] = { 'N', 'W' };
	int Running[15] = { 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };
	int TRate[21] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
	int Pass[21] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
	int OutArm[12] = { -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5 };
	int CatchArm[10] = { -4, -3, -2, -1, 0, 1, 2, 3, 4, 5 };
	int PHold[16] = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, -1, -2, -3, -4, -5, -6 };

	count = 0;

	// Process Batter file
	strTemp = "XB" + strTeamName.Left(20);
	exportFileName = strDir + "\\" + strTemp + ".txt"; // dir\XB000001.txt
	myFileName = strDir + "\\TB" + strTeamName.Right(10);

	// Allocate the League recordset
	CLeagues rsLeague(&m_pDatabase);
	TRY
	{
		// Execute the query
		rsLeague.Open(CRecordset::snapshot, NULL, CRecordset::none);
	}
		CATCH(CDBException, e)
	{
			// If a database exception occured, show error msg
			AfxMessageBox("Database League RS error: " + e->m_strError);
	}
	END_CATCH;

	// Allocate the Conference recordset
	CConferences rsConference(&m_pDatabase);
	TRY
	{
		// Execute the query
		rsConference.Open(CRecordset::snapshot, NULL, CRecordset::none);
	}
		CATCH(CDBException, e)
	{
			// If a database exception occured, show error msg
			AfxMessageBox("Database Conference RS error: " + e->m_strError);
	}
	END_CATCH;

	// Allocate the Division recordset
	CDivisions rsDivision(&m_pDatabase);
	TRY
	{
		// Execute the query
		rsDivision.Open(CRecordset::snapshot, NULL, CRecordset::none);
	}
		CATCH(CDBException, e)
	{
			// If a database exception occured, show error msg
			AfxMessageBox("Database Division RS error: " + e->m_strError);
	}
	END_CATCH;

	// Allocate the Teams recordset
	CTeams rsTeam(&m_pDatabase);
	TRY
	{
		// Execute the query
		rsTeam.Open(CRecordset::snapshot, NULL, CRecordset::none);
	}
		CATCH(CDBException, e)
	{
			// If a database exception occured, show error msg
			AfxMessageBox("Database Teams RS error: " + e->m_strError);
	}
	END_CATCH;

	// Allocate the Batter recordset
	CBatter rsBatter(&m_pDatabase);
	TRY
	{
		// Execute the query
		rsBatter.Open(CRecordset::snapshot, NULL, CRecordset::none);
	}
		CATCH(CDBException, e)
	{
			// If a database exception occured, show error msg
			AfxMessageBox("Database Batter RS error: " + e->m_strError);
	}
	END_CATCH;

	// Allocate the Batter Statistics recordset
	CBatterStats rsBatterStats(&m_pDatabase);
	TRY
	{
		// Execute the query
		rsBatterStats.Open(CRecordset::snapshot, NULL, CRecordset::none);
	}
		CATCH(CDBException, e)
	{
			// If a database exception occured, show error msg
			AfxMessageBox("Database BatterStats RS error: " + e->m_strError);
	}
	END_CATCH;

	// Process Team entry
	// Skip "1965 " to start with the actual team name.
	//AfxExtractSubString(myTeam, structBatter.GetTeamBatter(myFileName), 1, _T(' '));
	myTeam = structBatter.GetTeamBatter(myFileName).Mid(5).TrimRight(' ');
	myShortTeam = structBatter.GetShortTeamBatter(myFileName);
	myBallpark = structBatter.GetBallparkBatter(myFileName);
	AfxExtractSubString(myYear, structBatter.GetTeamBatter(myFileName), 0, _T(' '));
	
	// Determine if the team exists in DB.
	// Update the filter which is the WHERE portion
	rsTeam.m_strFilter = "[TeamName] = '" + myTeam + "' AND [TeamYear] = '" + myYear + "'";
	// Execute the query
	rsTeam.Requery();
	if (!rsTeam.GetRowsFetched())
	{
		// Team does not exist so add it
		strDivision = "None";
		if (myTeam == "Chicago White Sox" && myYear == "1965")
		{
			rsLeague.m_strFilter = "[LeagueName] = 'Base1965'";
			rsConference.m_strFilter = "[ConferenceName] = 'BaseAL1965'";
			rsDivision.m_strFilter = "[DivisionName] = 'None'";
			strDivision = "None";
		}
		if (myTeam == "Cleveland Indians" && myYear == "1965")
		{
			rsLeague.m_strFilter = "[LeagueName] = 'Base1965'";
			rsConference.m_strFilter = "[ConferenceName] = 'BaseAL1965'";
			rsDivision.m_strFilter = "[DivisionName] = 'None'";
			strDivision = "None";
		}
		if (myTeam == "Minnesota Twins" && myYear == "1965")
		{
			rsLeague.m_strFilter = "[LeagueName] = 'Base1965'";
			rsConference.m_strFilter = "[ConferenceName] = 'BaseAL1965'";
			rsDivision.m_strFilter = "[DivisionName] = 'None'";
			strDivision = "None";
		}
		if (myTeam == "Cincinnati Reds" && myYear == "1965")
		{
			rsLeague.m_strFilter = "[LeagueName] = 'Base1965'";
			rsConference.m_strFilter = "[ConferenceName] = 'BaseNL1965'";
			rsDivision.m_strFilter = "[DivisionName] = 'None'";
			strDivision = "None";
		}
		if (myTeam == "Los Angeles Dodgers" && myYear == "1965")
		{
			rsLeague.m_strFilter = "[LeagueName] = 'Base1965'";
			rsConference.m_strFilter = "[ConferenceName] = 'BaseNL1965'";
			rsDivision.m_strFilter = "[DivisionName] = 'None'";
			strDivision = "None";
		}
		if (myTeam == "San Francisco Giants" && myYear == "1965")
		{
			rsLeague.m_strFilter = "[LeagueName] = 'Base1965'";
			rsConference.m_strFilter = "[ConferenceName] = 'BaseNL1965'";
			rsDivision.m_strFilter = "[DivisionName] = 'None'";
			strDivision = "None";
		}

		if (myTeam == "Baltimore Orioles" && myYear == "1969")
		{
			rsLeague.m_strFilter = "[LeagueName] = 'Base1969'";
			rsConference.m_strFilter = "[ConferenceName] = 'BaseAL1969'";
			rsDivision.m_strFilter = "[DivisionName] = 'BaseALEast1969'";
			strDivision = "BaseALEast1969";
		}
		if (myTeam == "Seattle Pilots" && myYear == "1969")
		{
			rsLeague.m_strFilter = "[LeagueName] = 'Base1969'";
			rsConference.m_strFilter = "[ConferenceName] = 'BaseAL1969'";
			rsDivision.m_strFilter = "[DivisionName] = 'BaseALWest1969'";
			strDivision = "BaseALWest1969";
		}
		if (myTeam == "Washington Senators" && myYear == "1969")
		{
			rsLeague.m_strFilter = "[LeagueName] = 'Base1969'";
			rsConference.m_strFilter = "[ConferenceName] = 'BaseAL1969'";
			rsDivision.m_strFilter = "[DivisionName] = 'BaseALEast1969'";
			strDivision = "BaseALEast1969";
		}
		if (myTeam == "Houston Astros" && myYear == "1969")
		{
			rsLeague.m_strFilter = "[LeagueName] = 'Base1969'";
			rsConference.m_strFilter = "[ConferenceName] = 'BaseNL1969'";
			rsDivision.m_strFilter = "[DivisionName] = 'BaseNLWest1969'";
			strDivision = "BaseNLWest1969";
		}
		if (myTeam == "Pittsburg Pirates" && myYear == "1969")
		{
			rsLeague.m_strFilter = "[LeagueName] = 'Base1969'";
			rsConference.m_strFilter = "[ConferenceName] = 'BaseNL1969'";
			rsDivision.m_strFilter = "[DivisionName] = 'BaseNLEast1969'";
			strDivision = "BaseNLEast1969";
		}
		if (myTeam == "San Diego Padres" && myYear == "1969")
		{
			rsLeague.m_strFilter = "[LeagueName] = 'Base1969'";
			rsConference.m_strFilter = "[ConferenceName] = 'BaseNL1969'";
			rsDivision.m_strFilter = "[DivisionName] = 'BaseNLWest1969'";
			strDivision = "BaseNLWest1969";
		}

		if (myTeam == "Cleveland" && myYear == "1997")
		{
			rsLeague.m_strFilter = "[LeagueName] = 'Base1997'";
			rsConference.m_strFilter = "[ConferenceName] = 'BaseAL1997'";
			rsDivision.m_strFilter = "[DivisionName] = 'BaseALCentral1997'";
			strDivision = "BaseALCentral1997";
		}
		if (myTeam == "Florida" && myYear == "1997")
		{
			rsLeague.m_strFilter = "[LeagueName] = 'Base1997'";
			rsConference.m_strFilter = "[ConferenceName] = 'BaseNL1997'";
			rsDivision.m_strFilter = "[DivisionName] = 'BaseNLEast1997'";
			strDivision = "BaseNLEast1997";
		}
		if (myTeam == "Anaheim" && myYear == "1997")
		{
			rsLeague.m_strFilter = "[LeagueName] = 'Base1997'";
			rsConference.m_strFilter = "[ConferenceName] = 'BaseAL1997'";
			rsDivision.m_strFilter = "[DivisionName] = 'BaseALWest1997'";
			strDivision = "BaseALWest1997";
		}
		if (myTeam == "Baltimore" && myYear == "1997")
		{
			rsLeague.m_strFilter = "[LeagueName] = 'Base1997'";
			rsConference.m_strFilter = "[ConferenceName] = 'BaseAL1997'";
			rsDivision.m_strFilter = "[DivisionName] = 'BaseALEast1997'";
			strDivision = "BaseALEast1997";
		}

		if (myTeam == "Baltland" && myYear == "1997")
		{
			rsLeague.m_strFilter = "[LeagueName] = 'Base Plano 97 in 99'";
			rsConference.m_strFilter = "[ConferenceName] = 'Base Division A'";
			rsDivision.m_strFilter = "[DivisionName] = 'None'";
			strDivision = "None";
		}
		if (myTeam == "Bostago" && myYear == "1997")
		{
			rsLeague.m_strFilter = "[LeagueName] = 'Base Plano 97 in 99'";
			rsConference.m_strFilter = "[ConferenceName] = 'Base Division A'";
			rsDivision.m_strFilter = "[DivisionName] = 'None'";
			strDivision = "None";
		}
		if (myTeam == "Cleonto" && myYear == "1997")
		{
			rsLeague.m_strFilter = "[LeagueName] = 'Base Plano 97 in 99'";
			rsConference.m_strFilter = "[ConferenceName] = 'Base Division A'";
			rsDivision.m_strFilter = "[DivisionName] = 'None'";
			strDivision = "None";
		}
		if (myTeam == "Seaota" && myYear == "1997")
		{
			rsLeague.m_strFilter = "[LeagueName] = 'Base Plano 97 in 99'";
			rsConference.m_strFilter = "[ConferenceName] = 'Base Division A'";
			rsDivision.m_strFilter = "[DivisionName] = 'None'";
			strDivision = "None";
		}
		if (myTeam == "Texaheim" && myYear == "1997")
		{
			rsLeague.m_strFilter = "[LeagueName] = 'Base Plano 97 in 99'";
			rsConference.m_strFilter = "[ConferenceName] = 'Base Division A'";
			rsDivision.m_strFilter = "[DivisionName] = 'None'";
			strDivision = "None";
		}
		if (myTeam == "Yorkcity" && myYear == "1997")
		{
			rsLeague.m_strFilter = "[LeagueName] = 'Base Plano 97 in 99'";
			rsConference.m_strFilter = "[ConferenceName] = 'Base Division A'";
			rsDivision.m_strFilter = "[DivisionName] = 'None'";
			strDivision = "None";
		}
		if (myTeam == "Cinangeles" && myYear == "1997")
		{
			rsLeague.m_strFilter = "[LeagueName] = 'Base Plano 97 in 99'";
			rsConference.m_strFilter = "[ConferenceName] = 'Base Division B'";
			rsDivision.m_strFilter = "[DivisionName] = 'None'";
			strDivision = "None";
		}
		if (myTeam == "Florago" && myYear == "1997")
		{
			rsLeague.m_strFilter = "[LeagueName] = 'Base Plano 97 in 99'";
			rsConference.m_strFilter = "[ConferenceName] = 'Base Division B'";
			rsDivision.m_strFilter = "[DivisionName] = 'None'";
			strDivision = "None";
		}
		if (myTeam == "Montrado" && myYear == "1997")
		{
			rsLeague.m_strFilter = "[LeagueName] = 'Base Plano 97 in 99'";
			rsConference.m_strFilter = "[ConferenceName] = 'Base Division B'";
			rsDivision.m_strFilter = "[DivisionName] = 'None'";
			strDivision = "None";
		}
		if (myTeam == "NewDiego" && myYear == "1997")
		{
			rsLeague.m_strFilter = "[LeagueName] = 'Base Plano 97 in 99'";
			rsConference.m_strFilter = "[ConferenceName] = 'Base Division B'";
			rsDivision.m_strFilter = "[DivisionName] = 'None'";
			strDivision = "None";
		}
		if (myTeam == "Philanta" && myYear == "1997")
		{
			rsLeague.m_strFilter = "[LeagueName] = 'Base Plano 97 in 99'";
			rsConference.m_strFilter = "[ConferenceName] = 'Base Division B'";
			rsDivision.m_strFilter = "[DivisionName] = 'None'";
			strDivision = "None";
		}
		if (myTeam == "SanLouis" && myYear == "1997")
		{
			rsLeague.m_strFilter = "[LeagueName] = 'Base Plano 97 in 99'";
			rsConference.m_strFilter = "[ConferenceName] = 'Base Division B'";
			rsDivision.m_strFilter = "[DivisionName] = 'None'";
			strDivision = "None";
		}

		rsLeague.Requery();
		rsConference.Requery();
		rsDivision.Requery();

		rsTeam.AddNew();

		rsTeam.m_TeamName = myTeam;
		rsTeam.m_TeamNameShort = myShortTeam;
		rsTeam.m_BallparkName = myBallpark;
		rsTeam.m_HomeWins = 0;
		rsTeam.m_HomeLosses = 0;
		rsTeam.m_AwayWins = 0;
		rsTeam.m_AwayLosses = 0;
		rsTeam.m_LeagueID = rsLeague.m_LeagueID;
		rsTeam.m_ConferenceID = rsConference.m_ConferenceID;
		if (strDivision == "None")
			rsTeam.m_DivisionID = 0;
		else
			rsTeam.m_DivisionID = rsDivision.m_DivisionID;
		rsTeam.m_TeamYear = myYear;

		GetLocalTime(&lt);
		rsTeam.m_LastUpdateTime = lt;

		rsTeam.Update();
	}

	// Re-Execute the query
	rsTeam.Requery();
	if (!rsTeam.GetRowsFetched())
	{
		// Team does not exist so there is a problem
		AfxMessageBox("Database Added Team Missing RS error: ");
	}
	
	// A Team was selected so export all of the players
	count = structBatter.GetCountBatter(myFileName);
	for (i = 0; i<count; i++)
	{
		position = lTeamSection + (i*lPlayerSection);
		structBatter.GetBatter(myFileName, position);

		// When the last name is something like O'Tool, the "'" causes a problem
		// with the SQL search. By editing the string to insert a double "'"
		// in the search, the search works correctly.
		lLastName = structBatter.m_PlayerName.Left(structBatter.m_PlayerName.Find(','));
		std::string str1 = lLastName;
		if (str1.find('\'', 0) != std::string::npos)
		{
			std::string str2 = str1.substr(0, str1.find('\'', 0));
			// Insert the double "'" in the string.
			str2 = str2 + '\'' + '\'';
			strLastName = str2 + str1.substr((str1.find('\'', 0) + 1), std::string::npos);
		}
		else
		{
			strLastName = lLastName;
		}

		AfxExtractSubString(myYear, structBatter.GetTeamBatter(myFileName), 0, _T(' '));

		strBatterFirstName = structBatter.m_PlayerName.Right(
			structBatter.m_PlayerName.GetLength() - structBatter.m_PlayerName.Find(", ") - 2).TrimRight(' ');

		// Create search for duplicate record here.

		// Update the filter which is the WHERE portion to find the player
		// based on name and current team.
		tmpTeamID.Format("%d", rsTeam.m_TeamID);
		rsBatter.m_strFilter = "[FirstName] = '" + strBatterFirstName + "'" +
			" AND [LastName] = '" + strLastName.c_str() + "'" +
			" AND [TeamID] = " + tmpTeamID;
		// Execute the query
		rsBatter.Requery();
		// RowSetSize defaults to 1 so 1 or more matched rows will always result in 1
		if (!rsBatter.GetRowsFetched() == 1)
		{
			// Batter does not exist Therefore add this batter

			rsBatter.AddNew();

			rsBatter.m_LastName = lLastName;
			rsBatter.m_FirstName = strBatterFirstName;
			rsBatter.m_Pitcher = structBatter.m_Pitcher;
			rsBatter.m_Catcher = structBatter.m_Catcher;
			rsBatter.m_FirstBase = structBatter.m_FirstBase;
			rsBatter.m_SecondBase = structBatter.m_SecondBase;
			rsBatter.m_ShortStop = structBatter.m_ShortStop;
			rsBatter.m_ThirdBase = structBatter.m_ThirdBase;
			rsBatter.m_LeftField = structBatter.m_LeftField;
			rsBatter.m_CenterField = structBatter.m_CenterField;
			rsBatter.m_RightField = structBatter.m_RightField;
			rsBatter.m_Bunting = structBatter.m_bBunting;
			rsBatter.m_HitRun = structBatter.m_bHitRun;
			rsBatter.m_Running = structBatter.m_bRunning;
			rsBatter.m_Stealing = structBatter.m_bStealing;
			rsBatter.m_CatchArm = structBatter.m_bCatchArm;
			rsBatter.m_OutArm = structBatter.m_bOutArm;
			rsBatter.m_PowerRight = structBatter.m_bPowerR;
			rsBatter.m_PowerLeft = structBatter.m_bPowerL;
			rsBatter.m_Pass = structBatter.m_bPass;
			rsBatter.m_TRate = structBatter.m_bTRate;
			rsBatter.m_ER1 = structBatter.m_bER1;
			rsBatter.m_ER2 = structBatter.m_bER2;
			rsBatter.m_ER3 = structBatter.m_bER3;
			rsBatter.m_ER4 = structBatter.m_bER4;
			rsBatter.m_ER5 = structBatter.m_bER5;
			rsBatter.m_ER6 = structBatter.m_bER6;
			rsBatter.m_ER7 = structBatter.m_bER7;
			rsBatter.m_ER8 = structBatter.m_bER8;
			rsBatter.m_ER9 = structBatter.m_bER9;
			rsBatter.m_OBChanceHomeRun = structBatter.m_OBChanceHomeRun;
			rsBatter.m_OBChanceTriple = structBatter.m_OBChanceTriple;
			rsBatter.m_OBChanceDouble = structBatter.m_OBChanceDouble;
			rsBatter.m_OBChanceSingle = structBatter.m_OBChanceSingle;
			rsBatter.m_OBChanceWalk = structBatter.m_OBChanceWalk;
			rsBatter.m_ChanceDoublePlay = structBatter.m_ChanceDoublePlay;
			rsBatter.m_OBChanceHomeRunRight = structBatter.m_OBChanceHomeRunRight;
			rsBatter.m_OBChanceTripleRight = structBatter.m_OBChanceTripleRight;
			rsBatter.m_OBChanceDoubleRight = structBatter.m_OBChanceDoubleRight;
			rsBatter.m_OBChanceSingleRight = structBatter.m_OBChanceSingleRight;
			rsBatter.m_OBChanceWalkRight = structBatter.m_OBChanceWalkRight;
			rsBatter.m_ChanceDoublePlayRight = structBatter.m_ChanceDoublePlayRight;
			rsBatter.m_OBChanceHomeRunLeft = structBatter.m_OBChanceHomeRunLeft;
			rsBatter.m_OBChanceTripleLeft = structBatter.m_OBChanceTripleLeft;
			rsBatter.m_OBChanceDoubleLeft = structBatter.m_OBChanceDoubleLeft;
			rsBatter.m_OBChanceSingleLeft = structBatter.m_OBChanceSingleLeft;
			rsBatter.m_OBChanceWalkLeft = structBatter.m_OBChanceWalkLeft;
			rsBatter.m_ChanceDoublePlayLeft = structBatter.m_ChanceDoublePlayLeft;
			rsBatter.m_OBChanceBasic = structBatter.m_OBChanceBasic;
			rsBatter.m_OBChanceLeft = structBatter.m_OBChanceLeft;
			rsBatter.m_OBChanceRight = structBatter.m_OBChanceRight;
			rsBatter.m_BatterHits = structBatter.m_bBatterHits;
			rsBatter.m_TeamID = rsTeam.m_TeamID;

			GetLocalTime(&lt);
			rsBatter.m_LastUpdateTime = lt;

			rsBatter.Update();

			// Retrieve the ID of the batter that was just inserted.

			// Update the filter which is the WHERE portion to find the player
			// based on name and current team.
			tmpTeamID.Format("%d", rsTeam.m_TeamID);
			rsBatter.m_strFilter = "[FirstName] = '" + strBatterFirstName + "'" +
				" AND [LastName] = '" + strLastName.c_str() + "'" +
				" AND [TeamID] = " + tmpTeamID ;
			// Execute the query
			rsBatter.Requery();
			// RowSetSize defaults to 1 so 1 or more matched rows will always result in 1
			if (!rsBatter.GetRowsFetched() == 1)
			{
				// Batter does not exist so there is a problem
				AfxMessageBox("Database Requery of batter incorrect RS error: ");
			}

			// Create test to check for duplicate records here

			rsBatterStats.AddNew();

			rsBatterStats.m_AB = structBatter.m_AB;
			rsBatterStats.m_Runs = structBatter.m_Runs;
			rsBatterStats.m_Hits = structBatter.m_Hits;
			rsBatterStats.m_RBI = structBatter.m_RBI;
			rsBatterStats.m_2B = structBatter.m_2B;
			rsBatterStats.m_3B = structBatter.m_3B;
			rsBatterStats.m_HomeRuns = structBatter.m_HomeRuns;
			rsBatterStats.m_Walk = structBatter.m_Walk;
			rsBatterStats.m_StrikeOut = structBatter.m_StrikeOut;
			rsBatterStats.m_ReachedOnError = structBatter.m_ReachedOnError;
			rsBatterStats.m_Sacrifice = structBatter.m_Sacrifice;
			rsBatterStats.m_StolenBase = structBatter.m_StolenBase;
			rsBatterStats.m_CS = structBatter.m_CS;
			rsBatterStats.m_Games = structBatter.m_Games;
			rsBatterStats.m_HBP = structBatter.m_HBP;
			if (structBatter.m_AB == 0)
			{
				rsBatterStats.m_AVG = 0.0f;
				rsBatterStats.m_SLG = 0.0f;
				rsBatterStats.m_OBP = 0.0f;
			}
			else
			{
				rsBatterStats.m_AVG = (float)structBatter.m_Hits / structBatter.m_AB;
				rsBatterStats.m_SLG = (float)((structBatter.m_Hits - (structBatter.m_2B + structBatter.m_3B + structBatter.m_HomeRuns)) + (2 * structBatter.m_2B) + (3 * structBatter.m_3B) + (4 * structBatter.m_HomeRuns)) / (structBatter.m_AB);
				rsBatterStats.m_OBP = (float)(structBatter.m_Hits + structBatter.m_Walk + structBatter.m_ReachedOnError + structBatter.m_Sacrifice + structBatter.m_StolenBase) / (structBatter.m_AB + structBatter.m_Walk + structBatter.m_ReachedOnError + structBatter.m_Sacrifice + structBatter.m_StolenBase);
			}
			// BatterID always points back to initial Batter for fixed statistics
			rsBatterStats.m_BatterID = rsBatter.m_BatterID ;
			// TeamID can point to any team as this connects the statistics
			// that change based on actual play.
			rsBatterStats.m_TeamID = rsTeam.m_TeamID;

			GetLocalTime(&lt);
			rsBatterStats.m_LastUpdateTime = lt;

			rsBatterStats.Update();
		}
		else
		{
			// Batter already exists.
			//AfxMessageBox("Database Batter is already in DB: " + strBatterFirstName + " " + strLastName.c_str());
		}
	}
	rsBatterStats.Close();
	rsBatter.Close();

	// Process Pitcher file
	strTemp = "XP" + strTeamName.Left(20);
	exportFileName = strDir + "\\" + strTemp + ".txt"; // dir\XB000001.txt
	myFileName = strDir + "\\TP" + strTeamName.Right(10);

	// Allocate the Pitcher recordset
	CPitcher rsPitcher(&m_pDatabase);
	TRY
	{
		// Execute the query
		rsPitcher.Open(CRecordset::snapshot, NULL, CRecordset::none);
	}
		CATCH(CDBException, e)
	{
			// If a database exception occured, show error msg
			AfxMessageBox("Database Pitcher RS error: " + e->m_strError);
	}
	END_CATCH;

	// Allocate the Pitcher Statistics recordset
	CPitcherStats rsPitcherStats(&m_pDatabase);
	TRY
	{
		// Execute the query
		rsPitcherStats.Open(CRecordset::snapshot, NULL, CRecordset::none);
	}
		CATCH(CDBException, e)
	{
			// If a database exception occured, show error msg
			AfxMessageBox("Database Pitcher Statistics RS error: " + e->m_strError);
	}
	END_CATCH;

	// A Team was selected so export all of the players
	count = structPitcher.GetCountPitcher(myFileName);
	for (i = 0; i<count; i++)
	{
		position = lCountSection + (i*lPitcherSection);
		structPitcher.GetPitcher(myFileName, position);

		// When the last name is something like O'Tool, the "'" causes a problem
		// with the SQL search. By editing the string to insert a double "'"
		// in the search, the search works correctly.
		lLastName = structPitcher.m_PitcherName.Left(structPitcher.m_PitcherName.Find(','));
		std::string str1 = lLastName;
		if (str1.find('\'', 0) != std::string::npos)
		{
			std::string str2 = str1.substr(0, str1.find('\'', 0));
			// Insert the double "'" in the string.
			str2 = str2 + '\'' + '\'';
			strLastName = str2 + str1.substr((str1.find('\'', 0) + 1), std::string::npos);
		}
		else
		{
			strLastName = lLastName;
		}

		strPitcherFirstName = structPitcher.m_PitcherName.Right(
			structPitcher.m_PitcherName.GetLength() - structPitcher.m_PitcherName.Find(", ") - 2).TrimRight(' ');

		// Create search for duplicate record here.

		// Update the filter which is the WHERE portion to find the player
		// based on name and current team.
		tmpTeamID.Format("%d", rsTeam.m_TeamID);
		rsPitcher.m_strFilter = "[FirstName] = '" + strPitcherFirstName + "'" +
			" AND [LastName] = '" + strLastName.c_str() + "'" +
			" AND [TeamID] = " + tmpTeamID;
		// Execute the query
		rsPitcher.Requery();
		// RowSetSize defaults to 1 so 1 or more matched rows will always result in 1
		if (!rsPitcher.GetRowsFetched() == 1)
		{
			// Batter does not exist Therefore add this batter

			rsPitcher.AddNew();

			rsPitcher.m_FirstName = strPitcherFirstName;
			rsPitcher.m_LastName = lLastName;
			rsPitcher.m_OBChanceHomeRun = structPitcher.m_OBChanceHomeRun;
			rsPitcher.m_OBChanceTriple = structPitcher.m_OBChanceTriple;
			rsPitcher.m_OBChanceDouble = structPitcher.m_OBChanceDouble;
			rsPitcher.m_OBChanceSingle = structPitcher.m_OBChanceSingle;
			rsPitcher.m_OBChanceWalk = structPitcher.m_OBChanceWalk;
			rsPitcher.m_ChanceDoublePlay = structPitcher.m_ChanceDoublePlay;
			rsPitcher.m_OBChanceHomeRunRight = structPitcher.m_OBChanceHomeRunRight;
			rsPitcher.m_OBChanceTripleRight = structPitcher.m_OBChanceTripleRight;
			rsPitcher.m_OBChanceDoubleRight = structPitcher.m_OBChanceDoubleRight;
			rsPitcher.m_OBChanceSingleRight = structPitcher.m_OBChanceSingleRight;
			rsPitcher.m_OBChanceWalkRight = structPitcher.m_OBChanceWalkRight;
			rsPitcher.m_ChanceDoublePlayRight = structPitcher.m_ChanceDoublePlayRight;
			rsPitcher.m_OBChanceHomeRunLeft = structPitcher.m_OBChanceHomeRunLeft;
			rsPitcher.m_OBChanceTripleLeft = structPitcher.m_OBChanceTripleLeft;
			rsPitcher.m_OBChanceDoubleLeft = structPitcher.m_OBChanceDoubleLeft;
			rsPitcher.m_OBChanceSingleLeft = structPitcher.m_OBChanceSingleLeft;
			rsPitcher.m_OBChanceWalkLeft = structPitcher.m_OBChanceWalkLeft;
			rsPitcher.m_ChanceDoublePlayLeft = structPitcher.m_ChanceDoublePlayLeft;
			rsPitcher.m_OBChanceBasic = structPitcher.m_OBChanceBasic;
			rsPitcher.m_OBChanceLeft = structPitcher.m_OBChanceLeft;
			rsPitcher.m_OBChanceRight = structPitcher.m_OBChanceRight;
			rsPitcher.m_Starter = structPitcher.m_Starter;
			rsPitcher.m_Relief = structPitcher.m_Relief;
			rsPitcher.m_Throws = structPitcher.m_Throws;
			rsPitcher.m_Bunting = structPitcher.m_Bunting;
			rsPitcher.m_Hold = structPitcher.m_Hold;
			rsPitcher.m_WP = structPitcher.m_bWP;
			rsPitcher.m_Balk = structPitcher.m_bBalk;
			rsPitcher.m_PitcherField = structPitcher.m_PitcherField;
			rsPitcher.m_ER1 = structPitcher.m_bER1;
			rsPitcher.m_TeamID = rsTeam.m_TeamID;

			GetLocalTime(&lt);
			rsPitcher.m_LastUpdateTime = lt;

			rsPitcher.Update();

			// Retrieve the ID of the pitcher that was just inserted.

			// Update the filter which is the WHERE portion to find the player
			// based on name and current team.
			tmpTeamID.Format("%d", rsTeam.m_TeamID);
			rsPitcher.m_strFilter = "[FirstName] = '" + strPitcherFirstName + "'" +
				" AND [LastName] = '" + strLastName.c_str() + "'" +
				" AND [TeamID] = " + tmpTeamID;
			// Execute the query
			rsPitcher.Requery();
			// RowSetSize defaults to 1 so 1 or more matched rows will always result in 1
			if (!rsPitcher.GetRowsFetched() == 1)
			{
				// Batter does not exist so there is a problem
				AfxMessageBox("Database Requery of pitcher incorrect RS error: ");
			}

			rsPitcherStats.AddNew();

			rsPitcherStats.m_Wins = structPitcher.m_Wins;
			rsPitcherStats.m_Loss = structPitcher.m_Loss;
			rsPitcherStats.m_Saves = structPitcher.m_Saves;
			rsPitcherStats.m_InningsPitched = (float)atof(structPitcher.m_IP);
			rsPitcherStats.m_ER = structPitcher.m_ER;
			rsPitcherStats.m_Hits = structPitcher.m_Hits;
			rsPitcherStats.m_Walks = structPitcher.m_Walks;
			rsPitcherStats.m_Strikeouts = structPitcher.m_Strikeouts;
			rsPitcherStats.m_HomeRuns = structPitcher.m_HomeRuns;
			rsPitcherStats.m_Games = structPitcher.m_Games;
			rsPitcherStats.m_CompleteGames = structPitcher.m_CompGames;
			rsPitcherStats.m_Starts = structPitcher.m_Starts;
			if (atof(structPitcher.m_IP) == 0)
			{
				rsPitcherStats.m_ERA = 0.0f;
				rsPitcherStats.m_WHIP = 0.0f;
			}
			else
			{
				rsPitcherStats.m_ERA = (float)(structPitcher.m_ER * 9) / (float)atof(structPitcher.m_IP);
				rsPitcherStats.m_WHIP = (float)((structPitcher.m_Hits + structPitcher.m_Walks) * 9) / (float)atof(structPitcher.m_IP);
			}

			// PitcherID always points back to initial Batter for fixed statistics
			rsPitcherStats.m_PitcherID = rsPitcher.m_PitcherID;
			// TeamID can point to any team as this connects the statistics
			// that change based on actual play.
			rsPitcherStats.m_TeamID = rsTeam.m_TeamID;

			GetLocalTime(&lt);
			rsPitcherStats.m_LastUpdateTime = lt;

			rsPitcherStats.Update();
		}
		else
		{
			// Pitcher already exists.
			//AfxMessageBox("Database Pitcher is already in DB: " + strPitcherFirstName + " " + strLastName.c_str());
		}
	}

	rsPitcher.Close();
	rsPitcherStats.Close();
	rsTeam.Close();
	rsDivision.Close();
	rsConference.Close();
	rsLeague.Close();
}


bool CStratOMaticDBDoc::CreateDefaultLeague()
{
	SYSTEMTIME lt;

	// Allocate the League recordset
	CLeagues rsLeague(&m_pDatabase);
	TRY
	{
		// Execute the query
		rsLeague.Open(CRecordset::snapshot, NULL, CRecordset::none);
	}
		CATCH(CDBException, e)
	{
			// If a database exception occured, show error msg
			AfxMessageBox("Database League RS error: " + e->m_strError);
	}
	END_CATCH;

	// Allocate the Conference recordset
	CConferences rsConference(&m_pDatabase);
	TRY
	{
		// Execute the query
		rsConference.Open(CRecordset::snapshot, NULL, CRecordset::none);
	}
		CATCH(CDBException, e)
	{
			// If a database exception occured, show error msg
			AfxMessageBox("Database Conference RS error: " + e->m_strError);
	}
	END_CATCH;

	// Allocate the Division recordset
	CDivisions rsDivision(&m_pDatabase);
	TRY
	{
		// Execute the query
		rsDivision.Open(CRecordset::snapshot, NULL, CRecordset::none);
	}
		CATCH(CDBException, e)
	{
			// If a database exception occured, show error msg
			AfxMessageBox("Database Division RS error: " + e->m_strError);
	}
	END_CATCH;

	// Create search for duplicate record here.
	// Update the filter which is the WHERE portion to find the League.
	rsLeague.m_strFilter = "[LeagueName] = 'Base1965'";
	// Execute the query
	rsLeague.Requery();
	// RowSetSize defaults to 1 so 1 or more matched rows will always result in 1
	if (!rsLeague.GetRowsFetched() == 1)
	{
		rsLeague.AddNew();
		rsLeague.m_LeagueName = "Base1965";
		rsLeague.m_NumberOfConferences = 2;
		rsLeague.m_NumberOfDivisions = 0;
		GetLocalTime(&lt);
		rsLeague.m_LastUpdateTime = lt;
		rsLeague.Update();

		// Execute the query to retrieve LeagueID
		rsLeague.Requery();

		rsConference.AddNew();
		rsConference.m_ConferenceName = "BaseAL1965";
		rsConference.m_LeagueID = rsLeague.m_LeagueID;
		GetLocalTime(&lt);
		rsConference.m_LastUpdateTime = lt;
		rsConference.Update();

		rsConference.AddNew();
		rsConference.m_ConferenceName = "BaseNL1965";
		rsConference.m_LeagueID = rsLeague.m_LeagueID;
		GetLocalTime(&lt);
		rsConference.m_LastUpdateTime = lt;
		rsConference.Update();
	}

	// Update the filter which is the WHERE portion to find the League.
	rsLeague.m_strFilter = "[LeagueName] = 'Base1969'";
	// Execute the query
	rsLeague.Requery();
	// RowSetSize defaults to 1 so 1 or more matched rows will always result in 1
	if (!rsLeague.GetRowsFetched() == 1)
	{
		rsLeague.AddNew();
		rsLeague.m_LeagueName = "Base1969";
		rsLeague.m_NumberOfConferences = 2;
		rsLeague.m_NumberOfDivisions = 4;
		GetLocalTime(&lt);
		rsLeague.m_LastUpdateTime = lt;
		rsLeague.Update();

		// Execute the query to retrieve LeagueID
		rsLeague.Requery();

		rsConference.AddNew();
		rsConference.m_ConferenceName = "BaseAL1969";
		rsConference.m_LeagueID = rsLeague.m_LeagueID;
		GetLocalTime(&lt);
		rsConference.m_LastUpdateTime = lt;
		rsConference.Update();

		rsConference.AddNew();
		rsConference.m_ConferenceName = "BaseNL1969";
		rsConference.m_LeagueID = rsLeague.m_LeagueID;
		GetLocalTime(&lt);
		rsConference.m_LastUpdateTime = lt;
		rsConference.Update();

		// Update the filter which is the WHERE portion to find the League.
		rsConference.m_strFilter = "[ConferenceName] = 'BaseAL1969'";
		// Execute the query
		rsConference.Requery();
		// RowSetSize defaults to 1 so 1 or more matched rows will always result in 1
		if (rsConference.GetRowsFetched() == 1)
		{
			rsDivision.AddNew();
			rsDivision.m_DivisionName = "BaseALEast1969";
			rsDivision.m_ConferenceID = rsConference.m_ConferenceID;
			GetLocalTime(&lt);
			rsDivision.m_LastUpdateTime = lt;
			rsDivision.Update();

			rsDivision.AddNew();
			rsDivision.m_DivisionName = "BaseALWest1969";
			rsDivision.m_ConferenceID = rsConference.m_ConferenceID;
			GetLocalTime(&lt);
			rsDivision.m_LastUpdateTime = lt;
			rsDivision.Update();
		}

		// Update the filter which is the WHERE portion to find the League.
		rsConference.m_strFilter = "[ConferenceName] = 'BaseNL1969'";
		// Execute the query
		rsConference.Requery();
		// RowSetSize defaults to 1 so 1 or more matched rows will always result in 1
		if (rsConference.GetRowsFetched() == 1)
		{
			rsDivision.AddNew();
			rsDivision.m_DivisionName = "BaseNLEast1969";
			rsDivision.m_ConferenceID = rsConference.m_ConferenceID;
			GetLocalTime(&lt);
			rsDivision.m_LastUpdateTime = lt;
			rsDivision.Update();

			rsDivision.AddNew();
			rsDivision.m_DivisionName = "BaseNLWest1969";
			rsDivision.m_ConferenceID = rsConference.m_ConferenceID;
			GetLocalTime(&lt);
			rsDivision.m_LastUpdateTime = lt;
			rsDivision.Update();
		}
	}

	// Update the filter which is the WHERE portion to find the League.
	rsLeague.m_strFilter = "[LeagueName] = 'Base1997'";
	// Execute the query
	rsLeague.Requery();
	// RowSetSize defaults to 1 so 1 or more matched rows will always result in 1
	if (!rsLeague.GetRowsFetched() == 1)
	{
		rsLeague.AddNew();
		rsLeague.m_LeagueName = "Base1997";
		rsLeague.m_NumberOfConferences = 2;
		rsLeague.m_NumberOfDivisions = 6;
		GetLocalTime(&lt);
		rsLeague.m_LastUpdateTime = lt;
		rsLeague.Update();

		// Execute the query to retrieve LeagueID
		rsLeague.Requery();

		rsConference.AddNew();
		rsConference.m_ConferenceName = "BaseAL1997";
		rsConference.m_LeagueID = rsLeague.m_LeagueID;
		GetLocalTime(&lt);
		rsConference.m_LastUpdateTime = lt;
		rsConference.Update();

		rsConference.AddNew();
		rsConference.m_ConferenceName = "BaseNL1997";
		rsConference.m_LeagueID = rsLeague.m_LeagueID;
		GetLocalTime(&lt);
		rsConference.m_LastUpdateTime = lt;
		rsConference.Update();

		// Update the filter which is the WHERE portion to find the League.
		rsConference.m_strFilter = "[ConferenceName] = 'BaseAL1997'";
		// Execute the query
		rsConference.Requery();
		// RowSetSize defaults to 1 so 1 or more matched rows will always result in 1
		if (rsConference.GetRowsFetched() == 1)
		{
			rsDivision.AddNew();
			rsDivision.m_DivisionName = "BaseALEast1997";
			rsDivision.m_ConferenceID = rsConference.m_ConferenceID;
			GetLocalTime(&lt);
			rsDivision.m_LastUpdateTime = lt;
			rsDivision.Update();

			rsDivision.AddNew();
			rsDivision.m_DivisionName = "BaseALCentral1997";
			rsDivision.m_ConferenceID = rsConference.m_ConferenceID;
			GetLocalTime(&lt);
			rsDivision.m_LastUpdateTime = lt;
			rsDivision.Update();

			rsDivision.AddNew();
			rsDivision.m_DivisionName = "BaseALWest1997";
			rsDivision.m_ConferenceID = rsConference.m_ConferenceID;
			GetLocalTime(&lt);
			rsDivision.m_LastUpdateTime = lt;
			rsDivision.Update();
		}


		// Update the filter which is the WHERE portion to find the League.
		rsConference.m_strFilter = "[ConferenceName] = 'BaseNL1997'";
		// Execute the query
		rsConference.Requery();
		// RowSetSize defaults to 1 so 1 or more matched rows will always result in 1
		if (rsConference.GetRowsFetched() == 1)
		{
			rsDivision.AddNew();
			rsDivision.m_DivisionName = "BaseNLEast1997";
			rsDivision.m_ConferenceID = rsConference.m_ConferenceID;
			GetLocalTime(&lt);
			rsDivision.m_LastUpdateTime = lt;
			rsDivision.Update();

			rsDivision.AddNew();
			rsDivision.m_DivisionName = "BaseNLCentral1997";
			rsDivision.m_ConferenceID = rsConference.m_ConferenceID;
			GetLocalTime(&lt);
			rsDivision.m_LastUpdateTime = lt;
			rsDivision.Update();

			rsDivision.AddNew();
			rsDivision.m_DivisionName = "BaseNLWest1997";
			rsDivision.m_ConferenceID = rsConference.m_ConferenceID;
			GetLocalTime(&lt);
			rsDivision.m_LastUpdateTime = lt;
			rsDivision.Update();
		}
	}

	// Update the filter which is the WHERE portion to find the League.
	rsLeague.m_strFilter = "[LeagueName] = 'Base Plano 97 in 99'";
	// Execute the query
	rsLeague.Requery();
	// RowSetSize defaults to 1 so 1 or more matched rows will always result in 1
	if (!rsLeague.GetRowsFetched() == 1)
	{
		rsLeague.AddNew();
		rsLeague.m_LeagueName = "Base Plano 97 in 99";
		rsLeague.m_NumberOfConferences = 2;
		rsLeague.m_NumberOfDivisions = 0;
		GetLocalTime(&lt);
		rsLeague.m_LastUpdateTime = lt;
		rsLeague.Update();

		// Execute the query to retrieve LeagueID
		rsLeague.Requery();

		rsConference.AddNew();
		rsConference.m_ConferenceName = "Base Division A";
		rsConference.m_LeagueID = rsLeague.m_LeagueID;
		GetLocalTime(&lt);
		rsConference.m_LastUpdateTime = lt;
		rsConference.Update();

		rsConference.AddNew();
		rsConference.m_ConferenceName = "Base Division B";
		rsConference.m_LeagueID = rsLeague.m_LeagueID;
		GetLocalTime(&lt);
		rsConference.m_LastUpdateTime = lt;
		rsConference.Update();
	}

	rsDivision.Close();
	rsConference.Close();
	rsLeague.Close();
	return true;
}
