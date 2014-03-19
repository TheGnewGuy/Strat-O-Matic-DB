
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
#include "Strat-O-Matic-DBDoc.h"
#include "Batter_MULTI_SET.h"
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

	// Allocate the recordset
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

	// Allocate the recordset
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

	// A Team was selected so export all of the players
	count = structBatter.GetCountBatter(myFileName);
	for (i = 0; i<count; i++)
	{
		position = lTeamSection + (i*lPlayerSection);
		structBatter.GetBatter(myFileName, position);
		
		rsBatter.AddNew();

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
		rsBatter.m_LastName = lLastName;

		rsBatter.m_FirstName = structBatter.m_PlayerName.Right(
			structBatter.m_PlayerName.GetLength() - structBatter.m_PlayerName.Find(", "));
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
		// Set to 1 until team table upload is created
		rsBatter.m_TeamID = 1;

		GetLocalTime(&lt);
		rsBatter.m_LastUpdateTime = lt;

		rsBatter.Update();

		// Retrieve the ID of the batter that was just inserted.

		// Update the filter which is the WHERE portion
		rsBatter.m_strFilter = "[FirstName] = '" + structBatter.m_PlayerName.Right(
			structBatter.m_PlayerName.GetLength() - structBatter.m_PlayerName.Find(", ")) +
			"' AND [LastName] = '" + strLastName.c_str() + "'";
		// Execute the query
		rsBatter.Requery();

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
		rsBatterStats.m_BatterID = rsBatter.m_BatterID ;
		// Set to 1 until team table upload is created
		rsBatterStats.m_TeamID = 1;

		GetLocalTime(&lt);
		rsBatterStats.m_LastUpdateTime = lt;

		rsBatterStats.Update();

	}
	rsBatterStats.Close();

	rsBatter.Close();

	//// Process Pitcher file
	//strTemp = "XP" + strTeamName.Left(20);
	//exportFileName = strDir + "\\" + strTemp + ".txt"; // dir\XB000001.txt
	//myFileName = strDir + "\\TP" + strTeamName.Right(10);
	//exportPitcher.Open(exportFileName, CFile::modeCreate | CFile::modeWrite);
	//sprintf_s(exportData, "Team Name,Pitcher Name,,IP,ER,Hits,Walks,Strikeouts,");
	//strexportData = exportData;
	//exportPitcher.Write(strexportData, strexportData.GetLength());
	//sprintf_s(exportData, "Homeruns,Hold,Wins,Loss,Saves,Starter,Relief,Throws,");
	//strexportData = exportData;
	//exportPitcher.Write(strexportData, strexportData.GetLength());
	//sprintf_s(exportData, "ChB,ChL,ChR,");
	//strexportData = exportData;
	//exportPitcher.Write(strexportData, strexportData.GetLength());
	//sprintf_s(exportData, "ERA,TRG,");
	//strexportData = exportData;
	//exportPitcher.Write(strexportData, strexportData.GetLength());
	//sprintf_s(exportData, "Starts,");
	//strexportData = exportData;
	//exportPitcher.Write(strexportData, strexportData.GetLength());
	//sprintf_s(exportData, "Games,Completed Games,");
	//strexportData = exportData;
	//exportPitcher.Write(strexportData, strexportData.GetLength());
	//sprintf_s(exportData, "Ch1B,Ch2B,Ch3B,ChHR,ChW,ChDP,");
	//strexportData = exportData;
	//exportPitcher.Write(strexportData, strexportData.GetLength());
	//sprintf_s(exportData, "Ch1BL,Ch2BL,Ch3BL,ChHRL,ChWL,ChDPL,");
	//strexportData = exportData;
	//exportPitcher.Write(strexportData, strexportData.GetLength());
	//sprintf_s(exportData, "Ch1BR,Ch2BR,Ch3BR,ChHRR,ChWR,ChDPR,");
	//strexportData = exportData;
	//exportPitcher.Write(strexportData, strexportData.GetLength());
	//sprintf_s(exportData, "Wild Pitch,Balk,Fielding,ErrorRating,Bunting\n");
	//strexportData = exportData;
	//exportPitcher.Write(strexportData, strexportData.GetLength());

	//// A Team was selected so export all of the players
	//count = structPitcher.GetCountPitcher(myFileName);
	//for (i = 0; i<count; i++)
	//{
	//	position = lCountSection + (i*lPitcherSection);
	//	structPitcher.GetPitcher(myFileName, position);
	//	strexportData.Empty();

	//	fIP = atof(structPitcher.m_IP);

	//	if (fIP == 0)
	//	{
	//		fERA = 0;
	//		fTRG = 0;
	//	}
	//	else
	//	{
	//		fERA = (double)(structPitcher.m_ER * 9) / fIP;
	//		fTRG = (double)((structPitcher.m_Hits + structPitcher.m_Walks) * 9) / fIP;
	//	}

	//	sprintf_s(exportData, "%s,%s,",
	//		strTeamName.Left(40),
	//		structPitcher.m_PitcherName);
	//	strexportData = exportData;
	//	exportPitcher.Write(strexportData, strexportData.GetLength());
	//	sprintf_s(exportData, "%1.2f,%i,%i,%i,%i,%i,%i,%i,%i,",
	//		fIP,
	//		structPitcher.m_ER,
	//		structPitcher.m_Hits,
	//		structPitcher.m_Walks,
	//		structPitcher.m_Strikeouts,
	//		structPitcher.m_HomeRuns,
	//		PHold[structPitcher.m_Hold],
	//		structPitcher.m_Wins,
	//		structPitcher.m_Loss);
	//	strexportData = exportData;
	//	exportPitcher.Write(strexportData, strexportData.GetLength());
	//	sprintf_s(exportData, "%i,%i,%i,%c,%s,%s,%s,",
	//		structPitcher.m_Saves,
	//		structPitcher.m_Starter,
	//		structPitcher.m_Relief,
	//		BatterHits[structPitcher.m_Throws],
	//		structPitcher.m_OBChanceBasic,
	//		structPitcher.m_OBChanceLeft,
	//		structPitcher.m_OBChanceRight);
	//	strexportData = exportData;
	//	exportPitcher.Write(strexportData, strexportData.GetLength());
	//	sprintf_s(exportData, "%1.2f,%1.2f,",
	//		fERA,
	//		fTRG);
	//	strexportData = exportData;
	//	exportPitcher.Write(strexportData, strexportData.GetLength());
	//	sprintf_s(exportData, "%i,",
	//		structPitcher.m_Starts);
	//	strexportData = exportData;
	//	exportPitcher.Write(strexportData, strexportData.GetLength());
	//	sprintf_s(exportData, "%i,%i,",
	//		structPitcher.m_Games, structPitcher.m_CompGames);
	//	strexportData = exportData;
	//	exportPitcher.Write(strexportData, strexportData.GetLength());
	//	sprintf_s(exportData, "%s,%s,%s,%s,%s,%s,",
	//		structPitcher.m_OBChanceSingle,
	//		structPitcher.m_OBChanceDouble,
	//		structPitcher.m_OBChanceTriple,
	//		structPitcher.m_OBChanceHomeRun,
	//		structPitcher.m_OBChanceWalk,
	//		structPitcher.m_ChanceDoublePlay);
	//	strexportData = exportData;
	//	exportPitcher.Write(strexportData, strexportData.GetLength());
	//	sprintf_s(exportData, "%s,%s,%s,%s,%s,%s,",
	//		structPitcher.m_OBChanceSingleLeft,
	//		structPitcher.m_OBChanceDoubleLeft,
	//		structPitcher.m_OBChanceTripleLeft,
	//		structPitcher.m_OBChanceHomeRunLeft,
	//		structPitcher.m_OBChanceWalkLeft,
	//		structPitcher.m_ChanceDoublePlayLeft);
	//	strexportData = exportData;
	//	exportPitcher.Write(strexportData, strexportData.GetLength());
	//	sprintf_s(exportData, "%s,%s,%s,%s,%s,%s,",
	//		structPitcher.m_OBChanceSingleRight,
	//		structPitcher.m_OBChanceDoubleRight,
	//		structPitcher.m_OBChanceTripleRight,
	//		structPitcher.m_OBChanceHomeRunRight,
	//		structPitcher.m_OBChanceWalkRight,
	//		structPitcher.m_ChanceDoublePlayRight);
	//	strexportData = exportData;
	//	exportPitcher.Write(strexportData, strexportData.GetLength());
	//	sprintf_s(exportData, "%i,%i,%i,%i,%c\n",
	//		structPitcher.m_bWP,
	//		structPitcher.m_bBalk,
	//		structPitcher.m_PitcherField,
	//		structPitcher.m_bER1,
	//		Bunting[structPitcher.m_Bunting]);
	//	strexportData = exportData;
	//	exportPitcher.Write(strexportData, strexportData.GetLength());
	//}
	//exportPitcher.Close();
}
