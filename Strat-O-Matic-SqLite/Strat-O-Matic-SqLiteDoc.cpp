
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
#include <stdio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CStratOMaticSqLiteDoc

IMPLEMENT_DYNCREATE(CStratOMaticSqLiteDoc, CDocument)

BEGIN_MESSAGE_MAP(CStratOMaticSqLiteDoc, CDocument)
	ON_COMMAND(ID_FILE_OPEN, &CStratOMaticSqLiteDoc::OnFileOpen)
	ON_COMMAND(ID_SQL_CREATETABLE, &CStratOMaticSqLiteDoc::OnSqlCreateTable)
	ON_COMMAND(ID_SQL_DELETETABLE, &CStratOMaticSqLiteDoc::OnSqlDeleteTable)
	ON_COMMAND(ID_SQL_INSERTTEAM, &CStratOMaticSqLiteDoc::OnSqlInsertTeam)
	ON_COMMAND(ID_SQL_INSERTBATTER, &CStratOMaticSqLiteDoc::OnSqlInsertBatter)
	ON_COMMAND(ID_SQL_INSERTBATTERSTATS, &CStratOMaticSqLiteDoc::OnSqlInsertBatterStats)
END_MESSAGE_MAP()


// CStratOMaticSqLiteDoc construction/destruction

CStratOMaticSqLiteDoc::CStratOMaticSqLiteDoc()
{
	// TODO: add one-time construction code here

	m_DBFileName = _T("");
	m_dbOpen = 99;		// SQLITE_OK is set to '0'
}

CStratOMaticSqLiteDoc::~CStratOMaticSqLiteDoc()
{
	if (m_dbOpen = SQLITE_OK)
	{
		sqlite3_close(m_db);
		m_dbOpen = 99;
	}
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
	CHAR buffer[100];
	CFileDialog* myfiledlg;
	LPCTSTR lpszFilter = _T("DataBase Files (*.db)|*.db|All Files (*.*)|*.*||");
	myfiledlg = new CFileDialog(TRUE, _T("*.db"), NULL, NULL, lpszFilter, NULL);
	myfiledlg->m_ofn.lpstrTitle = _T("Load DataBase File");
	myfiledlg->DoModal();
	m_DBFileName = myfiledlg->GetPathName();
	CStringA ansiString(m_DBFileName);
	AddToLog(m_DBFileName);

	if (m_dbOpen = SQLITE_OK)
	{
		sqlite3_close(m_db);
		m_dbOpen = 99;
	}

	m_dbOpen = sqlite3_open(ansiString, &m_db);
	if (m_dbOpen)
	{
		sprintf_s(buffer, sizeof(buffer), "Can't open database: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	else
	{
		sprintf_s(buffer, sizeof(buffer), "Database opened: %s\n", ansiString);
		AddToLog(buffer);
		//AddToLog(_T("Database opened:"));
		int rc = sqlite3_prepare_v2(m_db, "SELECT SQLITE_VERSION()", -1, &m_stmt, 0);

		if (rc != SQLITE_OK)
		{

			//fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(m_db));
			sprintf_s(buffer, sizeof(buffer), "Failed to fetch data: %s\n", sqlite3_errmsg(m_db));
			AddToLog(buffer);
		}

		rc = sqlite3_step(m_stmt);

		if (rc == SQLITE_ROW)
		{
			//printf("%s  %s\n", sqlite3_column_name(m_stmt, 0), sqlite3_column_text(m_stmt, 0));
			sprintf_s(buffer, sizeof(buffer), "%s  %s\n", sqlite3_column_name(m_stmt, 0), sqlite3_column_text(m_stmt, 0));
			AddToLog(buffer);
		}

		sqlite3_finalize(m_stmt);

		// Display that initial setting for foregin keys is off or "0"
		rc = sqlite3_prepare_v2(m_db, "PRAGMA foreign_keys", -1, &m_stmt, 0);

		if (rc != SQLITE_OK)
		{
			sprintf_s(buffer, sizeof(buffer), "Failed to fetch data: %s\n", sqlite3_errmsg(m_db));
			AddToLog(buffer);
		}

		rc = sqlite3_step(m_stmt);

		if (rc == SQLITE_ROW)
		{
			sprintf_s(buffer, sizeof(buffer), "%s  %s\n", sqlite3_column_name(m_stmt, 0), sqlite3_column_text(m_stmt, 0));
			AddToLog(buffer);
		}

		sqlite3_finalize(m_stmt);

		// Turn on foreign key support
		rc = sqlite3_prepare_v2(m_db, "PRAGMA foreign_keys = ON", -1, &m_stmt, 0);

		if (rc != SQLITE_OK)
		{
			sprintf_s(buffer, sizeof(buffer), "Failed to fetch data: %s\n", sqlite3_errmsg(m_db));
			AddToLog(buffer);
		}

		rc = sqlite3_step(m_stmt);

		if (rc == SQLITE_ROW)
		{
			sprintf_s(buffer, sizeof(buffer), "%s  %s\n", sqlite3_column_name(m_stmt, 0), sqlite3_column_text(m_stmt, 0));
			AddToLog(buffer);
		}

		sqlite3_finalize(m_stmt);

		// Display that the setting for foregin keys is now on or "1"
		rc = sqlite3_prepare_v2(m_db, "PRAGMA foreign_keys", -1, &m_stmt, 0);

		if (rc != SQLITE_OK)
		{
			sprintf_s(buffer, sizeof(buffer), "Failed to fetch data: %s\n", sqlite3_errmsg(m_db));
			AddToLog(buffer);
		}

		rc = sqlite3_step(m_stmt);

		if (rc == SQLITE_ROW)
		{
			sprintf_s(buffer, sizeof(buffer), "%s  %s\n", sqlite3_column_name(m_stmt, 0), sqlite3_column_text(m_stmt, 0));
			AddToLog(buffer);
		}

		sqlite3_finalize(m_stmt);
	}

	//int temp;
	//temp = AfxMessageBox(m_DBFileName, MB_YESNO | MB_ICONQUESTION, 0);
}


void CStratOMaticSqLiteDoc::AddToLog(LPCTSTR msg)
{
	m_str_array_logmsgs.Add(msg);
	// The InvalidateRect is to force a window update.
	InvalidateRect(AfxGetMainWnd()->m_hWnd, NULL, FALSE);
}


void CStratOMaticSqLiteDoc::AddToLog(char* msg)
{
	m_str_array_logmsgs.Add(CString(msg));
	// The InvalidateRect is to force a window update.
	InvalidateRect(AfxGetMainWnd()->m_hWnd, NULL, FALSE);
}


void CStratOMaticSqLiteDoc::OnSqlCreateTable()
{
	// TODO: Add your command handler code here
	int rc;
	CHAR buffer[100];

	/* Create SQL statement */
	char *sqlTeam = "CREATE TABLE TEAM("  \
		"TeamID                INTEGER  NOT NULL PRIMARY KEY AUTOINCREMENT," \
		"TeamName              TEXT     NOT NULL," \
		"TeamNameShort         TEXT     NOT NULL," \
		"BallparkName          TEXT     NOT NULL," \
		"HomeWins              INT      NOT NULL," \
		"HomeLosses            INT      NOT NULL," \
		"AwayWins              INT      NOT NULL," \
		"AwayLosses            INT      NOT NULL," \
		"LeagueID              INTEGER  NOT NULL," \
		"ConferenceID          INTEGER  NOT NULL," \
		"DivisionID            INTEGER  NOT NULL," \
		"TeamYear              TEXT     NOT NULL," \
		"BaseTeam              BOOL     NOT NULL DEFAULT FALSE," \
		"LastUpdateTime        DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP" \
		");";
	rc = sqlite3_prepare_v2(m_db, sqlTeam, -1, &m_stmt, 0);
	if (rc != SQLITE_OK)
	{

		//fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(m_db));
		sprintf_s(buffer, sizeof(buffer), "Failed to fetch data: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	else
	{
		sprintf_s(buffer, sizeof(buffer), "Prepare for Team Create OK: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}

	rc = sqlite3_step(m_stmt);

	if (rc != SQLITE_DONE)
	{
		//printf("%s  %s\n", sqlite3_column_name(m_stmt, 0), sqlite3_column_text(m_stmt, 0));
		sprintf_s(buffer, sizeof(buffer), "Failed to insert item: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	else
	{
		sprintf_s(buffer, sizeof(buffer), "Step for Team Create OK: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}

	sqlite3_finalize(m_stmt);

	char *sqlBatter = "CREATE TABLE BATTER("  \
		"BatterID              INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT," \
		"FirstName             TEXT    NOT NULL," \
		"LastName              TEXT    NOT NULL," \
		"Pitcher               INT     NOT NULL," \
		"Catcher               INT     NOT NULL," \
		"FirstBase             INT     NOT NULL," \
		"SecondBase            INT     NOT NULL," \
		"ShortStop             INT     NOT NULL," \
		"ThirdBast             INT     NOT NULL," \
		"LeftField             INT     NOT NULL," \
		"CenterField           INT     NOT NULL," \
		"RightField            INT     NOT NULL," \
		"Bunting               INT     NOT NULL," \
		"HitRun                INT     NOT NULL," \
		"Running               INT     NOT NULL," \
		"Stealing              INT     NOT NULL," \
		"CatchArm              INT     NOT NULL," \
		"OutArm                INT     NOT NULL," \
		"PowerRight            INT     NOT NULL," \
		"PowerLeft             INT     NOT NULL," \
		"Pass                  INT     NOT NULL," \
		"TRate                 INT     NOT NULL," \
		"ER1                   INT     NOT NULL," \
		"ER2                   INT     NOT NULL," \
		"ER3                   INT     NOT NULL," \
		"ER4                   INT     NOT NULL," \
		"ER5                   INT     NOT NULL," \
		"ER6                   INT     NOT NULL," \
		"ER7                   INT     NOT NULL," \
		"ER8                   INT     NOT NULL," \
		"ER9                   INT     NOT NULL," \
		"BatterHits            INT     NOT NULL," \
		"TeamID                INTEGER NOT NULL," \
		"OBChanceHomeRun       TEXT    NOT NULL," \
		"OBChanceTriple        TEXT    NOT NULL," \
		"OBChanceDouble        TEXT    NOT NULL," \
		"OBChanceSingle        TEXT    NOT NULL," \
		"OBChanceWalk          TEXT    NOT NULL," \
		"ChanceDoublePlay      TEXT    NOT NULL," \
		"OBChanceHomeRunRight  TEXT    NOT NULL," \
		"OBChanceTripleRight   TEXT    NOT NULL," \
		"OBChanceDoubleRight   TEXT    NOT NULL," \
		"OBChanceSingleRight   TEXT    NOT NULL," \
		"OBChanceWalkRight     TEXT    NOT NULL," \
		"ChanceDoublePlayRight TEXT    NOT NULL," \
		"OBChanceHomeRunLeft   TEXT    NOT NULL," \
		"OBChanceTripleLeft    TEXT    NOT NULL," \
		"OBChanceDoubleLeft    TEXT    NOT NULL," \
		"OBChanceSingleLeft    TEXT    NOT NULL," \
		"OBChanceWalkLeft      TEXT    NOT NULL," \
		"ChanceDoublePlayLeft  TEXT    NOT NULL," \
		"OBChanceBasic         TEXT    NOT NULL," \
		"OBChanceLeft          TEXT    NOT NULL," \
		"OBChanceRight         TEXT    NOT NULL," \
		"LastUpdateTime        DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP," \
		"FOREIGN KEY(TeamID) REFERENCES TEAM(TeamID)" \
		");";
	rc = sqlite3_prepare_v2(m_db, sqlBatter, -1, &m_stmt, 0);
	if (rc != SQLITE_OK)
	{

		//fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(m_db));
		sprintf_s(buffer, sizeof(buffer), "Failed to fetch data: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	else
	{
		sprintf_s(buffer, sizeof(buffer), "Prepare for Batter Create OK: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}

	rc = sqlite3_step(m_stmt);

	if (rc != SQLITE_DONE)
	{
		//printf("%s  %s\n", sqlite3_column_name(m_stmt, 0), sqlite3_column_text(m_stmt, 0));
		sprintf_s(buffer, sizeof(buffer), "Failed to insert item: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	else
	{
		sprintf_s(buffer, sizeof(buffer), "Step for Batter Create OK: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}

	sqlite3_finalize(m_stmt);

	char *sqlBatterStats = "CREATE TABLE BATTERSTATS("  \
		"BatterStatsID         INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT," \
		"AB                    INT     NOT NULL," \
		"Runs                  INT     NOT NULL," \
		"Hits                  INT     NOT NULL," \
		"RBI                   INT     NOT NULL," \
		"Doubles               INT     NOT NULL," \
		"Triples               INT     NOT NULL," \
		"HomeRuns              INT     NOT NULL," \
		"Walk                  INT     NOT NULL," \
		"Stirkeout             INT     NOT NULL," \
		"ReachedOnError        INT     NOT NULL," \
		"Sacrifice             INT     NOT NULL," \
		"StolenBase            INT     NOT NULL," \
		"CS                    INT     NOT NULL," \
		"Games                 INT     NOT NULL," \
		"HBP                   INT     NOT NULL," \
		"AVG                   FLOAT   NOT NULL," \
		"SLG                   FLOAT   NOT NULL," \
		"OVP                   FLOAT   NOT NULL," \
		"BatterID              INTEGER NOT NULL," \
		"TeamID                INTEGER NOT NULL," \
		"LastUpdateTime        DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP," \
		"FOREIGN KEY(BatterID) REFERENCES TEAM(BatterID)," \
		"FOREIGN KEY(TeamID)   REFERENCES TEAM(TeamID)" \
		");";
	rc = sqlite3_prepare_v2(m_db, sqlBatterStats, -1, &m_stmt, 0);
	if (rc != SQLITE_OK)
	{

		//fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(m_db));
		sprintf_s(buffer, sizeof(buffer), "Failed to fetch data: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	else
	{
		sprintf_s(buffer, sizeof(buffer), "Prepare for BatterStats Create OK: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}

	rc = sqlite3_step(m_stmt);

	if (rc != SQLITE_DONE)
	{
		//printf("%s  %s\n", sqlite3_column_name(m_stmt, 0), sqlite3_column_text(m_stmt, 0));
		sprintf_s(buffer, sizeof(buffer), "Failed to insert item: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	else
	{
		sprintf_s(buffer, sizeof(buffer), "Step for BatterStats Create OK: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}

	sqlite3_finalize(m_stmt);
}


void CStratOMaticSqLiteDoc::OnSqlDeleteTable()
{
	// TODO: Add your command handler code here
}


void CStratOMaticSqLiteDoc::OnSqlInsertTeam()
{
	// TODO: Add your command handler code here
	int rc;
	CHAR buffer[100];
	char *sqlTeam;

	/* Create SQL statement */
	sqlTeam = "INSERT INTO TEAM("  \
		"TeamName," \
		"TeamNameShort," \
		"BallparkName," \
		"HomeWins," \
		"HomeLosses," \
		"AwayWins," \
		"AwayLosses," \
		"LeagueID," \
		"ConferenceID," \
		"DivisionID," \
		"TeamYear," \
		"BaseTeam" \
		")" \
		"VALUES (" \
		"'Cleveland Indians'," \
		"'CLE'," \
		"'Jacobs Field'," \
		"1," \
		"2," \
		"3," \
		"4," \
		"5," \
		"6," \
		"7," \
		"'1965'," \
		"1" \
		");";
	rc = sqlite3_prepare_v2(m_db, sqlTeam, -1, &m_stmt, 0);
	if (rc != SQLITE_OK)
	{

		//fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(m_db));
		sprintf_s(buffer, sizeof(buffer), "Failed to fetch data: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	else
	{
		sprintf_s(buffer, sizeof(buffer), "Prepare for Team Insert OK: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}

	rc = sqlite3_step(m_stmt);

	if (rc != SQLITE_DONE)
	{
		//printf("%s  %s\n", sqlite3_column_name(m_stmt, 0), sqlite3_column_text(m_stmt, 0));
		sprintf_s(buffer, sizeof(buffer), "Failed to insert item: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	else
	{
		sprintf_s(buffer, sizeof(buffer), "Step for Team Insert OK: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}

	sqlite3_finalize(m_stmt);
	/* Create SQL statement */
	sqlTeam = "INSERT INTO TEAM("  \
		"TeamName," \
		"TeamNameShort," \
		"BallparkName," \
		"HomeWins," \
		"HomeLosses," \
		"AwayWins," \
		"AwayLosses," \
		"LeagueID," \
		"ConferenceID," \
		"DivisionID," \
		"TeamYear," \
		"BaseTeam" \
		")" \
		"VALUES (" \
		"'Detriot Tigers'," \
		"'DET'," \
		"'Safco Park'," \
		"1," \
		"2," \
		"3," \
		"4," \
		"5," \
		"6," \
		"7," \
		"'1965'," \
		"1" \
		");";
	rc = sqlite3_prepare_v2(m_db, sqlTeam, -1, &m_stmt, 0);
	if (rc != SQLITE_OK)
	{

		//fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(m_db));
		sprintf_s(buffer, sizeof(buffer), "Failed to fetch data: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	else
	{
		sprintf_s(buffer, sizeof(buffer), "Prepare for Team Insert OK: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}

	rc = sqlite3_step(m_stmt);

	if (rc != SQLITE_DONE)
	{
		//printf("%s  %s\n", sqlite3_column_name(m_stmt, 0), sqlite3_column_text(m_stmt, 0));
		sprintf_s(buffer, sizeof(buffer), "Failed to insert item: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	else
	{
		sprintf_s(buffer, sizeof(buffer), "Step for Team Insert OK: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}

	sqlite3_finalize(m_stmt);
}


void CStratOMaticSqLiteDoc::OnSqlInsertBatter()
{
	// TODO: Add your command handler code here
	int rc;
	CHAR buffer[100];
	char *sqlBatter;
	char *sqlSelect;
	int myTeamId;


	// Select the TeamId

/*	sqlSelect = "SELECT TeamId from TEAM WHERE TeamName = 'Cleveland Indians' " \
	"and TeamYear = '1965'"; 
	*/
	sqlSelect = "SELECT TeamId from TEAM WHERE TeamName = ?1 " \
		"and TeamYear = ?2";
	rc = sqlite3_prepare_v2(m_db, sqlSelect, strlen(sqlSelect), &m_stmt, 0);

	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Failed to fetch data: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
//	char * newteamname = "Detriot Tigers";
	char * newteamname = "Cleveland Indians";
	// Bind the data to field '1' which is the first '?' in the SELECT statement
	rc = sqlite3_bind_text(m_stmt, 1, newteamname, strlen(newteamname), SQLITE_STATIC);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind teamname: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_text(m_stmt, 2, "1965", 4, SQLITE_STATIC);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind year: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}

	rc = sqlite3_step(m_stmt);

	if (rc == SQLITE_ROW)
	{
		sprintf_s(buffer, sizeof(buffer), "%s  %i\n", sqlite3_column_name(m_stmt, 0), sqlite3_column_int(m_stmt, 0));
		AddToLog(buffer);
		myTeamId = sqlite3_column_int(m_stmt, 0);
	}
	else
	{
		sprintf_s(buffer, sizeof(buffer), "Select returned nothing: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}

	sqlite3_finalize(m_stmt);

	/* Create SQL statement */
	sqlBatter = "INSERT INTO BATTER("  \
		"FirstName," \
		"LastName," \
		"Pitcher," \
		"Catcher," \
		"FirstBase," \
		"SecondBase," \
		"ShortStop," \
		"ThirdBast," \
		"LeftField," \
		"CenterField," \
		"RightField," \
		"Bunting," \
		"HitRun," \
		"Running," \
		"Stealing," \
		"CatchArm," \
		"OutArm," \
		"PowerRight," \
		"PowerLeft," \
		"Pass," \
		"TRate," \
		"ER1," \
		"ER2," \
		"ER3," \
		"ER4," \
		"ER5," \
		"ER6," \
		"ER7," \
		"ER8," \
		"ER9," \
		"BatterHits," \
		"TeamID," \
		"OBChanceHomeRun," \
		"OBChanceTriple," \
		"OBChanceDouble," \
		"OBChanceSingle," \
		"OBChanceWalk," \
		"ChanceDoublePlay," \
		"OBChanceHomeRunRight," \
		"OBChanceTripleRight," \
		"OBChanceDoubleRight," \
		"OBChanceSingleRight," \
		"OBChanceWalkRight," \
		"ChanceDoublePlayRight," \
		"OBChanceHomeRunLeft," \
		"OBChanceTripleLeft," \
		"OBChanceDoubleLeft," \
		"OBChanceSingleLeft," \
		"OBChanceWalkLeft," \
		"ChanceDoublePlayLeft," \
		"OBChanceBasic," \
		"OBChanceLeft," \
		"OBChanceRight" \
		")" \
		"VALUES (" \
		"'Max'," \
		"'Alvis'," \
		"0," \
		"0," \
		"0," \
		"3," \
		"0," \
		"0," \
		"0," \
		"0," \
		"0," \
		"0," \
		"0," \
		"0," \
		"0," \
		"0," \
		"0," \
		"0," \
		"0," \
		"0," \
		"0," \
		"0," \
		"0," \
		"0," \
		"3," \
		"0," \
		"0," \
		"0," \
		"0," \
		"0," \
		"0," \
		"?1," \
		"'0'," \
		"'0'," \
		"'0'," \
		"'0'," \
		"'0'," \
		"'0'," \
		"'0'," \
		"'0'," \
		"'0'," \
		"'0'," \
		"'0'," \
		"'0'," \
		"'0'," \
		"'0'," \
		"'0'," \
		"'0'," \
		"'0'," \
		"'0'," \
		"'30.65'," \
		"'0'," \
		"'0'" \
		");";

	rc = sqlite3_prepare_v2(m_db, sqlBatter, strlen(sqlBatter), &m_stmt, 0);
	if (rc != SQLITE_OK)
	{

		//fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(m_db));
		sprintf_s(buffer, sizeof(buffer), "Failed to fetch data: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	else
	{
		sprintf_s(buffer, sizeof(buffer), "Prepare for Batter Insert OK: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}

	// Bind the data to field '1' which is the first '?' in the INSERT statement
	rc = sqlite3_bind_int(m_stmt, 1, myTeamId);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}

	rc = sqlite3_step(m_stmt);

	if (rc != SQLITE_DONE)
	{
		//printf("%s  %s\n", sqlite3_column_name(m_stmt, 0), sqlite3_column_text(m_stmt, 0));
		sprintf_s(buffer, sizeof(buffer), "Failed to insert item: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	else
	{
		sprintf_s(buffer, sizeof(buffer), "Execute for Batter Insert OK.\n");
		AddToLog(buffer);
	}

	sqlite3_finalize(m_stmt);
}


void CStratOMaticSqLiteDoc::OnSqlInsertBatterStats()
{
	// TODO: Add your command handler code here
}
