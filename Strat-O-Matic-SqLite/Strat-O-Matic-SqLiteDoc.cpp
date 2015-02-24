
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
#include "FileStruct.h"

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
	ON_COMMAND(ID_SQL_DBCONVERT, &CStratOMaticSqLiteDoc::OnSqlDbconvert)
	ON_COMMAND(ID_SQL_INSERTDEFAULTLEAGUE, &CStratOMaticSqLiteDoc::OnSqlInsertDefaultLeague)
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
	char *sqlLeagues = "CREATE TABLE LEAGUES("  \
		"LeagueID              INTEGER  NOT NULL PRIMARY KEY AUTOINCREMENT," \
		"LeagueName            TEXT     NOT NULL," \
		"NumberOfConferences   INT      NOT NULL DEFAULT 0," \
		"NumberOfDivisions     INT      NOT NULL DEFAULT 0," \
		"BaseLeague            BOOL     NOT NULL DEFAULT FALSE," \
		"LastUpdateTime        DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP" \
		");";
	rc = sqlite3_prepare_v2(m_db, sqlLeagues, -1, &m_stmt, 0);
	if (rc != SQLITE_OK)
	{

		//fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(m_db));
		sprintf_s(buffer, sizeof(buffer), "Failed to fetch data: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	else
	{
		sprintf_s(buffer, sizeof(buffer), "Prepare for Leagues OK: %s\n", sqlite3_errmsg(m_db));
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
		sprintf_s(buffer, sizeof(buffer), "Step for Leagues Create OK: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}

	sqlite3_finalize(m_stmt);

	/* Create SQL statement */
	char *sqlConferences = "CREATE TABLE CONFERENCES("  \
		"ConferenceID          INTEGER  NOT NULL PRIMARY KEY AUTOINCREMENT," \
		"ConferenceName        TEXT     NOT NULL," \
		"LeagueID              INTEGER  NOT NULL," \
		"BaseConference        BOOL     NOT NULL DEFAULT FALSE," \
		"LastUpdateTime        DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP," \
		"FOREIGN KEY(LeagueID) REFERENCES LEAGUES(LeagueID)" \
		");";
	rc = sqlite3_prepare_v2(m_db, sqlConferences, -1, &m_stmt, 0);
	if (rc != SQLITE_OK)
	{

		//fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(m_db));
		sprintf_s(buffer, sizeof(buffer), "Failed to fetch data: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	else
	{
		sprintf_s(buffer, sizeof(buffer), "Prepare for Conference OK: %s\n", sqlite3_errmsg(m_db));
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
		sprintf_s(buffer, sizeof(buffer), "Step for Conference Create OK: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}

	sqlite3_finalize(m_stmt);

	/* Create SQL statement */
	char *sqlDivisions = "CREATE TABLE DIVISIONS("  \
		"DivisionID            INTEGER  NOT NULL PRIMARY KEY AUTOINCREMENT," \
		"DivisionName          TEXT     NOT NULL," \
		"LeagueID              INTEGER  NOT NULL," \
		"ConferenceID          INTEGER  NOT NULL," \
		"BaseDivisions         BOOL     NOT NULL DEFAULT FALSE," \
		"LastUpdateTime        DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP," \
		"FOREIGN KEY(LeagueID) REFERENCES LEAGUES(LeagueID)," \
		"FOREIGN KEY(ConferenceID) REFERENCES CONFERENCES(ConferenceID)" \
		");";
	rc = sqlite3_prepare_v2(m_db, sqlDivisions, -1, &m_stmt, 0);
	if (rc != SQLITE_OK)
	{

		//fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(m_db));
		sprintf_s(buffer, sizeof(buffer), "Failed to fetch data: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	else
	{
		sprintf_s(buffer, sizeof(buffer), "Prepare for Division OK: %s\n", sqlite3_errmsg(m_db));
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
		sprintf_s(buffer, sizeof(buffer), "Step for Division Create OK: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}

	sqlite3_finalize(m_stmt);

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
		"TeamYear              INT      NOT NULL," \
		"BaseTeam              BOOL     NOT NULL DEFAULT FALSE," \
		"LastUpdateTime        DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP," \
		"FOREIGN KEY(LeagueID) REFERENCES LEAGUES(LeagueID)" \
		"FOREIGN KEY(ConferenceID) REFERENCES CONFERENCES(ConferenceID)" \
		"FOREIGN KEY(DivisionID) REFERENCES DIVISIONS(DivisionID)" \
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
		"BatterID              INTEGER  NOT NULL PRIMARY KEY AUTOINCREMENT," \
		"FirstName             TEXT     NOT NULL," \
		"LastName              TEXT     NOT NULL," \
		"Pitcher               INT      NOT NULL," \
		"Catcher               INT      NOT NULL," \
		"FirstBase             INT      NOT NULL," \
		"SecondBase            INT      NOT NULL," \
		"ShortStop             INT      NOT NULL," \
		"ThirdBase             INT      NOT NULL," \
		"LeftField             INT      NOT NULL," \
		"CenterField           INT      NOT NULL," \
		"RightField            INT      NOT NULL," \
		"Bunting               INT      NOT NULL," \
		"HitRun                INT      NOT NULL," \
		"Running               INT      NOT NULL," \
		"Stealing              INT      NOT NULL," \
		"CatchArm              INT      NOT NULL," \
		"OutArm                INT      NOT NULL," \
		"PowerRight            INT      NOT NULL," \
		"PowerLeft             INT      NOT NULL," \
		"Pass                  INT      NOT NULL," \
		"TRate                 INT      NOT NULL," \
		"ER1                   INT      NOT NULL," \
		"ER2                   INT      NOT NULL," \
		"ER3                   INT      NOT NULL," \
		"ER4                   INT      NOT NULL," \
		"ER5                   INT      NOT NULL," \
		"ER6                   INT      NOT NULL," \
		"ER7                   INT      NOT NULL," \
		"ER8                   INT      NOT NULL," \
		"ER9                   INT      NOT NULL," \
		"BatterHits            BOOL     NOT NULL," \
		"TeamID                INTEGER  NOT NULL," \
		"OBChanceHomeRun       FLOAT    NOT NULL," \
		"OBChanceTriple        FLOAT    NOT NULL," \
		"OBChanceDouble        FLOAT    NOT NULL," \
		"OBChanceSingle        FLOAT    NOT NULL," \
		"OBChanceWalk          FLOAT    NOT NULL," \
		"ChanceDoublePlay      FLOAT    NOT NULL," \
		"OBChanceHomeRunRight  FLOAT    NOT NULL," \
		"OBChanceTripleRight   FLOAT    NOT NULL," \
		"OBChanceDoubleRight   FLOAT    NOT NULL," \
		"OBChanceSingleRight   FLOAT    NOT NULL," \
		"OBChanceWalkRight     FLOAT    NOT NULL," \
		"ChanceDoublePlayRight FLOAT    NOT NULL," \
		"OBChanceHomeRunLeft   FLOAT    NOT NULL," \
		"OBChanceTripleLeft    FLOAT    NOT NULL," \
		"OBChanceDoubleLeft    FLOAT    NOT NULL," \
		"OBChanceSingleLeft    FLOAT    NOT NULL," \
		"OBChanceWalkLeft      FLOAT    NOT NULL," \
		"ChanceDoublePlayLeft  FLOAT    NOT NULL," \
		"OBChanceBasic         FLOAT    NOT NULL," \
		"OBChanceLeft          FLOAT    NOT NULL," \
		"OBChanceRight         FLOAT    NOT NULL," \
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
		"BatterStatsID         INTEGER  NOT NULL PRIMARY KEY AUTOINCREMENT," \
		"AB                    INT      NOT NULL," \
		"Runs                  INT      NOT NULL," \
		"Hits                  INT      NOT NULL," \
		"RBI                   INT      NOT NULL," \
		"Doubles               INT      NOT NULL," \
		"Triples               INT      NOT NULL," \
		"HomeRuns              INT      NOT NULL," \
		"Walk                  INT      NOT NULL," \
		"Stirkeout             INT      NOT NULL," \
		"ReachedOnError        INT      NOT NULL," \
		"Sacrifice             INT      NOT NULL," \
		"StolenBase            INT      NOT NULL," \
		"CS                    INT      NOT NULL," \
		"Games                 INT      NOT NULL," \
		"HBP                   INT      NOT NULL," \
		"AVG                   FLOAT    NOT NULL," \
		"SLG                   FLOAT    NOT NULL," \
		"OVP                   FLOAT    NOT NULL," \
		"BatterID              INTEGER  NOT NULL," \
		"TeamID                INTEGER  NOT NULL," \
		"LastUpdateTime        DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP," \
		"FOREIGN KEY(BatterID) REFERENCES BATTER(BatterID)," \
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
	int myLeagueID;
	int myConfID;
	int myDivisionID;

	myLeagueID = myConfID = myDivisionID = 0;

	myLeagueID = GetLeagueID("Base1965");
	myConfID = GetConferenceID("BaseAL1965");
	myDivisionID = GetDivisionID("BaseALEast1965");

	if ((myLeagueID != 0) & (myConfID != 0) & (myDivisionID != 0))
	{
		TeamInsert("Cleveland Indians","CLE","Jacobs Field",1,2,1,2,myLeagueID,myConfID,myDivisionID,1965,true);
	}
	myLeagueID = myConfID = myDivisionID = 0;

	myLeagueID = GetLeagueID("Base1965");
	myConfID = GetConferenceID("BaseAL1965");
	myDivisionID = GetDivisionID("BaseALWest1965");

	if ((myLeagueID != 0) & (myConfID != 0) & (myDivisionID != 0))
	{
		TeamInsert("Detriot Tigers", "DET", "Safco Park", 1, 2, 1, 2, myLeagueID, myConfID, myDivisionID, 1965, true);
	}
}


void CStratOMaticSqLiteDoc::OnSqlInsertBatter()
{
	// TODO: Add your command handler code here
	int myTeamID;


	// Select the TeamId
	myTeamID = GetTeamID("Cleveland Indians", 1965);
	BatterInsert("Max", "Alvis", 0, 0, 0,
		0, 0, 3, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		1, 2, 3, 4, 5, 6, 7, 8, 9,
		true, myTeamID, 4.0, 3.0, 2.0, 1.0,
		5.0, 4.1, 9.4, 9.3, 9.2,
		9.1, 9.5, 9.21, 7.4, 7.3,
		7.2, 7.1, 7.5, 7.21, 5.5,
		6.6, 7.7);
}


void CStratOMaticSqLiteDoc::OnSqlInsertBatterStats()
{
	// TODO: Add your command handler code here
	int rc;
	CHAR buffer[100];
	char *sqlBatter;
	char *sqlSelect;
	int myTeamID;
	int myBatterID;

	// Select the TeamId
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
	rc = sqlite3_bind_int(m_stmt, 2, 1965);
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
		myTeamID = sqlite3_column_int(m_stmt, 0);
	}
	else
	{
		sprintf_s(buffer, sizeof(buffer), "Select returned nothing: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}

	sqlite3_finalize(m_stmt);

	// Select the BatterId
	sqlSelect = "SELECT BatterID from BATTER WHERE FirstName = ?1 and LastName = ?2";

	rc = sqlite3_prepare_v2(m_db, sqlSelect, strlen(sqlSelect), &m_stmt, 0);

	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Failed to fetch data: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	char * newfirstname = "Max";
	char * newlastname = "Alvis";
	// Bind the data to field '1' which is the first '?' in the SELECT statement
	rc = sqlite3_bind_text(m_stmt, 1, newfirstname, strlen(newfirstname), SQLITE_STATIC);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind teamname: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_text(m_stmt, 2, newlastname, strlen(newlastname), SQLITE_STATIC);
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
		myBatterID = sqlite3_column_int(m_stmt, 0);
	}
	else
	{
		sprintf_s(buffer, sizeof(buffer), "Select returned nothing: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}

	sqlite3_finalize(m_stmt);

	/* Create SQL statement */
	sqlBatter = "INSERT INTO BATTERSTATS("  \
		"AB," \
		"Runs," \
		"Hits," \
		"RBI," \
		"Doubles," \
		"Triples," \
		"HomeRuns," \
		"Walk," \
		"Stirkeout," \
		"ReachedOnError," \
		"Sacrifice," \
		"StolenBase," \
		"CS," \
		"Games," \
		"HBP," \
		"AVG," \
		"SLG," \
		"OVP," \
		"BatterID," \
		"TeamID" \
		")" \
		"VALUES (" \
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
		"?1," \
		"?2" \
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
	rc = sqlite3_bind_int(m_stmt, 1, myBatterID);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 2, myTeamID);
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


void CStratOMaticSqLiteDoc::OnSqlDbconvert()
{
	// TODO: Add your command handler code here
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

	// This is the base directory
	strLeagueDir = "data";

	hCursorWait = AfxGetApp()->LoadStandardCursor(IDC_WAIT);

	CreateDefaultLeague();

	arrayFileNames.RemoveAll();
	bWorking = myFileFind.FindFile(strLeagueDir + _T("\\TB*.dat"), 0);
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
			myInFile.Open(strLeagueDir + _T("\\") + arrayFileNames[i], CFile::modeRead);
			myInFile.Read(&count, sizeof(count));
			myInFile.Read(temp, 40);
			myInFile.Close();
			temp[40] = NULL;
			strTeamName = temp;
			strTeamName = strTeamName + _T("\t") + arrayFileNames[i];
			hCursorNormal = SetCursor(hCursorWait);
			ShowCursor(TRUE);
			ExportBaseFileToDB(strLeagueDir, strTeamName);
			ShowCursor(FALSE);
			SetCursor(hCursorNormal);
		}
	}
	else
	{
		// No teams found so display message
		pmyCWnd = AfxGetMainWnd();
		pmyCWnd->MessageBox(_T("Could not find any Team files. Please create a team"),
			_T("Teams not found!"), MB_OK | MB_ICONEXCLAMATION);
	}

	// This is the L0000001 directory
	strLeagueDir = "L0000001";

	hCursorWait = AfxGetApp()->LoadStandardCursor(IDC_WAIT);

	arrayFileNames.RemoveAll();
	bWorking = myFileFind.FindFile(strLeagueDir + _T("\\TB*.dat"), 0);
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
			myInFile.Open(strLeagueDir + _T("\\") + arrayFileNames[i], CFile::modeRead);
			myInFile.Read(&count, sizeof(count));
			myInFile.Read(temp, 40);
			myInFile.Close();
			temp[40] = NULL;
			strTeamName = temp;
			strTeamName = strTeamName + _T("\t") + arrayFileNames[i];
			hCursorNormal = SetCursor(hCursorWait);
			ShowCursor(TRUE);
			ExportLeagueFileToDB(strLeagueDir, strTeamName);
			ShowCursor(FALSE);
			SetCursor(hCursorNormal);
		}
	}
	else
	{
		// No teams found so display message
		pmyCWnd = AfxGetMainWnd();
		pmyCWnd->MessageBox(_T("Could not find any Team files. Please create a team"),
			_T("Teams not found!"), MB_OK | MB_ICONEXCLAMATION);
	}

	// This is the L0000002 directory
	strLeagueDir = "L0000002";

	hCursorWait = AfxGetApp()->LoadStandardCursor(IDC_WAIT);

	arrayFileNames.RemoveAll();
	bWorking = myFileFind.FindFile(strLeagueDir + _T("\\TB*.dat"), 0);
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
			myInFile.Open(strLeagueDir + _T("\\") + arrayFileNames[i], CFile::modeRead);
			myInFile.Read(&count, sizeof(count));
			myInFile.Read(temp, 40);
			myInFile.Close();
			temp[40] = NULL;
			strTeamName = temp;
			strTeamName = strTeamName + _T("\t") + arrayFileNames[i];
			hCursorNormal = SetCursor(hCursorWait);
			ShowCursor(TRUE);
			ExportLeagueFileToDB(strLeagueDir, strTeamName);
			ShowCursor(FALSE);
			SetCursor(hCursorNormal);
		}
	}
	else
	{
		// No teams found so display message
		pmyCWnd = AfxGetMainWnd();
		pmyCWnd->MessageBox(_T("Could not find any Team files. Please create a team"),
			_T("Teams not found!"), MB_OK | MB_ICONEXCLAMATION);
	}
}


bool CStratOMaticSqLiteDoc::CreateDefaultLeague()
{
	SYSTEMTIME lt;

	//// Allocate the League recordset
	//CLeagues rsLeague(&m_pDatabase);
	//TRY
	//{
	//	// Execute the query
	//	//rsLeague.Open(CRecordset::dynaset, NULL, CRecordset::none);
	//	rsLeague.Open(CRecordset::dynaset, NULL, CRecordset::none);
	//}
	//	CATCH(CDBException, e)
	//{
	//	// If a database exception occured, show error msg
	//	AfxMessageBox("Database League RS error: " + e->m_strError);
	//}
	//END_CATCH;

	//// Allocate the Conference recordset
	//CConferences rsConference(&m_pDatabase);
	//TRY
	//{
	//	// Execute the query
	//	rsConference.Open(CRecordset::dynaset, NULL, CRecordset::none);
	//}
	//	CATCH(CDBException, e)
	//{
	//	// If a database exception occured, show error msg
	//	AfxMessageBox("Database Conference RS error: " + e->m_strError);
	//}
	//END_CATCH;

	//// Allocate the Division recordset
	//CDivisions rsDivision(&m_pDatabase);
	//TRY
	//{
	//	// Execute the query
	//	rsDivision.Open(CRecordset::dynaset, NULL, CRecordset::none);
	//}
	//	CATCH(CDBException, e)
	//{
	//	// If a database exception occured, show error msg
	//	AfxMessageBox("Database Division RS error: " + e->m_strError);
	//}
	//END_CATCH;

	//// Create search for duplicate record here.
	//// Update the filter which is the WHERE portion to find the League.
	//rsLeague.m_strFilter = "[LeagueName] = 'Base1965'";
	//// Execute the query
	//rsLeague.Requery();
	//// RowSetSize defaults to 1 so 1 or more matched rows will always result in 1
	//if (!rsLeague.CanUpdate())
	//{
	//	AfxMessageBox(_T("Unable to update the League recordset."));
	//}
	//if (!rsLeague.GetRecordCount() == 1)
	//{
	//	rsLeague.AddNew();
	//	rsLeague.m_LeagueName = "Base1965";
	//	rsLeague.m_NumberOfConferences = 2;
	//	rsLeague.m_NumberOfDivisions = 0;
	//	rsLeague.m_BaseLeague = TRUE;
	//	GetLocalTime(&lt);
	//	rsLeague.m_LastUpdateTime = lt;
	//	rsLeague.Update();

	//	// Execute the query to retrieve LeagueID
	//	rsLeague.Requery();

	//	rsConference.AddNew();
	//	rsConference.m_ConferenceName = "BaseAL1965";
	//	rsConference.m_LeagueID = rsLeague.m_LeagueID;
	//	rsConference.m_BaseConference = TRUE;
	//	GetLocalTime(&lt);
	//	rsConference.m_LastUpdateTime = lt;
	//	rsConference.Update();

	//	rsConference.AddNew();
	//	rsConference.m_ConferenceName = "BaseNL1965";
	//	rsConference.m_LeagueID = rsLeague.m_LeagueID;
	//	rsConference.m_BaseConference = TRUE;
	//	GetLocalTime(&lt);
	//	rsConference.m_LastUpdateTime = lt;
	//	rsConference.Update();
	//}

	//// Update the filter which is the WHERE portion to find the League.
	//rsLeague.m_strFilter = "[LeagueName] = 'Base1969'";
	//// Execute the query
	//rsLeague.Requery();
	//// RowSetSize defaults to 1 so 1 or more matched rows will always result in 1
	//if (!rsLeague.GetRecordCount() == 1)
	//{
	//	rsLeague.AddNew();
	//	rsLeague.m_LeagueName = "Base1969";
	//	rsLeague.m_NumberOfConferences = 2;
	//	rsLeague.m_NumberOfDivisions = 4;
	//	rsLeague.m_BaseLeague = TRUE;
	//	GetLocalTime(&lt);
	//	rsLeague.m_LastUpdateTime = lt;
	//	rsLeague.Update();

	//	// Execute the query to retrieve LeagueID
	//	rsLeague.Requery();

	//	rsConference.AddNew();
	//	rsConference.m_ConferenceName = "BaseAL1969";
	//	rsConference.m_LeagueID = rsLeague.m_LeagueID;
	//	rsConference.m_BaseConference = TRUE;
	//	GetLocalTime(&lt);
	//	rsConference.m_LastUpdateTime = lt;
	//	rsConference.Update();

	//	rsConference.AddNew();
	//	rsConference.m_ConferenceName = "BaseNL1969";
	//	rsConference.m_LeagueID = rsLeague.m_LeagueID;
	//	rsConference.m_BaseConference = TRUE;
	//	GetLocalTime(&lt);
	//	rsConference.m_LastUpdateTime = lt;
	//	rsConference.Update();

	//	// Update the filter which is the WHERE portion to find the League.
	//	rsConference.m_strFilter = "[ConferenceName] = 'BaseAL1969'";
	//	// Execute the query
	//	rsConference.Requery();
	//	// RowSetSize defaults to 1 so 1 or more matched rows will always result in 1
	//	if (rsConference.GetRecordCount() == 1)
	//	{
	//		rsDivision.AddNew();
	//		rsDivision.m_DivisionName = "BaseALEast1969";
	//		rsDivision.m_ConferenceID = rsConference.m_ConferenceID;
	//		rsDivision.m_BaseDivision = TRUE;
	//		GetLocalTime(&lt);
	//		rsDivision.m_LastUpdateTime = lt;
	//		rsDivision.Update();

	//		rsDivision.AddNew();
	//		rsDivision.m_DivisionName = "BaseALWest1969";
	//		rsDivision.m_ConferenceID = rsConference.m_ConferenceID;
	//		rsDivision.m_BaseDivision = TRUE;
	//		GetLocalTime(&lt);
	//		rsDivision.m_LastUpdateTime = lt;
	//		rsDivision.Update();
	//	}

	//	// Update the filter which is the WHERE portion to find the League.
	//	rsConference.m_strFilter = "[ConferenceName] = 'BaseNL1969'";
	//	// Execute the query
	//	rsConference.Requery();
	//	// RowSetSize defaults to 1 so 1 or more matched rows will always result in 1
	//	if (rsConference.GetRecordCount() == 1)
	//	{
	//		rsDivision.AddNew();
	//		rsDivision.m_DivisionName = "BaseNLEast1969";
	//		rsDivision.m_ConferenceID = rsConference.m_ConferenceID;
	//		rsDivision.m_BaseDivision = TRUE;
	//		GetLocalTime(&lt);
	//		rsDivision.m_LastUpdateTime = lt;
	//		rsDivision.Update();

	//		rsDivision.AddNew();
	//		rsDivision.m_DivisionName = "BaseNLWest1969";
	//		rsDivision.m_ConferenceID = rsConference.m_ConferenceID;
	//		rsDivision.m_BaseDivision = TRUE;
	//		GetLocalTime(&lt);
	//		rsDivision.m_LastUpdateTime = lt;
	//		rsDivision.Update();
	//	}
	//}
	return true;
}

void CStratOMaticSqLiteDoc::ExportLeagueFileToDB(CString strLeagueDir, CString strTeamName)
{
	//SYSTEMTIME lt;
	//CString SqlString;
	//CString lLastName;
	//std::string strLastName;
	//BatterStruct structBatter;
	//PitcherStruct structPitcher;
	//CString tmpTeamID;
	//CString tmpTeamIDNew;
	//CString tmpTeamIDBase;
	//CString tmpLeagueID;
	//CString tmpLeagueIDBase;
	//CString myTeam;
	//CString strDivision;
	//CString myShortTeam;
	//CString myBallpark;
	//CString myYear;
	//CString strBatterFirstName;
	//CString strPitcherFirstName;
	//BYTE count;
	//CFile exportBatter;
	//CFile exportPitcher;
	//CString exportFileName;
	//CString strexportData;
	//int i;
	//CString strTemp;
	//CString myFileName;
	//LONG lTeamSection = 74;
	//LONG lCountSection = 1;
	//LONG lPlayerSection = structBatter.m_RecordSize;
	//LONG lPitcherSection = structPitcher.m_RecordSize;
	//LONG position;
	//char Bunting[5] = { 'A', 'B', 'C', 'D', 'E' };
	//char HitRun[4] = { 'A', 'B', 'C', 'D' };
	//char Stealing[7] = { '3', '2', 'A', 'B', 'C', 'D', 'E' };
	//char BatterHits[3] = { 'L', 'R', 'S' };
	//char Power[2] = { 'N', 'W' };
	//int Running[15] = { 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };
	//int TRate[21] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
	//int Pass[21] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
	//int OutArm[12] = { -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5 };
	//int CatchArm[10] = { -4, -3, -2, -1, 0, 1, 2, 3, 4, 5 };
	//int PHold[16] = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, -1, -2, -3, -4, -5, -6 };

	//count = 0;

	//// Process Batter file
	//strTemp = "XB" + strTeamName.Left(20);
	//exportFileName = strDir + "\\" + strTemp + ".txt"; // dir\XB000001.txt
	//myFileName = strDir + "\\TB" + strTeamName.Right(10);

	//// Allocate the League recordset
	//CLeagues rsLeague(&m_pDatabase);
	//TRY
	//{
	//	// Execute the query
	//	rsLeague.Open(CRecordset::dynaset, NULL, CRecordset::none);
	//}
	//	CATCH(CDBException, e)
	//{
	//	// If a database exception occured, show error msg
	//	AfxMessageBox("Database League RS error: " + e->m_strError);
	//}
	//END_CATCH;

	//// Allocate the League recordset
	//CLeagues rsLeagueBase(&m_pDatabase);
	//TRY
	//{
	//	// Execute the query
	//	rsLeagueBase.Open(CRecordset::dynaset, NULL, CRecordset::none);
	//}
	//	CATCH(CDBException, e)
	//{
	//	// If a database exception occured, show error msg
	//	AfxMessageBox("Database LeagueBase RS error: " + e->m_strError);
	//}
	//END_CATCH;

	//// Allocate the Conference recordset
	//CConferences rsConference(&m_pDatabase);
	//TRY
	//{
	//	// Execute the query
	//	rsConference.Open(CRecordset::dynaset, NULL, CRecordset::none);
	//}
	//	CATCH(CDBException, e)
	//{
	//	// If a database exception occured, show error msg
	//	AfxMessageBox("Database Conference RS error: " + e->m_strError);
	//}
	//END_CATCH;

	//// Allocate the Division recordset
	//CDivisions rsDivision(&m_pDatabase);
	//TRY
	//{
	//	// Execute the query
	//	rsDivision.Open(CRecordset::dynaset, NULL, CRecordset::none);
	//}
	//	CATCH(CDBException, e)
	//{
	//	// If a database exception occured, show error msg
	//	AfxMessageBox("Database Division RS error: " + e->m_strError);
	//}
	//END_CATCH;

	//// Allocate the Teams recordset
	//CTeams rsTeam(&m_pDatabase);
	//TRY
	//{
	//	// Execute the query
	//	rsTeam.Open(CRecordset::dynaset, NULL, CRecordset::none);
	//}
	//	CATCH(CDBException, e)
	//{
	//	// If a database exception occured, show error msg
	//	AfxMessageBox("Database Teams RS error: " + e->m_strError);
	//}
	//END_CATCH;

	//// Allocate the Teams recordset
	//CTeams rsTeamBase(&m_pDatabase);
	//TRY
	//{
	//	// Execute the query
	//	rsTeamBase.Open(CRecordset::dynaset, NULL, CRecordset::none);
	//}
	//	CATCH(CDBException, e)
	//{
	//	// If a database exception occured, show error msg
	//	AfxMessageBox("Database rsTeamBase RS error: " + e->m_strError);
	//}
	//END_CATCH;

	//// Allocate the Batter recordset
	//CBatter rsBatter(&m_pDatabase);
	//TRY
	//{
	//	// Execute the query
	//	rsBatter.Open(CRecordset::dynaset, NULL, CRecordset::none);
	//}
	//	CATCH(CDBException, e)
	//{
	//	// If a database exception occured, show error msg
	//	AfxMessageBox("Database Batter RS error: " + e->m_strError);
	//}
	//END_CATCH;

	//// Allocate the Batter Statistics recordset
	//CBatterStats rsBatterStats(&m_pDatabase);
	//TRY
	//{
	//	// Execute the query
	//	rsBatterStats.Open(CRecordset::dynaset, NULL, CRecordset::none);
	//}
	//	CATCH(CDBException, e)
	//{
	//	// If a database exception occured, show error msg
	//	AfxMessageBox("Database BatterStats RS error: " + e->m_strError);
	//}
	//END_CATCH;

	//// Process Team entry
	//// Skip "1965 " to start with the actual team name.
	////AfxExtractSubString(myTeam, structBatter.GetTeamBatter(myFileName), 1, _T(' '));
	//myTeam = structBatter.GetTeamBatter(myFileName).Mid(5).TrimRight(' ');
	//myShortTeam = structBatter.GetShortTeamBatter(myFileName);
	//myBallpark = structBatter.GetBallparkBatter(myFileName);
	//AfxExtractSubString(myYear, structBatter.GetTeamBatter(myFileName), 0, _T(' '));

	//strDivision = "None";
	//if (myTeam == "Chicago White Sox" && myYear == "1965")
	//{
	//	rsLeague.m_strFilter = "[LeagueName] = 'The Gnews 1998'";
	//	rsConference.m_strFilter = "[ConferenceName] = '65 Conference'";
	//	rsDivision.m_strFilter = "[DivisionName] = 'None'";
	//	strDivision = "None";
	//}
	//if (myTeam == "Cleveland Indians" && myYear == "1965")
	//{
	//	rsLeague.m_strFilter = "[LeagueName] = 'The Gnews 1998'";
	//	rsConference.m_strFilter = "[ConferenceName] = '65 Conference'";
	//	rsDivision.m_strFilter = "[DivisionName] = 'None'";
	//	strDivision = "None";
	//}
	//if (myTeam == "Minnesota Twins" && myYear == "1965")
	//{
	//	rsLeague.m_strFilter = "[LeagueName] = 'The Gnews 1998'";
	//	rsConference.m_strFilter = "[ConferenceName] = '65 Conference'";
	//	rsDivision.m_strFilter = "[DivisionName] = 'None'";
	//	strDivision = "None";
	//}
	//if (myTeam == "Cincinnati Reds" && myYear == "1965")
	//{
	//	rsLeague.m_strFilter = "[LeagueName] = 'The Gnews 1998'";
	//	rsConference.m_strFilter = "[ConferenceName] = '65 Conference'";
	//	rsDivision.m_strFilter = "[DivisionName] = 'None'";
	//	strDivision = "None";
	//}
	//if (myTeam == "Los Angeles Dodgers" && myYear == "1965")
	//{
	//	rsLeague.m_strFilter = "[LeagueName] = 'The Gnews 1998'";
	//	rsConference.m_strFilter = "[ConferenceName] = '65 Conference'";
	//	rsDivision.m_strFilter = "[DivisionName] = 'None'";
	//	strDivision = "None";
	//}
	//if (myTeam == "San Francisco Giants" && myYear == "1965")
	//{
	//	rsLeague.m_strFilter = "[LeagueName] = 'The Gnews 1998'";
	//	rsConference.m_strFilter = "[ConferenceName] = '65 Conference'";
	//	rsDivision.m_strFilter = "[DivisionName] = 'None'";
	//	strDivision = "None";
	//}

	//if (myTeam == "Baltimore Orioles" && myYear == "1969")
	//{
	//	rsLeague.m_strFilter = "[LeagueName] = 'The Gnews 1998'";
	//	rsConference.m_strFilter = "[ConferenceName] = '69 Conference'";
	//	rsDivision.m_strFilter = "[DivisionName] = 'None'";
	//	strDivision = "None";
	//}
	//if (myTeam == "Seattle Pilots" && myYear == "1969")
	//{
	//	rsLeague.m_strFilter = "[LeagueName] = 'The Gnews 1998'";
	//	rsConference.m_strFilter = "[ConferenceName] = '69 Conference'";
	//	rsDivision.m_strFilter = "[DivisionName] = 'None'";
	//	strDivision = "None";
	//}
	//if (myTeam == "Washington Senators" && myYear == "1969")
	//{
	//	rsLeague.m_strFilter = "[LeagueName] = 'The Gnews 1998'";
	//	rsConference.m_strFilter = "[ConferenceName] = '69 Conference'";
	//	rsDivision.m_strFilter = "[DivisionName] = 'None'";
	//	strDivision = "None";
	//}
	//if (myTeam == "Houston Astros" && myYear == "1969")
	//{
	//	rsLeague.m_strFilter = "[LeagueName] = 'The Gnews 1998'";
	//	rsConference.m_strFilter = "[ConferenceName] = '69 Conference'";
	//	rsDivision.m_strFilter = "[DivisionName] = 'None'";
	//	strDivision = "None";
	//}
	//if (myTeam == "Pittsburg Pirates" && myYear == "1969")
	//{
	//	rsLeague.m_strFilter = "[LeagueName] = 'The Gnews 1998'";
	//	rsConference.m_strFilter = "[ConferenceName] = '69 Conference'";
	//	rsDivision.m_strFilter = "[DivisionName] = 'None'";
	//	strDivision = "None";
	//}
	//if (myTeam == "San Diego Padres" && myYear == "1969")
	//{
	//	rsLeague.m_strFilter = "[LeagueName] = 'The Gnews 1998'";
	//	rsConference.m_strFilter = "[ConferenceName] = '69 Conference'";
	//	rsDivision.m_strFilter = "[DivisionName] = 'None'";
	//	strDivision = "None";
	//}

	//if (myTeam == "Baltland" && myYear == "1997")
	//{
	//	rsLeague.m_strFilter = "[LeagueName] = 'Plano 97 in 99'";
	//	rsConference.m_strFilter = "[ConferenceName] = 'Division A'";
	//	rsDivision.m_strFilter = "[DivisionName] = 'None'";
	//	strDivision = "None";
	//}
	//if (myTeam == "Bostago" && myYear == "1997")
	//{
	//	rsLeague.m_strFilter = "[LeagueName] = 'Plano 97 in 99'";
	//	rsConference.m_strFilter = "[ConferenceName] = 'Division A'";
	//	rsDivision.m_strFilter = "[DivisionName] = 'None'";
	//	strDivision = "None";
	//}
	//if (myTeam == "Cleonto" && myYear == "1997")
	//{
	//	rsLeague.m_strFilter = "[LeagueName] = 'Plano 97 in 99'";
	//	rsConference.m_strFilter = "[ConferenceName] = 'Division A'";
	//	rsDivision.m_strFilter = "[DivisionName] = 'None'";
	//	strDivision = "None";
	//}
	//if (myTeam == "Seaota" && myYear == "1997")
	//{
	//	rsLeague.m_strFilter = "[LeagueName] = 'Plano 97 in 99'";
	//	rsConference.m_strFilter = "[ConferenceName] = 'Division A'";
	//	rsDivision.m_strFilter = "[DivisionName] = 'None'";
	//	strDivision = "None";
	//}
	//if (myTeam == "Texaheim" && myYear == "1997")
	//{
	//	rsLeague.m_strFilter = "[LeagueName] = 'Plano 97 in 99'";
	//	rsConference.m_strFilter = "[ConferenceName] = 'Division A'";
	//	rsDivision.m_strFilter = "[DivisionName] = 'None'";
	//	strDivision = "None";
	//}
	//if (myTeam == "Yorkcity" && myYear == "1997")
	//{
	//	rsLeague.m_strFilter = "[LeagueName] = 'Plano 97 in 99'";
	//	rsConference.m_strFilter = "[ConferenceName] = 'Division A'";
	//	rsDivision.m_strFilter = "[DivisionName] = 'None'";
	//	strDivision = "None";
	//}
	//if (myTeam == "Cinangeles" && myYear == "1997")
	//{
	//	rsLeague.m_strFilter = "[LeagueName] = 'Plano 97 in 99'";
	//	rsConference.m_strFilter = "[ConferenceName] = 'Division B'";
	//	rsDivision.m_strFilter = "[DivisionName] = 'None'";
	//	strDivision = "None";
	//}
	//if (myTeam == "Florago" && myYear == "1997")
	//{
	//	rsLeague.m_strFilter = "[LeagueName] = 'Plano 97 in 99'";
	//	rsConference.m_strFilter = "[ConferenceName] = 'Division B'";
	//	rsDivision.m_strFilter = "[DivisionName] = 'None'";
	//	strDivision = "None";
	//}
	//if (myTeam == "Montrado" && myYear == "1997")
	//{
	//	rsLeague.m_strFilter = "[LeagueName] = 'Plano 97 in 99'";
	//	rsConference.m_strFilter = "[ConferenceName] = 'Division B'";
	//	rsDivision.m_strFilter = "[DivisionName] = 'None'";
	//	strDivision = "None";
	//}
	//if (myTeam == "NewDiego" && myYear == "1997")
	//{
	//	rsLeague.m_strFilter = "[LeagueName] = 'Plano 97 in 99'";
	//	rsConference.m_strFilter = "[ConferenceName] = 'Division B'";
	//	rsDivision.m_strFilter = "[DivisionName] = 'None'";
	//	strDivision = "None";
	//}
	//if (myTeam == "Philanta" && myYear == "1997")
	//{
	//	rsLeague.m_strFilter = "[LeagueName] = 'Plano 97 in 99'";
	//	rsConference.m_strFilter = "[ConferenceName] = 'Division B'";
	//	rsDivision.m_strFilter = "[DivisionName] = 'None'";
	//	strDivision = "None";
	//}
	//if (myTeam == "SanLouis" && myYear == "1997")
	//{
	//	rsLeague.m_strFilter = "[LeagueName] = 'Plano 97 in 99'";
	//	rsConference.m_strFilter = "[ConferenceName] = 'Division B'";
	//	rsDivision.m_strFilter = "[DivisionName] = 'None'";
	//	strDivision = "None";
	//}

	//rsLeague.Requery();
	//rsConference.Requery();
	//rsDivision.Requery();

	//// Determine if the team exists in DB.
	//// Update the filter which is the WHERE portion
	//tmpLeagueID.Format("%d", rsLeague.m_LeagueID);
	//rsTeam.m_strFilter = "[TeamName] = '" + myTeam + "' AND [TeamYear] = '" + myYear +
	//	"' AND [LeagueID] = " + tmpLeagueID;
	//// Execute the query
	//rsTeam.Requery();

	//if (!rsTeam.GetRecordCount())
	//{
	//	// Need to add the team as this team entry is tied to the player statistics.
	//	rsTeam.AddNew();

	//	rsTeam.m_TeamName = myTeam;
	//	rsTeam.m_TeamNameShort = myShortTeam;
	//	rsTeam.m_BallparkName = myBallpark;
	//	rsTeam.m_HomeWins = 0;
	//	rsTeam.m_HomeLosses = 0;
	//	rsTeam.m_AwayWins = 0;
	//	rsTeam.m_AwayLosses = 0;
	//	rsTeam.m_LeagueID = rsLeague.m_LeagueID;
	//	rsTeam.m_ConferenceID = rsConference.m_ConferenceID;
	//	if (strDivision == "None")
	//		rsTeam.m_DivisionID = 0;
	//	else
	//		rsTeam.m_DivisionID = rsDivision.m_DivisionID;
	//	rsTeam.m_TeamYear = myYear;

	//	GetLocalTime(&lt);
	//	rsTeam.m_LastUpdateTime = lt;

	//	rsTeam.Update();
	//}

	//// Get Base League information for base LeagueID
	//if (myYear == "1965")
	//{
	//	rsLeagueBase.m_strFilter = "[LeagueName] = 'Base1965'";
	//}
	//if (myYear == "1969")
	//{
	//	rsLeagueBase.m_strFilter = "[LeagueName] = 'Base1969'";
	//}
	//if (myYear == "1997")
	//{
	//	rsLeagueBase.m_strFilter = "[LeagueName] = 'Base Plano 97 in 99'";
	//}
	//rsLeagueBase.Requery();
	//tmpLeagueIDBase.Format("%d", rsLeagueBase.m_LeagueID);

	//// Update the TeamBase filter
	//tmpLeagueID.Format("%d", rsLeague.m_LeagueID);
	//rsTeamBase.m_strFilter = "[TeamName] = '" + myTeam + "' AND [TeamYear] = '" + myYear +
	//	"' AND [LeagueID] = " + tmpLeagueIDBase;
	//// Update the filter
	//tmpLeagueID.Format("%d", rsLeague.m_LeagueID);
	//rsTeam.m_strFilter = "[TeamName] = '" + myTeam + "' AND [TeamYear] = '" + myYear +
	//	"' AND [LeagueID] = " + tmpLeagueID;
	//// Re-Execute the query
	//rsTeamBase.Requery();
	//// Save TeamID of base Team
	//tmpTeamIDBase.Format("%d", rsTeamBase.m_TeamID);

	//rsTeam.Requery();
	//if (!rsTeam.GetRecordCount())
	//{
	//	// Team does not exist so there is a problem
	//	AfxMessageBox("Database Added Team Missing RS error: ");
	//}

	//// Save TeamID of newly entered Team
	//tmpTeamIDNew.Format("%d", rsTeam.m_TeamID);

	//// A Team was selected so export all of the players
	//count = structBatter.GetCountBatter(myFileName);
	//for (i = 0; i<count; i++)
	//{
	//	position = lTeamSection + (i*lPlayerSection);
	//	structBatter.GetBatter(myFileName, position);

	//	// When the last name is something like O'Tool, the "'" causes a problem
	//	// with the SQL search. By editing the string to insert a double "'"
	//	// in the search, the search works correctly.
	//	lLastName = structBatter.m_PlayerName.Left(structBatter.m_PlayerName.Find(','));
	//	std::string str1 = lLastName;
	//	if (str1.find('\'', 0) != std::string::npos)
	//	{
	//		std::string str2 = str1.substr(0, str1.find('\'', 0));
	//		// Insert the double "'" in the string.
	//		str2 = str2 + '\'' + '\'';
	//		strLastName = str2 + str1.substr((str1.find('\'', 0) + 1), std::string::npos);
	//	}
	//	else
	//	{
	//		strLastName = lLastName;
	//	}

	//	AfxExtractSubString(myYear, structBatter.GetTeamBatter(myFileName), 0, _T(' '));

	//	strBatterFirstName = structBatter.m_PlayerName.Right(
	//		structBatter.m_PlayerName.GetLength() - structBatter.m_PlayerName.Find(", ") - 2).TrimRight(' ');

	//	// Create search for duplicate record here.

	//	// Update the filter which is the WHERE portion to find the player
	//	// based on name and Base team to pick up fixed statistics.
	//	tmpTeamID.Format("%d", rsTeam.m_TeamID);
	//	rsBatter.m_strFilter = "[FirstName] = '" + strBatterFirstName + "'" +
	//		" AND [LastName] = '" + strLastName.c_str() + "'" +
	//		" AND [TeamID] = " + tmpTeamIDBase;
	//	// Execute the query
	//	rsBatter.Requery();
	//	// RowSetSize defaults to 1 so 1 or more matched rows will always result in 1
	//	if (rsBatter.GetRecordCount() == 1)
	//	{
	//		// Batter exists Therefore add statistics to this batter

	//		rsBatterStats.AddNew();

	//		rsBatterStats.m_AB = structBatter.m_AB;
	//		rsBatterStats.m_Runs = structBatter.m_Runs;
	//		rsBatterStats.m_Hits = structBatter.m_Hits;
	//		rsBatterStats.m_RBI = structBatter.m_RBI;
	//		rsBatterStats.m_2B = structBatter.m_2B;
	//		rsBatterStats.m_3B = structBatter.m_3B;
	//		rsBatterStats.m_HomeRuns = structBatter.m_HomeRuns;
	//		rsBatterStats.m_Walk = structBatter.m_Walk;
	//		rsBatterStats.m_StrikeOut = structBatter.m_StrikeOut;
	//		rsBatterStats.m_ReachedOnError = structBatter.m_ReachedOnError;
	//		rsBatterStats.m_Sacrifice = structBatter.m_Sacrifice;
	//		rsBatterStats.m_StolenBase = structBatter.m_StolenBase;
	//		rsBatterStats.m_CS = structBatter.m_CS;
	//		rsBatterStats.m_Games = structBatter.m_Games;
	//		rsBatterStats.m_HBP = structBatter.m_HBP;
	//		if (structBatter.m_AB == 0)
	//		{
	//			rsBatterStats.m_AVG = 0.0f;
	//			rsBatterStats.m_SLG = 0.0f;
	//			rsBatterStats.m_OBP = 0.0f;
	//		}
	//		else
	//		{
	//			rsBatterStats.m_AVG = (float)structBatter.m_Hits / structBatter.m_AB;
	//			rsBatterStats.m_SLG = (float)((structBatter.m_Hits - (structBatter.m_2B + structBatter.m_3B + structBatter.m_HomeRuns)) + (2 * structBatter.m_2B) + (3 * structBatter.m_3B) + (4 * structBatter.m_HomeRuns)) / (structBatter.m_AB);
	//			rsBatterStats.m_OBP = (float)(structBatter.m_Hits + structBatter.m_Walk + structBatter.m_ReachedOnError + structBatter.m_Sacrifice + structBatter.m_StolenBase) / (structBatter.m_AB + structBatter.m_Walk + structBatter.m_ReachedOnError + structBatter.m_Sacrifice + structBatter.m_StolenBase);
	//		}
	//		// BatterID always points back to initial Batter for fixed statistics
	//		rsBatterStats.m_BatterID = rsBatter.m_BatterID;
	//		// TeamID can point to any team as this connects the statistics
	//		// that change based on actual play.
	//		rsBatterStats.m_TeamID = rsTeam.m_TeamID;

	//		GetLocalTime(&lt);
	//		rsBatterStats.m_LastUpdateTime = lt;

	//		rsBatterStats.Update();
	//	}
	//	else
	//	{
	//		// Batter does not exist.
	//		//AfxMessageBox("Database Batter is missing in DB: " + strBatterFirstName + " " + strLastName.c_str());
	//	}
	//}
	//rsBatterStats.Close();
	//rsBatter.Close();

	//// Process Pitcher file
	//strTemp = "XP" + strTeamName.Left(20);
	//exportFileName = strDir + "\\" + strTemp + ".txt"; // dir\XB000001.txt
	//myFileName = strDir + "\\TP" + strTeamName.Right(10);

	//// Allocate the Pitcher recordset
	//CPitcher rsPitcher(&m_pDatabase);
	//TRY
	//{
	//	// Execute the query
	//	rsPitcher.Open(CRecordset::dynaset, NULL, CRecordset::none);
	//}
	//	CATCH(CDBException, e)
	//{
	//	// If a database exception occured, show error msg
	//	AfxMessageBox("Database Pitcher RS error: " + e->m_strError);
	//}
	//END_CATCH;

	//// Allocate the Pitcher Statistics recordset
	//CPitcherStats rsPitcherStats(&m_pDatabase);
	//TRY
	//{
	//	// Execute the query
	//	rsPitcherStats.Open(CRecordset::dynaset, NULL, CRecordset::none);
	//}
	//	CATCH(CDBException, e)
	//{
	//	// If a database exception occured, show error msg
	//	AfxMessageBox("Database Pitcher Statistics RS error: " + e->m_strError);
	//}
	//END_CATCH;

	//// A Team was selected so export all of the players
	//count = structPitcher.GetCountPitcher(myFileName);
	//for (i = 0; i<count; i++)
	//{
	//	position = lCountSection + (i*lPitcherSection);
	//	structPitcher.GetPitcher(myFileName, position);

	//	// When the last name is something like O'Tool, the "'" causes a problem
	//	// with the SQL search. By editing the string to insert a double "'"
	//	// in the search, the search works correctly.
	//	lLastName = structPitcher.m_PitcherName.Left(structPitcher.m_PitcherName.Find(','));
	//	std::string str1 = lLastName;
	//	if (str1.find('\'', 0) != std::string::npos)
	//	{
	//		std::string str2 = str1.substr(0, str1.find('\'', 0));
	//		// Insert the double "'" in the string.
	//		str2 = str2 + '\'' + '\'';
	//		strLastName = str2 + str1.substr((str1.find('\'', 0) + 1), std::string::npos);
	//	}
	//	else
	//	{
	//		strLastName = lLastName;
	//	}

	//	strPitcherFirstName = structPitcher.m_PitcherName.Right(
	//		structPitcher.m_PitcherName.GetLength() - structPitcher.m_PitcherName.Find(", ") - 2).TrimRight(' ');

	//	// Create search for duplicate record here.

	//	// Update the filter which is the WHERE portion to find the player
	//	// based on name and current team.
	//	tmpTeamID.Format("%d", rsTeam.m_TeamID);
	//	rsPitcher.m_strFilter = "[FirstName] = '" + strPitcherFirstName + "'" +
	//		" AND [LastName] = '" + strLastName.c_str() + "'" +
	//		" AND [TeamID] = " + tmpTeamIDBase;
	//	// Execute the query
	//	rsPitcher.Requery();
	//	// RowSetSize defaults to 1 so 1 or more matched rows will always result in 1
	//	if (rsPitcher.GetRecordCount() == 1)
	//	{
	//		// Batter exists Therefore add statistics to this pitcher

	//		rsPitcherStats.AddNew();

	//		rsPitcherStats.m_Wins = structPitcher.m_Wins;
	//		rsPitcherStats.m_Loss = structPitcher.m_Loss;
	//		rsPitcherStats.m_Saves = structPitcher.m_Saves;
	//		rsPitcherStats.m_InningsPitched = (float)atof(structPitcher.m_IP);
	//		rsPitcherStats.m_ER = structPitcher.m_ER;
	//		rsPitcherStats.m_Hits = structPitcher.m_Hits;
	//		rsPitcherStats.m_Walks = structPitcher.m_Walks;
	//		rsPitcherStats.m_Strikeouts = structPitcher.m_Strikeouts;
	//		rsPitcherStats.m_HomeRuns = structPitcher.m_HomeRuns;
	//		rsPitcherStats.m_Games = structPitcher.m_Games;
	//		rsPitcherStats.m_CompleteGames = structPitcher.m_CompGames;
	//		rsPitcherStats.m_Starts = structPitcher.m_Starts;
	//		if (atof(structPitcher.m_IP) == 0)
	//		{
	//			rsPitcherStats.m_ERA = 0.0f;
	//			rsPitcherStats.m_WHIP = 0.0f;
	//		}
	//		else
	//		{
	//			rsPitcherStats.m_ERA = (float)(structPitcher.m_ER * 9) / (float)atof(structPitcher.m_IP);
	//			rsPitcherStats.m_WHIP = (float)((structPitcher.m_Hits + structPitcher.m_Walks) * 9) / (float)atof(structPitcher.m_IP);
	//		}

	//		// PitcherID always points back to initial Batter for fixed statistics
	//		rsPitcherStats.m_PitcherID = rsPitcher.m_PitcherID;
	//		// TeamID can point to any team as this connects the statistics
	//		// that change based on actual play.
	//		rsPitcherStats.m_TeamID = rsTeam.m_TeamID;

	//		GetLocalTime(&lt);
	//		rsPitcherStats.m_LastUpdateTime = lt;

	//		rsPitcherStats.Update();
	//	}
	//	else
	//	{
	//		// Pitcher already exists.
	//		//AfxMessageBox("Database Pitcher is already in DB: " + strPitcherFirstName + " " + strLastName.c_str());
	//	}
	//}

	//rsPitcher.Close();
	//rsPitcherStats.Close();
	//rsTeamBase.Close();
	//rsTeam.Close();
	//rsDivision.Close();
	//rsConference.Close();
	//rsLeagueBase.Close();
	//rsLeague.Close();
}


void CStratOMaticSqLiteDoc::ExportBaseFileToDB(CString strDir, CString strTeamName)
{
	//SYSTEMTIME lt;
	//CString SqlString;
	//CString lLastName;
	//std::string strLastName;
	//BatterStruct structBatter;
	//PitcherStruct structPitcher;
	//CString tmpTeamID;
	//CString myTeam;
	//CString strDivision;
	////CString strTempTeam;
	//CString myShortTeam;
	//CString myBallpark;
	//CString myYear;
	//CString strBatterFirstName;
	//CString strPitcherFirstName;
	//BYTE count;
	//CFile exportBatter;
	//CFile exportPitcher;
	//CString exportFileName;
	//CString strexportData;
	//int i;
	//CString strTemp;
	//CString myFileName;
	//LONG lTeamSection = 74;
	//LONG lCountSection = 1;
	//LONG lPlayerSection = structBatter.m_RecordSize;
	//LONG lPitcherSection = structPitcher.m_RecordSize;
	//LONG position;
	//char Bunting[5] = { 'A', 'B', 'C', 'D', 'E' };
	//char HitRun[4] = { 'A', 'B', 'C', 'D' };
	//char Stealing[7] = { '3', '2', 'A', 'B', 'C', 'D', 'E' };
	//char BatterHits[3] = { 'L', 'R', 'S' };
	//char Power[2] = { 'N', 'W' };
	//int Running[15] = { 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };
	//int TRate[21] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
	//int Pass[21] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
	//int OutArm[12] = { -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5 };
	//int CatchArm[10] = { -4, -3, -2, -1, 0, 1, 2, 3, 4, 5 };
	//int PHold[16] = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, -1, -2, -3, -4, -5, -6 };

	//count = 0;

	//// Process Batter file
	//strTemp = "XB" + strTeamName.Left(20);
	//exportFileName = strDir + "\\" + strTemp + ".txt"; // dir\XB000001.txt
	//myFileName = strDir + "\\TB" + strTeamName.Right(10);

	//// Allocate the League recordset
	//CLeagues rsLeague(&m_pDatabase);
	//TRY
	//{
	//	// Execute the query
	//	rsLeague.Open(CRecordset::dynaset, NULL, CRecordset::none);
	//}
	//	CATCH(CDBException, e)
	//{
	//	// If a database exception occured, show error msg
	//	AfxMessageBox("Database League RS error: " + e->m_strError);
	//}
	//END_CATCH;

	//// Allocate the Conference recordset
	//CConferences rsConference(&m_pDatabase);
	//TRY
	//{
	//	// Execute the query
	//	rsConference.Open(CRecordset::dynaset, NULL, CRecordset::none);
	//}
	//	CATCH(CDBException, e)
	//{
	//	// If a database exception occured, show error msg
	//	AfxMessageBox("Database Conference RS error: " + e->m_strError);
	//}
	//END_CATCH;

	//// Allocate the Division recordset
	//CDivisions rsDivision(&m_pDatabase);
	//TRY
	//{
	//	// Execute the query
	//	rsDivision.Open(CRecordset::dynaset, NULL, CRecordset::none);
	//}
	//	CATCH(CDBException, e)
	//{
	//	// If a database exception occured, show error msg
	//	AfxMessageBox("Database Division RS error: " + e->m_strError);
	//}
	//END_CATCH;

	//// Allocate the Teams recordset
	//CTeams rsTeam(&m_pDatabase);
	//TRY
	//{
	//	// Execute the query
	//	rsTeam.Open(CRecordset::dynaset, NULL, CRecordset::none);
	//}
	//	CATCH(CDBException, e)
	//{
	//	// If a database exception occured, show error msg
	//	AfxMessageBox("Database Teams RS error: " + e->m_strError);
	//}
	//END_CATCH;

	//// Allocate the Batter recordset
	//CBatter rsBatter(&m_pDatabase);
	//TRY
	//{
	//	// Execute the query
	//	rsBatter.Open(CRecordset::dynaset, NULL, CRecordset::none);
	//}
	//	CATCH(CDBException, e)
	//{
	//	// If a database exception occured, show error msg
	//	AfxMessageBox("Database Batter RS error: " + e->m_strError);
	//}
	//END_CATCH;

	//// Allocate the Batter Statistics recordset
	//CBatterStats rsBatterStats(&m_pDatabase);
	//TRY
	//{
	//	// Execute the query
	//	rsBatterStats.Open(CRecordset::dynaset, NULL, CRecordset::none);
	//}
	//	CATCH(CDBException, e)
	//{
	//	// If a database exception occured, show error msg
	//	AfxMessageBox("Database BatterStats RS error: " + e->m_strError);
	//}
	//END_CATCH;

	//// Process Team entry
	//// Skip "1965 " to start with the actual team name.
	////AfxExtractSubString(myTeam, structBatter.GetTeamBatter(myFileName), 1, _T(' '));
	//myTeam = structBatter.GetTeamBatter(myFileName).Mid(5).TrimRight(' ');
	//myShortTeam = structBatter.GetShortTeamBatter(myFileName);
	//myBallpark = structBatter.GetBallparkBatter(myFileName);
	//AfxExtractSubString(myYear, structBatter.GetTeamBatter(myFileName), 0, _T(' '));

	//// Determine if the team exists in DB.
	//// Update the filter which is the WHERE portion
	//rsTeam.m_strFilter = "[TeamName] = '" + myTeam + "' AND [TeamYear] = '" + myYear + "'";
	//// Execute the query
	//rsTeam.Requery();
	//if (!rsTeam.GetRecordCount())
	//{
	//	// Team does not exist so add it
	//	strDivision = "None";
	//	if (myTeam == "Chicago White Sox" && myYear == "1965")
	//	{
	//		rsLeague.m_strFilter = "[LeagueName] = 'Base1965'";
	//		rsConference.m_strFilter = "[ConferenceName] = 'BaseAL1965'";
	//		rsDivision.m_strFilter = "[DivisionName] = 'None'";
	//		strDivision = "None";
	//	}
	//	if (myTeam == "Cleveland Indians" && myYear == "1965")
	//	{
	//		rsLeague.m_strFilter = "[LeagueName] = 'Base1965'";
	//		rsConference.m_strFilter = "[ConferenceName] = 'BaseAL1965'";
	//		rsDivision.m_strFilter = "[DivisionName] = 'None'";
	//		strDivision = "None";
	//	}
	//	if (myTeam == "Minnesota Twins" && myYear == "1965")
	//	{
	//		rsLeague.m_strFilter = "[LeagueName] = 'Base1965'";
	//		rsConference.m_strFilter = "[ConferenceName] = 'BaseAL1965'";
	//		rsDivision.m_strFilter = "[DivisionName] = 'None'";
	//		strDivision = "None";
	//	}
	//	if (myTeam == "Cincinnati Reds" && myYear == "1965")
	//	{
	//		rsLeague.m_strFilter = "[LeagueName] = 'Base1965'";
	//		rsConference.m_strFilter = "[ConferenceName] = 'BaseNL1965'";
	//		rsDivision.m_strFilter = "[DivisionName] = 'None'";
	//		strDivision = "None";
	//	}
	//	if (myTeam == "Los Angeles Dodgers" && myYear == "1965")
	//	{
	//		rsLeague.m_strFilter = "[LeagueName] = 'Base1965'";
	//		rsConference.m_strFilter = "[ConferenceName] = 'BaseNL1965'";
	//		rsDivision.m_strFilter = "[DivisionName] = 'None'";
	//		strDivision = "None";
	//	}
	//	if (myTeam == "San Francisco Giants" && myYear == "1965")
	//	{
	//		rsLeague.m_strFilter = "[LeagueName] = 'Base1965'";
	//		rsConference.m_strFilter = "[ConferenceName] = 'BaseNL1965'";
	//		rsDivision.m_strFilter = "[DivisionName] = 'None'";
	//		strDivision = "None";
	//	}

	//	if (myTeam == "Baltimore Orioles" && myYear == "1969")
	//	{
	//		rsLeague.m_strFilter = "[LeagueName] = 'Base1969'";
	//		rsConference.m_strFilter = "[ConferenceName] = 'BaseAL1969'";
	//		rsDivision.m_strFilter = "[DivisionName] = 'BaseALEast1969'";
	//		strDivision = "BaseALEast1969";
	//	}
	//	if (myTeam == "Seattle Pilots" && myYear == "1969")
	//	{
	//		rsLeague.m_strFilter = "[LeagueName] = 'Base1969'";
	//		rsConference.m_strFilter = "[ConferenceName] = 'BaseAL1969'";
	//		rsDivision.m_strFilter = "[DivisionName] = 'BaseALWest1969'";
	//		strDivision = "BaseALWest1969";
	//	}
	//	if (myTeam == "Washington Senators" && myYear == "1969")
	//	{
	//		rsLeague.m_strFilter = "[LeagueName] = 'Base1969'";
	//		rsConference.m_strFilter = "[ConferenceName] = 'BaseAL1969'";
	//		rsDivision.m_strFilter = "[DivisionName] = 'BaseALEast1969'";
	//		strDivision = "BaseALEast1969";
	//	}
	//	if (myTeam == "Houston Astros" && myYear == "1969")
	//	{
	//		rsLeague.m_strFilter = "[LeagueName] = 'Base1969'";
	//		rsConference.m_strFilter = "[ConferenceName] = 'BaseNL1969'";
	//		rsDivision.m_strFilter = "[DivisionName] = 'BaseNLWest1969'";
	//		strDivision = "BaseNLWest1969";
	//	}
	//	if (myTeam == "Pittsburg Pirates" && myYear == "1969")
	//	{
	//		rsLeague.m_strFilter = "[LeagueName] = 'Base1969'";
	//		rsConference.m_strFilter = "[ConferenceName] = 'BaseNL1969'";
	//		rsDivision.m_strFilter = "[DivisionName] = 'BaseNLEast1969'";
	//		strDivision = "BaseNLEast1969";
	//	}
	//	if (myTeam == "San Diego Padres" && myYear == "1969")
	//	{
	//		rsLeague.m_strFilter = "[LeagueName] = 'Base1969'";
	//		rsConference.m_strFilter = "[ConferenceName] = 'BaseNL1969'";
	//		rsDivision.m_strFilter = "[DivisionName] = 'BaseNLWest1969'";
	//		strDivision = "BaseNLWest1969";
	//	}

	//	if (myTeam == "Cleveland" && myYear == "1997")
	//	{
	//		rsLeague.m_strFilter = "[LeagueName] = 'Base1997'";
	//		rsConference.m_strFilter = "[ConferenceName] = 'BaseAL1997'";
	//		rsDivision.m_strFilter = "[DivisionName] = 'BaseALCentral1997'";
	//		strDivision = "BaseALCentral1997";
	//	}
	//	if (myTeam == "Florida" && myYear == "1997")
	//	{
	//		rsLeague.m_strFilter = "[LeagueName] = 'Base1997'";
	//		rsConference.m_strFilter = "[ConferenceName] = 'BaseNL1997'";
	//		rsDivision.m_strFilter = "[DivisionName] = 'BaseNLEast1997'";
	//		strDivision = "BaseNLEast1997";
	//	}
	//	if (myTeam == "Anaheim" && myYear == "1997")
	//	{
	//		rsLeague.m_strFilter = "[LeagueName] = 'Base1997'";
	//		rsConference.m_strFilter = "[ConferenceName] = 'BaseAL1997'";
	//		rsDivision.m_strFilter = "[DivisionName] = 'BaseALWest1997'";
	//		strDivision = "BaseALWest1997";
	//	}
	//	if (myTeam == "Baltimore" && myYear == "1997")
	//	{
	//		rsLeague.m_strFilter = "[LeagueName] = 'Base1997'";
	//		rsConference.m_strFilter = "[ConferenceName] = 'BaseAL1997'";
	//		rsDivision.m_strFilter = "[DivisionName] = 'BaseALEast1997'";
	//		strDivision = "BaseALEast1997";
	//	}

	//	if (myTeam == "Baltland" && myYear == "1997")
	//	{
	//		rsLeague.m_strFilter = "[LeagueName] = 'Base Plano 97 in 99'";
	//		rsConference.m_strFilter = "[ConferenceName] = 'Base Division A'";
	//		rsDivision.m_strFilter = "[DivisionName] = 'None'";
	//		strDivision = "None";
	//	}
	//	if (myTeam == "Bostago" && myYear == "1997")
	//	{
	//		rsLeague.m_strFilter = "[LeagueName] = 'Base Plano 97 in 99'";
	//		rsConference.m_strFilter = "[ConferenceName] = 'Base Division A'";
	//		rsDivision.m_strFilter = "[DivisionName] = 'None'";
	//		strDivision = "None";
	//	}
	//	if (myTeam == "Cleonto" && myYear == "1997")
	//	{
	//		rsLeague.m_strFilter = "[LeagueName] = 'Base Plano 97 in 99'";
	//		rsConference.m_strFilter = "[ConferenceName] = 'Base Division A'";
	//		rsDivision.m_strFilter = "[DivisionName] = 'None'";
	//		strDivision = "None";
	//	}
	//	if (myTeam == "Seaota" && myYear == "1997")
	//	{
	//		rsLeague.m_strFilter = "[LeagueName] = 'Base Plano 97 in 99'";
	//		rsConference.m_strFilter = "[ConferenceName] = 'Base Division A'";
	//		rsDivision.m_strFilter = "[DivisionName] = 'None'";
	//		strDivision = "None";
	//	}
	//	if (myTeam == "Texaheim" && myYear == "1997")
	//	{
	//		rsLeague.m_strFilter = "[LeagueName] = 'Base Plano 97 in 99'";
	//		rsConference.m_strFilter = "[ConferenceName] = 'Base Division A'";
	//		rsDivision.m_strFilter = "[DivisionName] = 'None'";
	//		strDivision = "None";
	//	}
	//	if (myTeam == "Yorkcity" && myYear == "1997")
	//	{
	//		rsLeague.m_strFilter = "[LeagueName] = 'Base Plano 97 in 99'";
	//		rsConference.m_strFilter = "[ConferenceName] = 'Base Division A'";
	//		rsDivision.m_strFilter = "[DivisionName] = 'None'";
	//		strDivision = "None";
	//	}
	//	if (myTeam == "Cinangeles" && myYear == "1997")
	//	{
	//		rsLeague.m_strFilter = "[LeagueName] = 'Base Plano 97 in 99'";
	//		rsConference.m_strFilter = "[ConferenceName] = 'Base Division B'";
	//		rsDivision.m_strFilter = "[DivisionName] = 'None'";
	//		strDivision = "None";
	//	}
	//	if (myTeam == "Florago" && myYear == "1997")
	//	{
	//		rsLeague.m_strFilter = "[LeagueName] = 'Base Plano 97 in 99'";
	//		rsConference.m_strFilter = "[ConferenceName] = 'Base Division B'";
	//		rsDivision.m_strFilter = "[DivisionName] = 'None'";
	//		strDivision = "None";
	//	}
	//	if (myTeam == "Montrado" && myYear == "1997")
	//	{
	//		rsLeague.m_strFilter = "[LeagueName] = 'Base Plano 97 in 99'";
	//		rsConference.m_strFilter = "[ConferenceName] = 'Base Division B'";
	//		rsDivision.m_strFilter = "[DivisionName] = 'None'";
	//		strDivision = "None";
	//	}
	//	if (myTeam == "NewDiego" && myYear == "1997")
	//	{
	//		rsLeague.m_strFilter = "[LeagueName] = 'Base Plano 97 in 99'";
	//		rsConference.m_strFilter = "[ConferenceName] = 'Base Division B'";
	//		rsDivision.m_strFilter = "[DivisionName] = 'None'";
	//		strDivision = "None";
	//	}
	//	if (myTeam == "Philanta" && myYear == "1997")
	//	{
	//		rsLeague.m_strFilter = "[LeagueName] = 'Base Plano 97 in 99'";
	//		rsConference.m_strFilter = "[ConferenceName] = 'Base Division B'";
	//		rsDivision.m_strFilter = "[DivisionName] = 'None'";
	//		strDivision = "None";
	//	}
	//	if (myTeam == "SanLouis" && myYear == "1997")
	//	{
	//		rsLeague.m_strFilter = "[LeagueName] = 'Base Plano 97 in 99'";
	//		rsConference.m_strFilter = "[ConferenceName] = 'Base Division B'";
	//		rsDivision.m_strFilter = "[DivisionName] = 'None'";
	//		strDivision = "None";
	//	}

	//	rsLeague.Requery();
	//	rsConference.Requery();
	//	rsDivision.Requery();

	//	rsTeam.AddNew();

	//	rsTeam.m_TeamName = myTeam;
	//	rsTeam.m_TeamNameShort = myShortTeam;
	//	rsTeam.m_BallparkName = myBallpark;
	//	rsTeam.m_HomeWins = 0;
	//	rsTeam.m_HomeLosses = 0;
	//	rsTeam.m_AwayWins = 0;
	//	rsTeam.m_AwayLosses = 0;
	//	rsTeam.m_LeagueID = rsLeague.m_LeagueID;
	//	rsTeam.m_ConferenceID = rsConference.m_ConferenceID;
	//	if (strDivision == "None")
	//		rsTeam.m_DivisionID = 0;
	//	else
	//		rsTeam.m_DivisionID = rsDivision.m_DivisionID;
	//	rsTeam.m_TeamYear = myYear;
	//	rsTeam.m_BaseTeam = TRUE;

	//	GetLocalTime(&lt);
	//	rsTeam.m_LastUpdateTime = lt;

	//	rsTeam.Update();
	//}

	//// Re-Execute the query
	//rsTeam.Requery();
	//if (!rsTeam.GetRecordCount())
	//{
	//	// Team does not exist so there is a problem
	//	AfxMessageBox("Database Added Team Missing RS error: ");
	//}

	//// A Team was selected so export all of the players
	//count = structBatter.GetCountBatter(myFileName);
	//for (i = 0; i<count; i++)
	//{
	//	position = lTeamSection + (i*lPlayerSection);
	//	structBatter.GetBatter(myFileName, position);

	//	// When the last name is something like O'Tool, the "'" causes a problem
	//	// with the SQL search. By editing the string to insert a double "'"
	//	// in the search, the search works correctly.
	//	lLastName = structBatter.m_PlayerName.Left(structBatter.m_PlayerName.Find(','));
	//	std::string str1 = lLastName;
	//	if (str1.find('\'', 0) != std::string::npos)
	//	{
	//		std::string str2 = str1.substr(0, str1.find('\'', 0));
	//		// Insert the double "'" in the string.
	//		str2 = str2 + '\'' + '\'';
	//		strLastName = str2 + str1.substr((str1.find('\'', 0) + 1), std::string::npos);
	//	}
	//	else
	//	{
	//		strLastName = lLastName;
	//	}

	//	AfxExtractSubString(myYear, structBatter.GetTeamBatter(myFileName), 0, _T(' '));

	//	strBatterFirstName = structBatter.m_PlayerName.Right(
	//		structBatter.m_PlayerName.GetLength() - structBatter.m_PlayerName.Find(", ") - 2).TrimRight(' ');

	//	// Create search for duplicate record here.

	//	// Update the filter which is the WHERE portion to find the player
	//	// based on name and current team.
	//	tmpTeamID.Format("%d", rsTeam.m_TeamID);
	//	rsBatter.m_strFilter = "[FirstName] = '" + strBatterFirstName + "'" +
	//		" AND [LastName] = '" + strLastName.c_str() + "'" +
	//		" AND [TeamID] = " + tmpTeamID;
	//	// Execute the query
	//	rsBatter.Requery();
	//	// RowSetSize defaults to 1 so 1 or more matched rows will always result in 1
	//	if (!rsBatter.GetRecordCount() == 1)
	//	{
	//		// Batter does not exist Therefore add this batter

	//		rsBatter.AddNew();

	//		rsBatter.m_LastName = lLastName;
	//		rsBatter.m_FirstName = strBatterFirstName;
	//		rsBatter.m_Pitcher = structBatter.m_Pitcher;
	//		rsBatter.m_Catcher = structBatter.m_Catcher;
	//		rsBatter.m_FirstBase = structBatter.m_FirstBase;
	//		rsBatter.m_SecondBase = structBatter.m_SecondBase;
	//		rsBatter.m_ShortStop = structBatter.m_ShortStop;
	//		rsBatter.m_ThirdBase = structBatter.m_ThirdBase;
	//		rsBatter.m_LeftField = structBatter.m_LeftField;
	//		rsBatter.m_CenterField = structBatter.m_CenterField;
	//		rsBatter.m_RightField = structBatter.m_RightField;
	//		rsBatter.m_Bunting = structBatter.m_bBunting;
	//		rsBatter.m_HitRun = structBatter.m_bHitRun;
	//		rsBatter.m_Running = structBatter.m_bRunning;
	//		rsBatter.m_Stealing = structBatter.m_bStealing;
	//		rsBatter.m_CatchArm = structBatter.m_bCatchArm;
	//		rsBatter.m_OutArm = structBatter.m_bOutArm;
	//		rsBatter.m_PowerRight = structBatter.m_bPowerR;
	//		rsBatter.m_PowerLeft = structBatter.m_bPowerL;
	//		rsBatter.m_Pass = structBatter.m_bPass;
	//		rsBatter.m_TRate = structBatter.m_bTRate;
	//		rsBatter.m_ER1 = structBatter.m_bER1;
	//		rsBatter.m_ER2 = structBatter.m_bER2;
	//		rsBatter.m_ER3 = structBatter.m_bER3;
	//		rsBatter.m_ER4 = structBatter.m_bER4;
	//		rsBatter.m_ER5 = structBatter.m_bER5;
	//		rsBatter.m_ER6 = structBatter.m_bER6;
	//		rsBatter.m_ER7 = structBatter.m_bER7;
	//		rsBatter.m_ER8 = structBatter.m_bER8;
	//		rsBatter.m_ER9 = structBatter.m_bER9;
	//		rsBatter.m_OBChanceHomeRun = structBatter.m_OBChanceHomeRun;
	//		rsBatter.m_OBChanceTriple = structBatter.m_OBChanceTriple;
	//		rsBatter.m_OBChanceDouble = structBatter.m_OBChanceDouble;
	//		rsBatter.m_OBChanceSingle = structBatter.m_OBChanceSingle;
	//		rsBatter.m_OBChanceWalk = structBatter.m_OBChanceWalk;
	//		rsBatter.m_ChanceDoublePlay = structBatter.m_ChanceDoublePlay;
	//		rsBatter.m_OBChanceHomeRunRight = structBatter.m_OBChanceHomeRunRight;
	//		rsBatter.m_OBChanceTripleRight = structBatter.m_OBChanceTripleRight;
	//		rsBatter.m_OBChanceDoubleRight = structBatter.m_OBChanceDoubleRight;
	//		rsBatter.m_OBChanceSingleRight = structBatter.m_OBChanceSingleRight;
	//		rsBatter.m_OBChanceWalkRight = structBatter.m_OBChanceWalkRight;
	//		rsBatter.m_ChanceDoublePlayRight = structBatter.m_ChanceDoublePlayRight;
	//		rsBatter.m_OBChanceHomeRunLeft = structBatter.m_OBChanceHomeRunLeft;
	//		rsBatter.m_OBChanceTripleLeft = structBatter.m_OBChanceTripleLeft;
	//		rsBatter.m_OBChanceDoubleLeft = structBatter.m_OBChanceDoubleLeft;
	//		rsBatter.m_OBChanceSingleLeft = structBatter.m_OBChanceSingleLeft;
	//		rsBatter.m_OBChanceWalkLeft = structBatter.m_OBChanceWalkLeft;
	//		rsBatter.m_ChanceDoublePlayLeft = structBatter.m_ChanceDoublePlayLeft;
	//		rsBatter.m_OBChanceBasic = structBatter.m_OBChanceBasic;
	//		rsBatter.m_OBChanceLeft = structBatter.m_OBChanceLeft;
	//		rsBatter.m_OBChanceRight = structBatter.m_OBChanceRight;
	//		rsBatter.m_BatterHits = structBatter.m_bBatterHits;
	//		rsBatter.m_TeamID = rsTeam.m_TeamID;

	//		GetLocalTime(&lt);
	//		rsBatter.m_LastUpdateTime = lt;

	//		rsBatter.Update();

	//		// Retrieve the ID of the batter that was just inserted.

	//		// Update the filter which is the WHERE portion to find the player
	//		// based on name and current team.
	//		tmpTeamID.Format("%d", rsTeam.m_TeamID);
	//		rsBatter.m_strFilter = "[FirstName] = '" + strBatterFirstName + "'" +
	//			" AND [LastName] = '" + strLastName.c_str() + "'" +
	//			" AND [TeamID] = " + tmpTeamID;
	//		// Execute the query
	//		rsBatter.Requery();
	//		// RowSetSize defaults to 1 so 1 or more matched rows will always result in 1
	//		if (!rsBatter.GetRecordCount() == 1)
	//		{
	//			// Batter does not exist so there is a problem
	//			AfxMessageBox("Database Requery of batter incorrect RS error: ");
	//		}

	//		// Create test to check for duplicate records here

	//		rsBatterStats.AddNew();

	//		rsBatterStats.m_AB = structBatter.m_AB;
	//		rsBatterStats.m_Runs = structBatter.m_Runs;
	//		rsBatterStats.m_Hits = structBatter.m_Hits;
	//		rsBatterStats.m_RBI = structBatter.m_RBI;
	//		rsBatterStats.m_2B = structBatter.m_2B;
	//		rsBatterStats.m_3B = structBatter.m_3B;
	//		rsBatterStats.m_HomeRuns = structBatter.m_HomeRuns;
	//		rsBatterStats.m_Walk = structBatter.m_Walk;
	//		rsBatterStats.m_StrikeOut = structBatter.m_StrikeOut;
	//		rsBatterStats.m_ReachedOnError = structBatter.m_ReachedOnError;
	//		rsBatterStats.m_Sacrifice = structBatter.m_Sacrifice;
	//		rsBatterStats.m_StolenBase = structBatter.m_StolenBase;
	//		rsBatterStats.m_CS = structBatter.m_CS;
	//		rsBatterStats.m_Games = structBatter.m_Games;
	//		rsBatterStats.m_HBP = structBatter.m_HBP;
	//		if (structBatter.m_AB == 0)
	//		{
	//			rsBatterStats.m_AVG = 0.0f;
	//			rsBatterStats.m_SLG = 0.0f;
	//			rsBatterStats.m_OBP = 0.0f;
	//		}
	//		else
	//		{
	//			rsBatterStats.m_AVG = (float)structBatter.m_Hits / structBatter.m_AB;
	//			rsBatterStats.m_SLG = (float)((structBatter.m_Hits - (structBatter.m_2B + structBatter.m_3B + structBatter.m_HomeRuns)) + (2 * structBatter.m_2B) + (3 * structBatter.m_3B) + (4 * structBatter.m_HomeRuns)) / (structBatter.m_AB);
	//			rsBatterStats.m_OBP = (float)(structBatter.m_Hits + structBatter.m_Walk + structBatter.m_ReachedOnError + structBatter.m_Sacrifice + structBatter.m_StolenBase) / (structBatter.m_AB + structBatter.m_Walk + structBatter.m_ReachedOnError + structBatter.m_Sacrifice + structBatter.m_StolenBase);
	//		}
	//		// BatterID always points back to initial Batter for fixed statistics
	//		rsBatterStats.m_BatterID = rsBatter.m_BatterID;
	//		// TeamID can point to any team as this connects the statistics
	//		// that change based on actual play.
	//		rsBatterStats.m_TeamID = rsTeam.m_TeamID;

	//		GetLocalTime(&lt);
	//		rsBatterStats.m_LastUpdateTime = lt;

	//		rsBatterStats.Update();
	//	}
	//	else
	//	{
	//		// Batter already exists.
	//		//AfxMessageBox("Database Batter is already in DB: " + strBatterFirstName + " " + strLastName.c_str());
	//	}
	//}
	//rsBatterStats.Close();
	//rsBatter.Close();

	//// Process Pitcher file
	//strTemp = "XP" + strTeamName.Left(20);
	//exportFileName = strDir + "\\" + strTemp + ".txt"; // dir\XB000001.txt
	//myFileName = strDir + "\\TP" + strTeamName.Right(10);

	//// Allocate the Pitcher recordset
	//CPitcher rsPitcher(&m_pDatabase);
	//TRY
	//{
	//	// Execute the query
	//	rsPitcher.Open(CRecordset::dynaset, NULL, CRecordset::none);
	//}
	//	CATCH(CDBException, e)
	//{
	//	// If a database exception occured, show error msg
	//	AfxMessageBox("Database Pitcher RS error: " + e->m_strError);
	//}
	//END_CATCH;

	//// Allocate the Pitcher Statistics recordset
	//CPitcherStats rsPitcherStats(&m_pDatabase);
	//TRY
	//{
	//	// Execute the query
	//	rsPitcherStats.Open(CRecordset::dynaset, NULL, CRecordset::none);
	//}
	//	CATCH(CDBException, e)
	//{
	//	// If a database exception occured, show error msg
	//	AfxMessageBox("Database Pitcher Statistics RS error: " + e->m_strError);
	//}
	//END_CATCH;

	//// A Team was selected so export all of the players
	//count = structPitcher.GetCountPitcher(myFileName);
	//for (i = 0; i<count; i++)
	//{
	//	position = lCountSection + (i*lPitcherSection);
	//	structPitcher.GetPitcher(myFileName, position);

	//	// When the last name is something like O'Tool, the "'" causes a problem
	//	// with the SQL search. By editing the string to insert a double "'"
	//	// in the search, the search works correctly.
	//	lLastName = structPitcher.m_PitcherName.Left(structPitcher.m_PitcherName.Find(','));
	//	std::string str1 = lLastName;
	//	if (str1.find('\'', 0) != std::string::npos)
	//	{
	//		std::string str2 = str1.substr(0, str1.find('\'', 0));
	//		// Insert the double "'" in the string.
	//		str2 = str2 + '\'' + '\'';
	//		strLastName = str2 + str1.substr((str1.find('\'', 0) + 1), std::string::npos);
	//	}
	//	else
	//	{
	//		strLastName = lLastName;
	//	}

	//	strPitcherFirstName = structPitcher.m_PitcherName.Right(
	//		structPitcher.m_PitcherName.GetLength() - structPitcher.m_PitcherName.Find(", ") - 2).TrimRight(' ');

	//	// Create search for duplicate record here.

	//	// Update the filter which is the WHERE portion to find the player
	//	// based on name and current team.
	//	tmpTeamID.Format("%d", rsTeam.m_TeamID);
	//	rsPitcher.m_strFilter = "[FirstName] = '" + strPitcherFirstName + "'" +
	//		" AND [LastName] = '" + strLastName.c_str() + "'" +
	//		" AND [TeamID] = " + tmpTeamID;
	//	// Execute the query
	//	rsPitcher.Requery();
	//	// RowSetSize defaults to 1 so 1 or more matched rows will always result in 1
	//	if (!rsPitcher.GetRecordCount() == 1)
	//	{
	//		// Batter does not exist Therefore add this batter

	//		rsPitcher.AddNew();

	//		rsPitcher.m_FirstName = strPitcherFirstName;
	//		rsPitcher.m_LastName = lLastName;
	//		rsPitcher.m_OBChanceHomeRun = structPitcher.m_OBChanceHomeRun;
	//		rsPitcher.m_OBChanceTriple = structPitcher.m_OBChanceTriple;
	//		rsPitcher.m_OBChanceDouble = structPitcher.m_OBChanceDouble;
	//		rsPitcher.m_OBChanceSingle = structPitcher.m_OBChanceSingle;
	//		rsPitcher.m_OBChanceWalk = structPitcher.m_OBChanceWalk;
	//		rsPitcher.m_ChanceDoublePlay = structPitcher.m_ChanceDoublePlay;
	//		rsPitcher.m_OBChanceHomeRunRight = structPitcher.m_OBChanceHomeRunRight;
	//		rsPitcher.m_OBChanceTripleRight = structPitcher.m_OBChanceTripleRight;
	//		rsPitcher.m_OBChanceDoubleRight = structPitcher.m_OBChanceDoubleRight;
	//		rsPitcher.m_OBChanceSingleRight = structPitcher.m_OBChanceSingleRight;
	//		rsPitcher.m_OBChanceWalkRight = structPitcher.m_OBChanceWalkRight;
	//		rsPitcher.m_ChanceDoublePlayRight = structPitcher.m_ChanceDoublePlayRight;
	//		rsPitcher.m_OBChanceHomeRunLeft = structPitcher.m_OBChanceHomeRunLeft;
	//		rsPitcher.m_OBChanceTripleLeft = structPitcher.m_OBChanceTripleLeft;
	//		rsPitcher.m_OBChanceDoubleLeft = structPitcher.m_OBChanceDoubleLeft;
	//		rsPitcher.m_OBChanceSingleLeft = structPitcher.m_OBChanceSingleLeft;
	//		rsPitcher.m_OBChanceWalkLeft = structPitcher.m_OBChanceWalkLeft;
	//		rsPitcher.m_ChanceDoublePlayLeft = structPitcher.m_ChanceDoublePlayLeft;
	//		rsPitcher.m_OBChanceBasic = structPitcher.m_OBChanceBasic;
	//		rsPitcher.m_OBChanceLeft = structPitcher.m_OBChanceLeft;
	//		rsPitcher.m_OBChanceRight = structPitcher.m_OBChanceRight;
	//		rsPitcher.m_Starter = structPitcher.m_Starter;
	//		rsPitcher.m_Relief = structPitcher.m_Relief;
	//		rsPitcher.m_Throws = structPitcher.m_Throws;
	//		rsPitcher.m_Bunting = structPitcher.m_Bunting;
	//		rsPitcher.m_Hold = structPitcher.m_Hold;
	//		rsPitcher.m_WP = structPitcher.m_bWP;
	//		rsPitcher.m_Balk = structPitcher.m_bBalk;
	//		rsPitcher.m_PitcherField = structPitcher.m_PitcherField;
	//		rsPitcher.m_ER1 = structPitcher.m_bER1;
	//		rsPitcher.m_TeamID = rsTeam.m_TeamID;

	//		GetLocalTime(&lt);
	//		rsPitcher.m_LastUpdateTime = lt;

	//		rsPitcher.Update();

	//		// Retrieve the ID of the pitcher that was just inserted.

	//		// Update the filter which is the WHERE portion to find the player
	//		// based on name and current team.
	//		tmpTeamID.Format("%d", rsTeam.m_TeamID);
	//		rsPitcher.m_strFilter = "[FirstName] = '" + strPitcherFirstName + "'" +
	//			" AND [LastName] = '" + strLastName.c_str() + "'" +
	//			" AND [TeamID] = " + tmpTeamID;
	//		// Execute the query
	//		rsPitcher.Requery();
	//		// RowSetSize defaults to 1 so 1 or more matched rows will always result in 1
	//		if (!rsPitcher.GetRecordCount() == 1)
	//		{
	//			// Batter does not exist so there is a problem
	//			AfxMessageBox("Database Requery of pitcher incorrect RS error: ");
	//		}

	//		rsPitcherStats.AddNew();

	//		rsPitcherStats.m_Wins = structPitcher.m_Wins;
	//		rsPitcherStats.m_Loss = structPitcher.m_Loss;
	//		rsPitcherStats.m_Saves = structPitcher.m_Saves;
	//		rsPitcherStats.m_InningsPitched = (float)atof(structPitcher.m_IP);
	//		rsPitcherStats.m_ER = structPitcher.m_ER;
	//		rsPitcherStats.m_Hits = structPitcher.m_Hits;
	//		rsPitcherStats.m_Walks = structPitcher.m_Walks;
	//		rsPitcherStats.m_Strikeouts = structPitcher.m_Strikeouts;
	//		rsPitcherStats.m_HomeRuns = structPitcher.m_HomeRuns;
	//		rsPitcherStats.m_Games = structPitcher.m_Games;
	//		rsPitcherStats.m_CompleteGames = structPitcher.m_CompGames;
	//		rsPitcherStats.m_Starts = structPitcher.m_Starts;
	//		if (atof(structPitcher.m_IP) == 0)
	//		{
	//			rsPitcherStats.m_ERA = 0.0f;
	//			rsPitcherStats.m_WHIP = 0.0f;
	//		}
	//		else
	//		{
	//			rsPitcherStats.m_ERA = (float)(structPitcher.m_ER * 9) / (float)atof(structPitcher.m_IP);
	//			rsPitcherStats.m_WHIP = (float)((structPitcher.m_Hits + structPitcher.m_Walks) * 9) / (float)atof(structPitcher.m_IP);
	//		}

	//		// PitcherID always points back to initial Batter for fixed statistics
	//		rsPitcherStats.m_PitcherID = rsPitcher.m_PitcherID;
	//		// TeamID can point to any team as this connects the statistics
	//		// that change based on actual play.
	//		rsPitcherStats.m_TeamID = rsTeam.m_TeamID;

	//		GetLocalTime(&lt);
	//		rsPitcherStats.m_LastUpdateTime = lt;

	//		rsPitcherStats.Update();
	//	}
	//	else
	//	{
	//		// Pitcher already exists.
	//		//AfxMessageBox("Database Pitcher is already in DB: " + strPitcherFirstName + " " + strLastName.c_str());
	//	}
	//}

	//rsPitcher.Close();
	//rsPitcherStats.Close();
	//rsTeam.Close();
	//rsDivision.Close();
	//rsConference.Close();
	//rsLeague.Close();
}


void CStratOMaticSqLiteDoc::OnSqlInsertDefaultLeague()
{
	// TODO: Add your command handler code here
	int myLeagueID = 0;
	int myConfID = 0;

	LeagueInsert("Base1965", 2, 4, true);
	LeagueInsert("Base1969", 2, 4, true);

	// Select the LeagueId
	myLeagueID = GetLeagueID("Base1965");

	if (myLeagueID != 0)
	{
		ConferenceInsert("BaseAL1965", myLeagueID, true);
		ConferenceInsert("BaseNL1965", myLeagueID, true);
		myConfID = GetConferenceID("BaseAL1965");
		if (myConfID != 0)
		{
			DivisionInsert("BaseALEast1965", myLeagueID, myConfID, true);
			DivisionInsert("BaseALWest1965", myLeagueID, myConfID, true);
		}
		myConfID = 0;
		myConfID = GetConferenceID("BaseNL1965");
		if (myConfID != 0)
		{
			DivisionInsert("BaseNLEast1965", myLeagueID, myConfID, true);
			DivisionInsert("BaseNLWest1965", myLeagueID, myConfID, true);
		}
	}

	myLeagueID = 0;
	myConfID = 0;

	// Select the LeagueId
	myLeagueID = GetLeagueID("Base1969");

	if (myLeagueID != 0)
	{
		ConferenceInsert("BaseAL1969", myLeagueID, true);
		ConferenceInsert("BaseNL1969", myLeagueID, true);
		myConfID = GetConferenceID("BaseAL1969");
		if (myConfID != 0)
		{
			DivisionInsert("BaseALEast1969", myLeagueID, myConfID, true);
			DivisionInsert("BaseALWest1969", myLeagueID, myConfID, true);
		}
		myConfID = 0;
		myConfID = GetConferenceID("BaseNL1969");
		if (myConfID != 0)
		{
			DivisionInsert("BaseNLEast1969", myLeagueID, myConfID, true);
			DivisionInsert("BaseNLWest1969", myLeagueID, myConfID, true);
		}
	}
}


int CStratOMaticSqLiteDoc::LeagueInsert(CStringA strName, int NumberConf, int NumDivisions, bool Base)
{
	char *sqlLeague;
	int rc;
	CHAR buffer[100];

	/* Create SQL statement */
	sqlLeague = "INSERT INTO LEAGUES("  \
		"LeagueName," \
		"NumberOfConferences," \
		"NumberOfDivisions," \
		"BaseLeague" \
		")" \
		"VALUES (" \
		"?1," \
		"?2," \
		"?3," \
		"?4" \
		");";

	rc = sqlite3_prepare_v2(m_db, sqlLeague, strlen(sqlLeague), &m_stmt, 0);
	if (rc != SQLITE_OK)
	{

		//fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(m_db));
		sprintf_s(buffer, sizeof(buffer), "Failed to fetch data: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	else
	{
		sprintf_s(buffer, sizeof(buffer), "Prepare for LEAGUES Insert OK: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}

	// Bind the data to field '1' which is the first '?' in the INSERT statement
	rc = sqlite3_bind_text(m_stmt, 1, strName, strlen(strName), SQLITE_STATIC);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind txt: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 2, NumberConf);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 3, NumDivisions);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 4, Base);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}

	rc = sqlite3_step(m_stmt);
	return 0;
}


int CStratOMaticSqLiteDoc::ConferenceInsert(CStringA strConfName, int LeagueID, bool Base)
{
	int rc;
	char *sqlConference;
	CHAR buffer[100];

	/* Create SQL statement */
	sqlConference = "INSERT INTO CONFERENCES("  \
		"ConferenceName," \
		"LeagueID," \
		"BaseConference" \
		")" \
		"VALUES (" \
		"?1," \
		"?2," \
		"?3" \
		");";

	rc = sqlite3_prepare_v2(m_db, sqlConference, strlen(sqlConference), &m_stmt, 0);
	if (rc != SQLITE_OK)
	{

		//fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(m_db));
		sprintf_s(buffer, sizeof(buffer), "Failed to fetch data: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	else
	{
		sprintf_s(buffer, sizeof(buffer), "Prepare for Conference Insert OK: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}

	// Bind the data to field '1' which is the first '?' in the INSERT statement
	rc = sqlite3_bind_text(m_stmt, 1, strConfName, strlen(strConfName), SQLITE_STATIC);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind txt: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 2, LeagueID);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 3, Base);
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
		sprintf_s(buffer, sizeof(buffer), "Execute for Conference Insert OK.\n");
		AddToLog(buffer);
	}

	sqlite3_finalize(m_stmt);
	return 0;
}


int CStratOMaticSqLiteDoc::DivisionInsert(CStringA strDivisionName, int LeagueID, int ConferenceID, bool Base)
{
	int rc;
	char *sqlConference;
	CHAR buffer[100];

	/* Create SQL statement */
	sqlConference = "INSERT INTO DIVISIONS("  \
		"DivisionName," \
		"LeagueID," \
		"ConferenceID," \
		"BaseDivisions" \
		")" \
		"VALUES (" \
		"?1," \
		"?2," \
		"?3," \
		"?4" \
		");";

	rc = sqlite3_prepare_v2(m_db, sqlConference, strlen(sqlConference), &m_stmt, 0);
	if (rc != SQLITE_OK)
	{

		//fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(m_db));
		sprintf_s(buffer, sizeof(buffer), "Failed to fetch data: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	else
	{
		sprintf_s(buffer, sizeof(buffer), "Prepare for Conference Insert OK: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}

	// Bind the data to field '1' which is the first '?' in the INSERT statement
	rc = sqlite3_bind_text(m_stmt, 1, strDivisionName, strlen(strDivisionName), SQLITE_STATIC);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind txt: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 2, LeagueID);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 3, ConferenceID);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 4, Base);
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
		sprintf_s(buffer, sizeof(buffer), "Execute for Conference Insert OK.\n");
		AddToLog(buffer);
	}

	sqlite3_finalize(m_stmt);
	return 0;
}

int CStratOMaticSqLiteDoc::TeamInsert(CStringA strTeamName, CStringA strTeamNameShort, CStringA strBallParkName, int HomeWins, 
	int HomeLosses, int AwayWins, int AwayLosses, int LeagueID, int ConferenceID, int DivisionID, int Year, bool Base)
{
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
		"?1," \
		"?2," \
		"?3," \
		"?4," \
		"?5," \
		"?6," \
		"?7," \
		"?8," \
		"?9," \
		"?10," \
		"?11," \
		"?12" \
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
	// Bind the data to field '1' which is the first '?' in the SELECT statement
	rc = sqlite3_bind_text(m_stmt, 1, strTeamName, strlen(strTeamName), SQLITE_STATIC);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind strTeamName: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_text(m_stmt, 2, strTeamNameShort, strlen(strTeamNameShort), SQLITE_STATIC);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind strTeamNameShort: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_text(m_stmt, 3, strBallParkName, strlen(strBallParkName), SQLITE_STATIC);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind strBallParkName: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 4, HomeWins);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind HomeWins: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 5, HomeLosses);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind HomeLosses: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 6, AwayWins);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind AwayWins: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 7, AwayLosses);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind AwayLosses: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 8, LeagueID);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind LeagueID: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 9, ConferenceID);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind ConferenceID: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 10, DivisionID);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind DivisionID: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 11, Year);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind Year: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 12, Base);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind Base: %s\n", sqlite3_errmsg(m_db));
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
	return 0;
}

int CStratOMaticSqLiteDoc::GetLeagueID(CStringA strLeagueName)
{
	int rc;
	CHAR buffer[100];
	char *sqlSelect;
	int myLeagueID = 0;

	// Select the LeagueId
	sqlSelect = "SELECT LeagueId from LEAGUES WHERE LeagueName = ?1";

	rc = sqlite3_prepare_v2(m_db, sqlSelect, strlen(sqlSelect), &m_stmt, 0);

	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Failed to fetch data: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	// Bind the data to field '1' which is the first '?' in the SELECT statement
	rc = sqlite3_bind_text(m_stmt, 1, strLeagueName, strlen(strLeagueName), SQLITE_STATIC);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind LeagueName: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}

	rc = sqlite3_step(m_stmt);

	if (rc == SQLITE_ROW)
	{
		sprintf_s(buffer, sizeof(buffer), "%s  %i\n", sqlite3_column_name(m_stmt, 0), sqlite3_column_int(m_stmt, 0));
		AddToLog(buffer);
		myLeagueID = sqlite3_column_int(m_stmt, 0);
	}
	else
	{
		sprintf_s(buffer, sizeof(buffer), "Select returned nothing: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}

	sqlite3_finalize(m_stmt);
	return myLeagueID;
}


int CStratOMaticSqLiteDoc::GetConferenceID(CStringA strConfName)
{
	int rc;
	CHAR buffer[100];
	char *sqlSelect;
	int myConfID = 0;

	// Select the LeagueId
	sqlSelect = "SELECT ConferenceID from CONFERENCES WHERE ConferenceName = ?1";

	rc = sqlite3_prepare_v2(m_db, sqlSelect, strlen(sqlSelect), &m_stmt, 0);

	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Failed to fetch data: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	// Bind the data to field '1' which is the first '?' in the SELECT statement
	rc = sqlite3_bind_text(m_stmt, 1, strConfName, strlen(strConfName), SQLITE_STATIC);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind strConfName: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}

	rc = sqlite3_step(m_stmt);

	if (rc == SQLITE_ROW)
	{
		sprintf_s(buffer, sizeof(buffer), "%s  %i\n", sqlite3_column_name(m_stmt, 0), sqlite3_column_int(m_stmt, 0));
		AddToLog(buffer);
		myConfID = sqlite3_column_int(m_stmt, 0);
	}
	else
	{
		sprintf_s(buffer, sizeof(buffer), "Select returned nothing: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}

	sqlite3_finalize(m_stmt);
	return myConfID;
}

int CStratOMaticSqLiteDoc::GetDivisionID(CStringA strDivisionName)
{
	int rc;
	CHAR buffer[100];
	char *sqlSelect;
	int myDivisionID = 0;

	// Select the LeagueId
	sqlSelect = "SELECT DivisionID from DIVISIONS WHERE DivisionName = ?1";

	rc = sqlite3_prepare_v2(m_db, sqlSelect, strlen(sqlSelect), &m_stmt, 0);

	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Failed to fetch data: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	// Bind the data to field '1' which is the first '?' in the SELECT statement
	rc = sqlite3_bind_text(m_stmt, 1, strDivisionName, strlen(strDivisionName), SQLITE_STATIC);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind strDivisionName: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}

	rc = sqlite3_step(m_stmt);

	if (rc == SQLITE_ROW)
	{
		sprintf_s(buffer, sizeof(buffer), "%s  %i\n", sqlite3_column_name(m_stmt, 0), sqlite3_column_int(m_stmt, 0));
		AddToLog(buffer);
		myDivisionID = sqlite3_column_int(m_stmt, 0);
	}
	else
	{
		sprintf_s(buffer, sizeof(buffer), "Select returned nothing: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}

	sqlite3_finalize(m_stmt);
	return myDivisionID;
}

int CStratOMaticSqLiteDoc::BatterInsert(CStringA strFirstName, CStringA strLastName, int Pitcher, int Catcher, int FirstBase, 
	int SecondBase, int ShortStop, int ThirdBase, int LeftField, int CenterField, int RightField, int Bunting, int HitRun, 
	int Running, int Stealing, int CatchArm, int OutArm, int PowerRight, int PowerLeft, int Pass, int TRate, 
	int ER1, int ER2, int ER3, int ER4, int ER5, int ER6, int ER7, int ER8, int ER9, 
	bool BatterHits, int TeamID, float OBChanceHomeRun, float OBChanceTriple, float OBChanceDouble, float OBChanceSingle, 
	float OBChanceWalk, float ChanceDoublePlay, float OBChanceHomeRunRight, float OBChanceTripleRight, float OBChanceDoubleRight, 
	float OBChanceSingleRight, float OBChanceWalkRight, float ChanceDoublePlayRight, float OBChanceHomeRunLeft, float OBChanceTripleLeft, 
	float OBChanceDoubleLeft, float OBChanceSingleLeft, float OBChanceWalkLeft, float ChanceDoublePlayLeft, float OBChanceBasic, 
	float OBChanceLeft, float OBChanceRight)
{
	int rc;
	CHAR buffer[100];
	char *sqlBatter;

	/* Create SQL statement */
	sqlBatter = "INSERT INTO BATTER("  \
		"FirstName," \
		"LastName," \
		"Pitcher," \
		"Catcher," \
		"FirstBase," \
		"SecondBase," \
		"ShortStop," \
		"ThirdBase," \
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
		"?1," \
		"?2," \
		"?3," \
		"?4," \
		"?5," \
		"?6," \
		"?7," \
		"?8," \
		"?9," \
		"?10," \
		"?11," \
		"?12," \
		"?13," \
		"?14," \
		"?15," \
		"?16," \
		"?17," \
		"?18," \
		"?19," \
		"?20," \
		"?21," \
		"?22," \
		"?23," \
		"?24," \
		"?25," \
		"?26," \
		"?27," \
		"?28," \
		"?29," \
		"?30," \
		"?31," \
		"?32," \
		"?33," \
		"?34," \
		"?35," \
		"?36," \
		"?37," \
		"?38," \
		"?39," \
		"?40," \
		"?41," \
		"?42," \
		"?43," \
		"?44," \
		"?45," \
		"?46," \
		"?47," \
		"?48," \
		"?49," \
		"?50," \
		"?51," \
		"?52," \
		"?53" \
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
	rc = sqlite3_bind_text(m_stmt, 1, strFirstName, strlen(strFirstName), SQLITE_STATIC);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind strDivisionName: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_text(m_stmt, 2, strLastName, strlen(strLastName), SQLITE_STATIC);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind strDivisionName: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 3, Pitcher);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 4, Catcher);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 5, FirstBase);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 6, SecondBase);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 7, ShortStop);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 8, ThirdBase);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 9, LeftField);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 10, CenterField);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 11, RightField);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 12, Bunting);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 13, HitRun);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 14, Running);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 15, Stealing);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 16, CatchArm);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 17, OutArm);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 18, PowerRight);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 19, PowerLeft);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 20, Pass);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 21, TRate);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 22, ER1);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 23, ER2);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 24, ER3);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 25, ER4);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 26, ER5);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 27, ER6);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 28, ER7);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 29, ER8);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 30, ER9);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 31, BatterHits);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 32, TeamID);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_double(m_stmt, 33, OBChanceHomeRun);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind double: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_double(m_stmt, 34, OBChanceTriple);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind double: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_double(m_stmt, 35, OBChanceDouble);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind intdouble %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_double(m_stmt, 36, OBChanceSingle);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind double: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_double(m_stmt, 37, OBChanceWalk);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind double: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_double(m_stmt, 38, ChanceDoublePlay);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind double: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_double(m_stmt, 39, OBChanceHomeRunRight);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind double: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_double(m_stmt, 40, OBChanceTripleRight);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind double: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_double(m_stmt, 41, OBChanceDoubleRight);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind double: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_double(m_stmt, 42, OBChanceSingleRight);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind double: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_double(m_stmt, 43, OBChanceWalkRight);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind double: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_double(m_stmt, 44, ChanceDoublePlayRight);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind double: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_double(m_stmt, 45, OBChanceHomeRunLeft);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind double: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_double(m_stmt, 46, OBChanceTripleLeft);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind double: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_double(m_stmt, 47, OBChanceDoubleLeft);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind double: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_double(m_stmt, 48, OBChanceSingleLeft);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind double: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_double(m_stmt, 49, OBChanceWalkLeft);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind double: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_double(m_stmt, 50, ChanceDoublePlayLeft);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind double: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_double(m_stmt, 51, OBChanceBasic);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind double: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_double(m_stmt, 52, OBChanceLeft);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind double: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_double(m_stmt, 53, OBChanceRight);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind double: %s\n", sqlite3_errmsg(m_db));
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
	return 0;
}


int CStratOMaticSqLiteDoc::GetTeamID(CStringA strTeamName, int Year)
{
	int myTeamId = 0;
	int rc;
	CHAR buffer[100];
	char *sqlSelect;

	sqlSelect = "SELECT TeamId from TEAM WHERE TeamName = ?1 and TeamYear = ?2";

	rc = sqlite3_prepare_v2(m_db, sqlSelect, strlen(sqlSelect), &m_stmt, 0);

	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Failed to fetch data: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}

	// Bind the data to field '1' which is the first '?' in the SELECT statement
	rc = sqlite3_bind_text(m_stmt, 1, strTeamName, strlen(strTeamName), SQLITE_STATIC);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind teamname: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 2, Year);
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
	return myTeamId;
}
