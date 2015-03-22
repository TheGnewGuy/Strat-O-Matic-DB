
// Strat-O-Matic-SqLiteDoc.cpp : implementation of the CStratOMaticSqLiteDoc class
//
// SELECT printf("%.2f", floatField) AS field FROM table;
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
#include <string>

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
	ON_COMMAND(ID_SQL_INSERTPITCHER, &CStratOMaticSqLiteDoc::OnSqlInsertPitcher)
	ON_COMMAND(ID_SQL_INSERTPITCHERSTATS, &CStratOMaticSqLiteDoc::OnSqlInsertPitcherStats)
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
		"LeagueYear            INT      NOT NULL DEFAULT 1800," \
		"ActiveRec             BOOL     NOT NULL DEFAULT TRUE," \
		"CreateTime            DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP," \
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
		"ActiveRec             BOOL     NOT NULL DEFAULT TRUE," \
		"CreateTime            DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP," \
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
		"ActiveRec             BOOL     NOT NULL DEFAULT TRUE," \
		"CreateTime            DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP," \
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
		"TotalWins             INT      NOT NULL DEFAULT 0," \
		"TotalLosses           INT      NOT NULL DEFAULT 0," \
		"HomeWins              INT      NOT NULL DEFAULT 0," \
		"HomeLosses            INT      NOT NULL DEFAULT 0," \
		"AwayWins              INT      NOT NULL DEFAULT 0," \
		"AwayLosses            INT      NOT NULL DEFAULT 0," \
		"LeagueID              INTEGER  NOT NULL," \
		"ConferenceID          INTEGER  NOT NULL," \
		"DivisionID            INTEGER  NOT NULL," \
		"TeamYear              INT      NOT NULL," \
		"BaseTeam              BOOL     NOT NULL DEFAULT FALSE," \
		"ActiveRec             BOOL     NOT NULL DEFAULT TRUE," \
		"CreateTime            DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP," \
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
		"ActiveRec             BOOL     NOT NULL DEFAULT TRUE," \
		"CreateTime            DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP," \
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
		"OBP                   FLOAT    NOT NULL," \
		"BatterID              INTEGER  NOT NULL," \
		"TeamID                INTEGER  NOT NULL," \
		"ActiveRec             BOOL     NOT NULL DEFAULT TRUE," \
		"CreateTime            DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP," \
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

	char *sqlPitcher = "CREATE TABLE PITCHER("  \
		"PitcherID             INTEGER  NOT NULL PRIMARY KEY AUTOINCREMENT," \
		"FirstName             TEXT     NOT NULL," \
		"LastName              TEXT     NOT NULL," \
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
		"Starter               INT      NOT NULL," \
		"Relief                INT      NOT NULL," \
		"Throws                INT      NOT NULL," \
		"Bunting               INT      NOT NULL," \
		"Hold                  INT      NOT NULL," \
		"WP                    INT      NOT NULL," \
		"Balk                  INT      NOT NULL," \
		"Pitcher               INT      NOT NULL," \
		"ER1                   INT      NOT NULL," \
		"TeamID                INTEGER  NOT NULL," \
		"ActiveRec             BOOL     NOT NULL DEFAULT TRUE," \
		"CreateTime            DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP," \
		"LastUpdateTime        DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP," \
		"FOREIGN KEY(TeamID) REFERENCES TEAM(TeamID)" \
		");";
	rc = sqlite3_prepare_v2(m_db, sqlPitcher, -1, &m_stmt, 0);
	if (rc != SQLITE_OK)
	{

		//fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(m_db));
		sprintf_s(buffer, sizeof(buffer), "Failed to fetch data: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	else
	{
		sprintf_s(buffer, sizeof(buffer), "Prepare for Pitcher Create OK: %s\n", sqlite3_errmsg(m_db));
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
		sprintf_s(buffer, sizeof(buffer), "Step for Pitcher Create OK:\n");
		AddToLog(buffer);
	}

	sqlite3_finalize(m_stmt);

	char *sqlPitcherStats = "CREATE TABLE PITCHERSTATS("  \
		"PitcherStatsID        INTEGER  NOT NULL PRIMARY KEY AUTOINCREMENT," \
		"Wins                  INT      NOT NULL," \
		"Loss                  INT      NOT NULL," \
		"Saves                 INT      NOT NULL," \
		"InningsPitched        FLOAT    NOT NULL," \
		"ER                    INT      NOT NULL," \
		"Hits                  INT      NOT NULL," \
		"Walks                 INT      NOT NULL," \
		"Strikeouts            INT      NOT NULL," \
		"HomeRuns              INT      NOT NULL," \
		"Games                 INT      NOT NULL," \
		"CompleteGames         INT      NOT NULL," \
		"Starts                INT      NOT NULL," \
		"ERA                   FLOAT    NOT NULL," \
		"WHIP                  FLOAT    NOT NULL," \
		"PitcherID             INTEGER  NOT NULL," \
		"TeamID                INTEGER  NOT NULL," \
		"ActiveRec             BOOL     NOT NULL DEFAULT TRUE," \
		"CreateTime            DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP," \
		"LastUpdateTime        DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP," \
		"FOREIGN KEY(PitcherID) REFERENCES PITCHER(PitcherID)," \
		"FOREIGN KEY(TeamID)    REFERENCES TEAM(TeamID)" \
		");";
	rc = sqlite3_prepare_v2(m_db, sqlPitcherStats, -1, &m_stmt, 0);
	if (rc != SQLITE_OK)
	{

		//fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(m_db));
		sprintf_s(buffer, sizeof(buffer), "Failed to fetch data: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	else
	{
		sprintf_s(buffer, sizeof(buffer), "Prepare for PitcherStats Create OK: %s\n", sqlite3_errmsg(m_db));
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
		sprintf_s(buffer, sizeof(buffer), "Step for PitcherStats Create OK: \n");
		AddToLog(buffer);
	}

	sqlite3_finalize(m_stmt);
}


void CStratOMaticSqLiteDoc::OnSqlDeleteTable()
{
	// TODO: Add your command handler code here
	char *sqlstmt;
	int rc;
	CHAR buffer[100];

	/* Create SQL statement */
	sqlstmt = "DROP TABLE IF EXISTS PITCHERSTATS;";

	rc = sqlite3_prepare_v2(m_db, sqlstmt, strlen(sqlstmt), &m_stmt, 0);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Failed to fetch data: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	else
	{
		sprintf_s(buffer, sizeof(buffer), "Prepare for DROP OK:\n");
		AddToLog(buffer);
	}
	rc = sqlite3_step(m_stmt);

	/* Create SQL statement */
	sqlstmt = "DROP TABLE IF EXISTS PITCHER;";
	rc = sqlite3_prepare_v2(m_db, sqlstmt, strlen(sqlstmt), &m_stmt, 0);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Failed to fetch data: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	else
	{
		sprintf_s(buffer, sizeof(buffer), "Prepare for DROP OK:\n");
		AddToLog(buffer);
	}
	rc = sqlite3_step(m_stmt);

	/* Create SQL statement */
	sqlstmt = "DROP TABLE IF EXISTS BATTERSTATS;";
	rc = sqlite3_prepare_v2(m_db, sqlstmt, strlen(sqlstmt), &m_stmt, 0);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Failed to fetch data: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	else
	{
		sprintf_s(buffer, sizeof(buffer), "Prepare for DROP OK:\n");
		AddToLog(buffer);
	}
	rc = sqlite3_step(m_stmt);

	/* Create SQL statement */
	sqlstmt = "DROP TABLE IF EXISTS BATTER;";
	rc = sqlite3_prepare_v2(m_db, sqlstmt, strlen(sqlstmt), &m_stmt, 0);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Failed to fetch data: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	else
	{
		sprintf_s(buffer, sizeof(buffer), "Prepare for DROP OK:\n");
		AddToLog(buffer);
	}
	rc = sqlite3_step(m_stmt);

	/* Create SQL statement */
	sqlstmt = "DROP TABLE IF EXISTS TEAM;";
	rc = sqlite3_prepare_v2(m_db, sqlstmt, strlen(sqlstmt), &m_stmt, 0);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Failed to fetch data: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	else
	{
		sprintf_s(buffer, sizeof(buffer), "Prepare for DROP OK:\n");
		AddToLog(buffer);
	}
	rc = sqlite3_step(m_stmt);

	/* Create SQL statement */
	sqlstmt = "DROP TABLE IF EXISTS DIVISIONS;";
	rc = sqlite3_prepare_v2(m_db, sqlstmt, strlen(sqlstmt), &m_stmt, 0);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Failed to fetch data: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	else
	{
		sprintf_s(buffer, sizeof(buffer), "Prepare for DROP OK:\n");
		AddToLog(buffer);
	}
	rc = sqlite3_step(m_stmt);

	/* Create SQL statement */
	sqlstmt = "DROP TABLE IF EXISTS CONFERENCES;";
	rc = sqlite3_prepare_v2(m_db, sqlstmt, strlen(sqlstmt), &m_stmt, 0);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Failed to fetch data: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	else
	{
		sprintf_s(buffer, sizeof(buffer), "Prepare for DROP OK:\n");
		AddToLog(buffer);
	}
	rc = sqlite3_step(m_stmt);

	/* Create SQL statement */
	sqlstmt = "DROP TABLE IF EXISTS LEAGUES;";
	rc = sqlite3_prepare_v2(m_db, sqlstmt, strlen(sqlstmt), &m_stmt, 0);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Failed to fetch data: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	else
	{
		sprintf_s(buffer, sizeof(buffer), "Prepare for DROP OK:\n");
		AddToLog(buffer);
	}
	rc = sqlite3_step(m_stmt);
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
	BatterInsert("Joe", "Azcue", 0, 4, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		1, 2, 3, 4, 5, 6, 7, 8, 9,
		true, myTeamID, 4.0, 3.0, 2.0, 1.0,
		5.0, 4.1, 9.4, 9.3, 9.2,
		9.1, 9.5, 9.21, 7.4, 7.3,
		7.2, 7.1, 7.5, 7.21, 5.5,
		6.6, 7.7);
	BatterInsert("Rocky", "Colavito", 0, 0, 0,
		0, 0, 0, 0, 0, 3, 0, 0,
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
	int myTeamID = 0;
	int myBatterID = 0;

	// Select the TeamId
	myTeamID =  GetTeamID("Cleveland Indians", 1965);

	// Select the BatterId
	myBatterID = GetBatterID("Max", "Alvis");

	BatterStatsInsert(100, 6, 27, 5, 3, 2, 1, 6,
		7, 1, 1, 2, 1, 25, 0, 0.324, 0.205, 0.123,
		myBatterID, myTeamID);
	myBatterID = GetBatterID("Joe", "Azcue");
	BatterStatsInsert(100, 6, 27, 5, 3, 2, 1, 6,
		7, 1, 1, 2, 1, 25, 0, 0.324, 0.205, 0.123,
		myBatterID, myTeamID);
	myBatterID = GetBatterID("Rocky", "Colavito");
	BatterStatsInsert(100, 6, 27, 5, 3, 2, 1, 6,
		7, 1, 1, 2, 1, 25, 0, 0.324, 0.205, 0.123,
		myBatterID, myTeamID);
}

void CStratOMaticSqLiteDoc::OnSqlInsertPitcher()
{
	// TODO: Add your command handler code here
	int myTeamID;

	// Select the TeamId
	myTeamID = GetTeamID("Cleveland Indians", 1965);
	PitcherInsert("Sonny", "Siebert", 
		0, 0, 0, 0, 0, 3, 
		0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 
		14.35, 0, 0,
		1, 1, 0, 0, 5, 0, 0, 0, 0,
		myTeamID);
	PitcherInsert("Gary", "Bell",
		0, 0, 0, 0, 0, 3,
		0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0,
		25.5, 0, 0,
		0, 1, 0, 0, 5, 0, 0, 0, 0,
		myTeamID);
	PitcherInsert("Sam", "McDowell",
		0, 0, 0, 0, 0, 3,
		0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0,
		21.2, 0, 0,
		1, 1, 0, 0, 5, 0, 0, 0, 0,
		myTeamID);
}


void CStratOMaticSqLiteDoc::OnSqlInsertPitcherStats()
{
	// TODO: Add your command handler code here
	int myTeamID = 0;
	int myPitcherID = 0;

	// Select the TeamId
	myTeamID = GetTeamID("Cleveland Indians", 1965);

	// Select the PitcherID
	myPitcherID = GetPitcherID("Sonny", "Siebert");
	PitcherStatsInsert(16, 8, 0, 189, 51,
		139, 46, 191, 14, 
		0, 0, 0,
		2.43, 8.81,
		myPitcherID, myTeamID);
	myPitcherID = GetPitcherID("Gary", "Bell");
	PitcherStatsInsert(6, 5, 0, 104, 35,
		86, 50, 86, 7,
		0, 0, 0,
		3.03, 11.77,
		myPitcherID, myTeamID);
	myPitcherID = GetPitcherID("Sam", "McDowell");
	PitcherStatsInsert(17, 11, 0, 273, 66,
		178, 132, 325, 9,
		0, 0, 0,
		2.18, 10.22,
		myPitcherID, myTeamID);
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

	//CreateDefaultLeague();

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


void CStratOMaticSqLiteDoc::ExportLeagueFileToDB(CString strLeagueDir, CString strTeamName)
{
	SYSTEMTIME lt;
	CString SqlString;
	CStringA lLastName;
	std::string strLastName;
	BatterStruct structBatter;
	PitcherStruct structPitcher;
	CString tmpTeamID;
	CString tmpTeamIDNew;
	CString tmpTeamIDBase;
	CString tmpLeagueID;
	CString tmpLeagueIDBase;
	CStringA myTeam;
	CString strDivision;
	CStringA myShortTeam;
	CStringA myBallpark;
	CString myYear;
	CStringA strBatterFirstName;
	CStringA strPitcherFirstName;
	BYTE count;
	CFile exportBatter;
	CFile exportPitcher;
	CString exportFileName;
	CString strexportData;
	int i;
	CString strTemp;
	CString myFileName;
	LONG lTeamSection = 74;
	LONG lCountSection = 1;
	LONG lPlayerSection = structBatter.m_RecordSize;
	LONG lPitcherSection = structPitcher.m_RecordSize;
	LONG position;
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
	int imyYear;
	int myteamID = 0;
	int myteamStatID = 0;
	int myLeagueID = 0;
	int myConfID = 0;
	int myDivisionID = 0;
	int myBatterID = 0;
	int myPitcherID = 0;

	count = 0;

	// Process Batter file
	strTemp = _T("XB") + strTeamName.Left(20);
	exportFileName = strLeagueDir + _T("\\") + strTemp + _T(".txt"); // dir\XB000001.txt
	myFileName = strLeagueDir + _T("\\TB") + strTeamName.Right(10);

	// Process Team entry
	// Skip "1965 " to start with the actual team name.
	//AfxExtractSubString(myTeam, structBatter.GetTeamBatter(myFileName), 1, _T(' '));
	myTeam = structBatter.GetTeamBatter(myFileName).Mid(5).TrimRight(' ');
	myShortTeam = structBatter.GetShortTeamBatter(myFileName);
	myBallpark = structBatter.GetBallparkBatter(myFileName);
	AfxExtractSubString(myYear, structBatter.GetTeamBatter(myFileName), 0, _T(' '));
	imyYear = _ttoi(myYear);

	// Determine if the team exists in DB.
	if (myTeam == "Chicago White Sox" && imyYear == 1965)
	{
		myLeagueID = GetLeagueID("The Gnews 1998");
		myConfID = GetConferenceID("65 Conference");
		myDivisionID = GetDivisionID("DEFAULT");
	}
	if (myTeam == "Cleveland Indians" && imyYear == 1965)
	{
		myLeagueID = GetLeagueID("The Gnews 1998");
		myConfID = GetConferenceID("65 Conference");
		myDivisionID = GetDivisionID("DEFAULT");
	}
	if (myTeam == "Minnesota Twins" && imyYear == 1965)
	{
		myLeagueID = GetLeagueID("The Gnews 1998");
		myConfID = GetConferenceID("65 Conference");
		myDivisionID = GetDivisionID("DEFAULT");
	}
	if (myTeam == "Cincinnati Reds" && imyYear == 1965)
	{
		myLeagueID = GetLeagueID("The Gnews 1998");
		myConfID = GetConferenceID("65 Conference");
		myDivisionID = GetDivisionID("DEFAULT");
	}
	if (myTeam == "Los Angeles Dodgers" && imyYear == 1965)
	{
		myLeagueID = GetLeagueID("The Gnews 1998");
		myConfID = GetConferenceID("65 Conference");
		myDivisionID = GetDivisionID("DEFAULT");
	}
	if (myTeam == "San Francisco Giants" && imyYear == 1965)
	{
		myLeagueID = GetLeagueID("The Gnews 1998");
		myConfID = GetConferenceID("65 Conference");
		myDivisionID = GetDivisionID("DEFAULT");
	}

	if (myTeam == "Baltimore Orioles" && imyYear == 1969)
	{
		myLeagueID = GetLeagueID("The Gnews 1998");
		myConfID = GetConferenceID("69 Conference");
		myDivisionID = GetDivisionID("DEFAULT");
	}
	if (myTeam == "Seattle Pilots" && imyYear == 1969)
	{
		myLeagueID = GetLeagueID("The Gnews 1998");
		myConfID = GetConferenceID("69 Conference");
		myDivisionID = GetDivisionID("DEFAULT");
	}
	if (myTeam == "Washington Senators" && imyYear == 1969)
	{
		myLeagueID = GetLeagueID("The Gnews 1998");
		myConfID = GetConferenceID("69 Conference");
		myDivisionID = GetDivisionID("DEFAULT");
	}
	if (myTeam == "Houston Astros" && imyYear == 1969)
	{
		myLeagueID = GetLeagueID("The Gnews 1998");
		myConfID = GetConferenceID("69 Conference");
		myDivisionID = GetDivisionID("DEFAULT");
	}
	if (myTeam == "Pittsburg Pirates" && imyYear == 1969)
	{
		myLeagueID = GetLeagueID("The Gnews 1998");
		myConfID = GetConferenceID("69 Conference");
		myDivisionID = GetDivisionID("DEFAULT");
	}
	if (myTeam == "San Diego Padres" && imyYear == 1969)
	{
		myLeagueID = GetLeagueID("The Gnews 1998");
		myConfID = GetConferenceID("69 Conference");
		myDivisionID = GetDivisionID("DEFAULT");
	}

	if (myTeam == "Baltland" && imyYear == 1997)
	{
		myLeagueID = GetLeagueID("Plano 97 in 99");
		myConfID = GetConferenceID("Division A");
		myDivisionID = GetDivisionID("DEFAULT");
	}
	if (myTeam == "Bostago" && imyYear == 1997)
	{
		myLeagueID = GetLeagueID("Plano 97 in 99");
		myConfID = GetConferenceID("Division A");
		myDivisionID = GetDivisionID("DEFAULT");
	}
	if (myTeam == "Cleonto" && imyYear == 1997)
	{
		myLeagueID = GetLeagueID("Plano 97 in 99");
		myConfID = GetConferenceID("Division A");
		myDivisionID = GetDivisionID("DEFAULT");
	}
	if (myTeam == "Seaota" && imyYear == 1997)
	{
		myLeagueID = GetLeagueID("Plano 97 in 99");
		myConfID = GetConferenceID("Division A");
		myDivisionID = GetDivisionID("DEFAULT");
	}
	if (myTeam == "Texaheim" && imyYear == 1997)
	{
		myLeagueID = GetLeagueID("Plano 97 in 99");
		myConfID = GetConferenceID("Division A");
		myDivisionID = GetDivisionID("DEFAULT");
	}
	if (myTeam == "Yorkcity" && imyYear == 1997)
	{
		myLeagueID = GetLeagueID("Plano 97 in 99");
		myConfID = GetConferenceID("Division A");
		myDivisionID = GetDivisionID("DEFAULT");
	}
	if (myTeam == "Cinangeles" && imyYear == 1997)
	{
		myLeagueID = GetLeagueID("Plano 97 in 99");
		myConfID = GetConferenceID("Division B");
		myDivisionID = GetDivisionID("DEFAULT");
	}
	if (myTeam == "Florago" && imyYear == 1997)
	{
		myLeagueID = GetLeagueID("Plano 97 in 99");
		myConfID = GetConferenceID("Division B");
		myDivisionID = GetDivisionID("DEFAULT");
	}
	if (myTeam == "Montrado" && imyYear == 1997)
	{
		myLeagueID = GetLeagueID("Plano 97 in 99");
		myConfID = GetConferenceID("Division B");
		myDivisionID = GetDivisionID("DEFAULT");
	}
	if (myTeam == "NewDiego" && imyYear == 1997)
	{
		myLeagueID = GetLeagueID("Plano 97 in 99");
		myConfID = GetConferenceID("Division B");
		myDivisionID = GetDivisionID("DEFAULT");
	}
	if (myTeam == "Philanta" && imyYear == 1997)
	{
		myLeagueID = GetLeagueID("Plano 97 in 99");
		myConfID = GetConferenceID("Division B");
		myDivisionID = GetDivisionID("DEFAULT");
	}
	if (myTeam == "SanLouis" && imyYear == 1997)
	{
		myLeagueID = GetLeagueID("Plano 97 in 99");
		myConfID = GetConferenceID("Division B");
		myDivisionID = GetDivisionID("DEFAULT");
	}
	myteamID = GetTeamID(myTeam, imyYear, myLeagueID);
	if (myteamID == 0)
		TeamInsert(myTeam, myShortTeam, myBallpark, 0, 0, 0, 0, myLeagueID, myConfID, myDivisionID, imyYear, false);

	// If the team waas just created, we need to get its ID
	myteamStatID = GetTeamID(myTeam, imyYear, myLeagueID);
	myteamID = GetTeamIDBase(myTeam, imyYear, true);

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

		// The following was needed for MS Access search but not for sqlite
		//
		//std::string str1 = lLastName;
		////CT2CA pszConvertedAnsiString(lLastName);
		////std::string str1(pszConvertedAnsiString);
		//if (str1.find('\'', 0) != std::string::npos)
		//{
		//	std::string str2 = str1.substr(0, str1.find('\'', 0));
		//	// Insert the double "'" in the string.
		//	str2 = str2 + '\'' + '\'';
		//	strLastName = str2 + str1.substr((str1.find('\'', 0) + 1), std::string::npos);
		//}
		//else
		//{
		//	strLastName = lLastName;
		//	//strLastName = str1;
		//}

		AfxExtractSubString(myYear, structBatter.GetTeamBatter(myFileName), 0, _T(' '));

		strBatterFirstName = structBatter.m_PlayerName.Right(
			structBatter.m_PlayerName.GetLength() - structBatter.m_PlayerName.Find(_T(", ")) - 2).TrimRight(' ');

		// Create search for duplicate record here.

		// Update the filter which is the WHERE portion to find the player
		// based on name and current team.
		//tmpTeamID.Format("%d", rsTeam.m_TeamID);
		//rsBatter.m_strFilter = "[FirstName] = '" + strBatterFirstName + "'" +
		//	" AND [LastName] = '" + strLastName.c_str() + "'" +
		//	" AND [TeamID] = " + tmpTeamID;
		//CT2CA pszConvertedAnsiString(strBatterFirstName);
		//std::string str3(pszConvertedAnsiString);
		//		myBatterID = GetBatterID(strBatterFirstName, strLastName.c_str(), myteamID);
		myBatterID = GetBatterID(strBatterFirstName, lLastName, myteamID);

		// The following should never happen and this should point to a base team.
		if (!myBatterID)
		{
			// Batter does not exist Therefore add this batter

			BatterInsert(
				strBatterFirstName,
				lLastName,
				structBatter.m_Pitcher,
				structBatter.m_Catcher,
				structBatter.m_FirstBase,
				structBatter.m_SecondBase,
				structBatter.m_ShortStop,
				structBatter.m_ThirdBase,
				structBatter.m_LeftField,
				structBatter.m_CenterField,
				structBatter.m_RightField,
				structBatter.m_bBunting,
				structBatter.m_bHitRun,
				structBatter.m_bRunning,
				structBatter.m_bStealing,
				structBatter.m_bCatchArm,
				structBatter.m_bOutArm,
				structBatter.m_bPowerR,
				structBatter.m_bPowerL,
				structBatter.m_bPass,
				structBatter.m_bTRate,
				structBatter.m_bER1,
				structBatter.m_bER2,
				structBatter.m_bER3,
				structBatter.m_bER4,
				structBatter.m_bER5,
				structBatter.m_bER6,
				structBatter.m_bER7,
				structBatter.m_bER8,
				structBatter.m_bER9,
				structBatter.m_bBatterHits,
				myteamID,
				_ttof(structBatter.m_OBChanceHomeRun),
				_ttof(structBatter.m_OBChanceTriple),
				_ttof(structBatter.m_OBChanceDouble),
				_ttof(structBatter.m_OBChanceSingle),
				_ttof(structBatter.m_OBChanceWalk),
				_ttof(structBatter.m_ChanceDoublePlay),
				_ttof(structBatter.m_OBChanceHomeRunRight),
				_ttof(structBatter.m_OBChanceTripleRight),
				_ttof(structBatter.m_OBChanceDoubleRight),
				_ttof(structBatter.m_OBChanceSingleRight),
				_ttof(structBatter.m_OBChanceWalkRight),
				_ttof(structBatter.m_ChanceDoublePlayRight),
				_ttof(structBatter.m_OBChanceHomeRunLeft),
				_ttof(structBatter.m_OBChanceTripleLeft),
				_ttof(structBatter.m_OBChanceDoubleLeft),
				_ttof(structBatter.m_OBChanceSingleLeft),
				_ttof(structBatter.m_OBChanceWalkLeft),
				_ttof(structBatter.m_ChanceDoublePlayLeft),
				_ttof(structBatter.m_OBChanceBasic),
				_ttof(structBatter.m_OBChanceLeft),
				_ttof(structBatter.m_OBChanceRight)
				);
		}

		//GetLocalTime(&lt);

		// Retrieve the ID of the base batter.
		myBatterID = 0;
		//		myBatterID = GetBatterID(strBatterFirstName, strLastName.c_str(), myteamID);
		myBatterID = GetBatterID(strBatterFirstName, lLastName, myteamID);
		float AVG = 0.0f;
		float SLG = 0.0f;
		float OBP = 0.0f;
		if (structBatter.m_AB == 0)
		{
			AVG = 0.0f;
			SLG = 0.0f;
			OBP = 0.0f;
		}
		else
		{
			AVG = (float)structBatter.m_Hits / structBatter.m_AB;
			SLG = (float)((structBatter.m_Hits - (structBatter.m_2B + structBatter.m_3B + structBatter.m_HomeRuns)) + (2 * structBatter.m_2B) + (3 * structBatter.m_3B) + (4 * structBatter.m_HomeRuns)) / (structBatter.m_AB);
			OBP = (float)(structBatter.m_Hits + structBatter.m_Walk + structBatter.m_ReachedOnError + structBatter.m_Sacrifice + structBatter.m_StolenBase) / (structBatter.m_AB + structBatter.m_Walk + structBatter.m_ReachedOnError + structBatter.m_Sacrifice + structBatter.m_StolenBase);
		}
		// Create test to check for duplicate records here

		if (myBatterID > 0)
		{
			BatterStatsInsert(
				structBatter.m_AB,
				structBatter.m_Runs,
				structBatter.m_Hits,
				structBatter.m_RBI,
				structBatter.m_2B,
				structBatter.m_3B,
				structBatter.m_HomeRuns,
				structBatter.m_Walk,
				structBatter.m_StrikeOut,
				structBatter.m_ReachedOnError,
				structBatter.m_Sacrifice,
				structBatter.m_StolenBase,
				structBatter.m_CS,
				structBatter.m_Games,
				structBatter.m_HBP,
				AVG,
				SLG,
				OBP,
				myBatterID,
				myteamStatID
				);
		}
		else
		{
			// BatterStats already exist.
			//AfxMessageBox(_T("Database Batter is already in DB: ") + strBatterFirstName + _T(" ") + strLastName.c_str());
		}
	}

	// Process Pitcher file
	strTemp = _T("XP") + strTeamName.Left(20);
	exportFileName = strLeagueDir + _T("\\") + strTemp + _T(".txt"); // dir\XB000001.txt
	myFileName = strLeagueDir + _T("\\TP") + strTeamName.Right(10);

	// A Team was selected so export all of the players
	count = structPitcher.GetCountPitcher(myFileName);
	for (i = 0; i<count; i++)
	{
		position = lCountSection + (i*lPitcherSection);
		structPitcher.GetPitcher(myFileName, position);

		//	// When the last name is something like O'Tool, the "'" causes a problem
		//	// with the SQL search. By editing the string to insert a double "'"
		//	// in the search, the search works correctly.
		lLastName = structPitcher.m_PitcherName.Left(structPitcher.m_PitcherName.Find(','));
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

		strPitcherFirstName = structPitcher.m_PitcherName.Right(
			structPitcher.m_PitcherName.GetLength() - structPitcher.m_PitcherName.Find(_T(", ")) - 2).TrimRight(' ');

		// Create search for duplicate record here.
		myPitcherID = 0;
		myPitcherID = GetPitcherID(strPitcherFirstName, lLastName, myteamID);

		if (!myPitcherID)
		{
			// Batter does not exist Therefore add this batter
			PitcherInsert(
				strPitcherFirstName,
				lLastName,
				_ttof(structPitcher.m_OBChanceHomeRun),
				_ttof(structPitcher.m_OBChanceTriple),
				_ttof(structPitcher.m_OBChanceDouble),
				_ttof(structPitcher.m_OBChanceSingle),
				_ttof(structPitcher.m_OBChanceWalk),
				_ttof(structPitcher.m_ChanceDoublePlay),
				_ttof(structPitcher.m_OBChanceHomeRunRight),
				_ttof(structPitcher.m_OBChanceTripleRight),
				_ttof(structPitcher.m_OBChanceDoubleRight),
				_ttof(structPitcher.m_OBChanceSingleRight),
				_ttof(structPitcher.m_OBChanceWalkRight),
				_ttof(structPitcher.m_ChanceDoublePlayRight),
				_ttof(structPitcher.m_OBChanceHomeRunLeft),
				_ttof(structPitcher.m_OBChanceTripleLeft),
				_ttof(structPitcher.m_OBChanceDoubleLeft),
				_ttof(structPitcher.m_OBChanceSingleLeft),
				_ttof(structPitcher.m_OBChanceWalkLeft),
				_ttof(structPitcher.m_ChanceDoublePlayLeft),
				_ttof(structPitcher.m_OBChanceBasic),
				_ttof(structPitcher.m_OBChanceLeft),
				_ttof(structPitcher.m_OBChanceRight),
				structPitcher.m_Starter,
				structPitcher.m_Relief,
				structPitcher.m_Throws,
				structPitcher.m_Bunting,
				structPitcher.m_Hold,
				structPitcher.m_bWP,
				structPitcher.m_bBalk,
				structPitcher.m_PitcherField,
				structPitcher.m_bER1,
				myteamID
				);
		}


		// Retrieve the ID of the base pitcher.
		myPitcherID = GetPitcherID(strPitcherFirstName, lLastName, myteamID);

		float ERA = 0.0f;
		float WHIP = 0.0f;
		if (_ttof(structPitcher.m_IP) == 0)
		{
			ERA = 0.0f;
			WHIP = 0.0f;
		}
		else
		{
			ERA = (float)(structPitcher.m_ER * 9) / (float)_ttof(structPitcher.m_IP);
			WHIP = (float)((structPitcher.m_Hits + structPitcher.m_Walks) * 9) / (float)_ttof(structPitcher.m_IP);
		}


		if (myPitcherID > 0)
		{
			// PitcherID always points back to initial Pitcher for fixed statistics
			// TeamID can point to any team as this connects the statistics
			// that change based on actual play.
			PitcherStatsInsert(
				structPitcher.m_Wins,
				structPitcher.m_Loss,
				structPitcher.m_Saves,
				(float)_ttof(structPitcher.m_IP),
				structPitcher.m_ER,
				structPitcher.m_Hits,
				structPitcher.m_Walks,
				structPitcher.m_Strikeouts,
				structPitcher.m_HomeRuns,
				structPitcher.m_Games,
				structPitcher.m_CompGames,
				structPitcher.m_Starts,
				ERA,
				WHIP,
				myPitcherID,
				myteamStatID
				);
		}
		else
		{
			// PitcherStats already exist.
			//AfxMessageBox("Database Pitcher is already in DB: " + strPitcherFirstName + " " + strLastName.c_str());
		}
	}
}


void CStratOMaticSqLiteDoc::ExportBaseFileToDB(CString strDir, CString strTeamName)
{
	SYSTEMTIME lt;
	CString SqlString;
	CStringA lLastName;
	std::string strLastName;
	BatterStruct structBatter;
	PitcherStruct structPitcher;
	CString tmpTeamID;
	CStringA myTeam;
	CString strDivision;
	//CString strTempTeam;
	CStringA myShortTeam;
	CStringA myBallpark;
	CString myYear;
	int imyYear;
	CStringA strBatterFirstName;
	CStringA strPitcherFirstName;
	BYTE count;
	CFile exportBatter;
	CFile exportPitcher;
	CString exportFileName;
	CString strexportData;
	int i;
	CString strTemp;
	CString myFileName;
	LONG lTeamSection = 74;
	LONG lCountSection = 1;
	LONG lPlayerSection = structBatter.m_RecordSize;
	LONG lPitcherSection = structPitcher.m_RecordSize;
	LONG position;
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
	int myteamID = 0;
	int myLeagueID = 0;
	int myConfID = 0;
	int myDivisionID = 0;
	int myBatterID = 0;
	int myPitcherID = 0;
	count = 0;

	// Process Batter file
	strTemp = _T("XB") + strTeamName.Left(20);
	exportFileName = strDir + _T("\\") + strTemp + _T(".txt"); // dir\XB000001.txt
	myFileName = strDir + _T("\\TB") + strTeamName.Right(10);

	// Process Team entry
	// Skip "1965 " to start with the actual team name.
	//AfxExtractSubString(myTeam, structBatter.GetTeamBatter(myFileName), 1, _T(' '));
	myTeam = structBatter.GetTeamBatter(myFileName).Mid(5).TrimRight(' ');
	myShortTeam = structBatter.GetShortTeamBatter(myFileName);
	myBallpark = structBatter.GetBallparkBatter(myFileName);
	AfxExtractSubString(myYear, structBatter.GetTeamBatter(myFileName), 0, _T(' '));
	imyYear = _ttoi(myYear);

	// Determine if the team exists in DB.
	myteamID = GetTeamID(myTeam, imyYear);
	if (myteamID == 0)
	{
		if (myTeam == "Chicago White Sox" && imyYear == 1965)
		{
			myLeagueID = GetLeagueID("Base1965");
			myConfID = GetConferenceID("BaseAL1965");
			myDivisionID = GetDivisionID("DEFAULT");
		}
		if (myTeam == "Cleveland Indians" && imyYear == 1965)
		{
			myLeagueID = GetLeagueID("Base1965");
			myConfID = GetConferenceID("BaseAL1965");
			myDivisionID = GetDivisionID("DEFAULT");
		}
		if (myTeam == "Minnesota Twins" && imyYear == 1965)
		{
			myLeagueID = GetLeagueID("Base1965");
			myConfID = GetConferenceID("BaseAL1965");
			myDivisionID = GetDivisionID("DEFAULT");
		}
		// Extra team for some reason
		if (myTeam == "Baltimore" && imyYear == 1965)
		{
			myLeagueID = GetLeagueID("Base1965");
			myConfID = GetConferenceID("BaseAL1965");
			myDivisionID = GetDivisionID("DEFAULT");
		}
		if (myTeam == "Cincinnati Reds" && imyYear == 1965)
		{
			myLeagueID = GetLeagueID("Base1965");
			myConfID = GetConferenceID("BaseNL1965");
			myDivisionID = GetDivisionID("DEFAULT");
		}
		if (myTeam == "Los Angeles Dodgers" && imyYear == 1965)
		{
			myLeagueID = GetLeagueID("Base1965");
			myConfID = GetConferenceID("BaseNL1965");
			myDivisionID = GetDivisionID("DEFAULT");
		}
		if (myTeam == "San Francisco Giants" && imyYear == 1965)
		{
			myLeagueID = GetLeagueID("Base1965");
			myConfID = GetConferenceID("BaseNL1965");
			myDivisionID = GetDivisionID("DEFAULT");
		}
		if (myTeam == "Baltimore Orioles" && imyYear == 1969)
		{
			myLeagueID = GetLeagueID("Base1969");
			myConfID = GetConferenceID("BaseAL1969");
			myDivisionID = GetDivisionID("BaseALEast1969");
		}
		if (myTeam == "Seattle Pilots" && imyYear == 1969)
		{
			myLeagueID = GetLeagueID("Base1969");
			myConfID = GetConferenceID("BaseAL1969");
			myDivisionID = GetDivisionID("BaseALWest1969");
		}
		if (myTeam == "Washington Senators" && imyYear == 1969)
		{
			myLeagueID = GetLeagueID("Base1969");
			myConfID = GetConferenceID("BaseAL1969");
			myDivisionID = GetDivisionID("BaseALEast1969");
		}
		if (myTeam == "Houston Astros" && imyYear == 1969)
		{
			myLeagueID = GetLeagueID("Base1969");
			myConfID = GetConferenceID("BaseNL1969");
			myDivisionID = GetDivisionID("BaseNLWest1969");
		}
		if (myTeam == "Pittsburg Pirates" && imyYear == 1969)
		{
			myLeagueID = GetLeagueID("Base1969");
			myConfID = GetConferenceID("BaseNL1969");
			myDivisionID = GetDivisionID("BaseNLEast1969");
		}
		if (myTeam == "San Diego Padres" && imyYear == 1969)
		{
			myLeagueID = GetLeagueID("Base1969");
			myConfID = GetConferenceID("BaseNL1969");
			myDivisionID = GetDivisionID("BaseNLWest1969");
		}
		if (myTeam == "Cleveland" && imyYear == 1997)
		{
			myLeagueID = GetLeagueID("Base1997");
			myConfID = GetConferenceID("BaseAL1997");
			myDivisionID = GetDivisionID("BaseALCentral1997");
		}
		if (myTeam == "Florida" && imyYear == 1997)
		{
			myLeagueID = GetLeagueID("Base1997");
			myConfID = GetConferenceID("BaseNL1997");
			myDivisionID = GetDivisionID("BaseNLEast1997");
		}
		if (myTeam == "Anaheim" && imyYear == 1997)
		{
			myLeagueID = GetLeagueID("Base1997");
			myConfID = GetConferenceID("BaseAL1997");
			myDivisionID = GetDivisionID("BaseALWest1997");
		}
		if (myTeam == "Baltimore" && imyYear == 1997)
		{
			myLeagueID = GetLeagueID("Base1997");
			myConfID = GetConferenceID("BaseAL1997");
			myDivisionID = GetDivisionID("BaseALEast1997");
		}
		if (myTeam == "Baltland" && imyYear == 1997)
		{
			myLeagueID = GetLeagueID("Base Plano 97 in 99");
			myConfID = GetConferenceID("Base Division A");
			myDivisionID = GetDivisionID("DEFAULT");
		}
		if (myTeam == "Bostago" && imyYear == 1997)
		{
			myLeagueID = GetLeagueID("Base Plano 97 in 99");
			myConfID = GetConferenceID("Base Division A");
			myDivisionID = GetDivisionID("DEFAULT");
		}
		if (myTeam == "Cleonto" && imyYear == 1997)
		{
			myLeagueID = GetLeagueID("Base Plano 97 in 99");
			myConfID = GetConferenceID("Base Division A");
			myDivisionID = GetDivisionID("DEFAULT");
		}
		if (myTeam == "Seaota" && imyYear == 1997)
		{
			myLeagueID = GetLeagueID("Base Plano 97 in 99");
			myConfID = GetConferenceID("Base Division A");
			myDivisionID = GetDivisionID("DEFAULT");
		}
		if (myTeam == "Texaheim" && imyYear == 1997)
		{
			myLeagueID = GetLeagueID("Base Plano 97 in 99");
			myConfID = GetConferenceID("Base Division A");
			myDivisionID = GetDivisionID("DEFAULT");
		}
		if (myTeam == "Yorkcity" && imyYear == 1997)
		{
			myLeagueID = GetLeagueID("Base Plano 97 in 99");
			myConfID = GetConferenceID("Base Division A");
			myDivisionID = GetDivisionID("DEFAULT");
		}
		if (myTeam == "Cinangeles" && imyYear == 1997)
		{
			myLeagueID = GetLeagueID("Base Plano 97 in 99");
			myConfID = GetConferenceID("Base Division B");
			myDivisionID = GetDivisionID("DEFAULT");
		}
		if (myTeam == "Florago" && imyYear == 1997)
		{
			myLeagueID = GetLeagueID("Base Plano 97 in 99");
			myConfID = GetConferenceID("Base Division B");
			myDivisionID = GetDivisionID("DEFAULT");
		}
		if (myTeam == "Montrado" && imyYear == 1997)
		{
			myLeagueID = GetLeagueID("Base Plano 97 in 99");
			myConfID = GetConferenceID("Base Division B");
			myDivisionID = GetDivisionID("DEFAULT");
		}
		if (myTeam == "NewDiego" && imyYear == 1997)
		{
			myLeagueID = GetLeagueID("Base Plano 97 in 99");
			myConfID = GetConferenceID("Base Division B");
			myDivisionID = GetDivisionID("DEFAULT");
		}
		if (myTeam == "Philanta" && imyYear == 1997)
		{
			myLeagueID = GetLeagueID("Base Plano 97 in 99");
			myConfID = GetConferenceID("Base Division B");
			myDivisionID = GetDivisionID("DEFAULT");
		}
		if (myTeam == "SanLouis" && imyYear == 1997)
		{
			myLeagueID = GetLeagueID("Base Plano 97 in 99");
			myConfID = GetConferenceID("Base Division B");
			myDivisionID = GetDivisionID("DEFAULT");
		}
		TeamInsert(myTeam, myShortTeam, myBallpark, 0, 0, 0, 0, myLeagueID, myConfID, myDivisionID, imyYear, true);
	}

	// If the team waas just created, we need to get its ID
	myteamID = GetTeamID(myTeam, imyYear);

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

		// The following was needed for MS Access search but not for sqlite
		//
		//std::string str1 = lLastName;
		////CT2CA pszConvertedAnsiString(lLastName);
		////std::string str1(pszConvertedAnsiString);
		//if (str1.find('\'', 0) != std::string::npos)
		//{
		//	std::string str2 = str1.substr(0, str1.find('\'', 0));
		//	// Insert the double "'" in the string.
		//	str2 = str2 + '\'' + '\'';
		//	strLastName = str2 + str1.substr((str1.find('\'', 0) + 1), std::string::npos);
		//}
		//else
		//{
		//	strLastName = lLastName;
		//	//strLastName = str1;
		//}

		AfxExtractSubString(myYear, structBatter.GetTeamBatter(myFileName), 0, _T(' '));

		strBatterFirstName = structBatter.m_PlayerName.Right(
			structBatter.m_PlayerName.GetLength() - structBatter.m_PlayerName.Find(_T(", ")) - 2).TrimRight(' ');

		// Create search for duplicate record here.

		// Update the filter which is the WHERE portion to find the player
		// based on name and current team.
		//tmpTeamID.Format("%d", rsTeam.m_TeamID);
		//rsBatter.m_strFilter = "[FirstName] = '" + strBatterFirstName + "'" +
		//	" AND [LastName] = '" + strLastName.c_str() + "'" +
		//	" AND [TeamID] = " + tmpTeamID;
		//CT2CA pszConvertedAnsiString(strBatterFirstName);
		//std::string str3(pszConvertedAnsiString);
//		myBatterID = GetBatterID(strBatterFirstName, strLastName.c_str(), myteamID);
		myBatterID = GetBatterID(strBatterFirstName, lLastName, myteamID);

		// Execute the query
		//rsBatter.Requery();
		// RowSetSize defaults to 1 so 1 or more matched rows will always result in 1
		if (!myBatterID)
		{
			// Batter does not exist Therefore add this batter

			BatterInsert(
				strBatterFirstName,
				lLastName,
				structBatter.m_Pitcher,
				structBatter.m_Catcher,
				structBatter.m_FirstBase,
				structBatter.m_SecondBase,
				structBatter.m_ShortStop,
				structBatter.m_ThirdBase,
				structBatter.m_LeftField,
				structBatter.m_CenterField,
				structBatter.m_RightField,
				structBatter.m_bBunting,
				structBatter.m_bHitRun,
				structBatter.m_bRunning,
				structBatter.m_bStealing,
				structBatter.m_bCatchArm,
				structBatter.m_bOutArm,
				structBatter.m_bPowerR,
				structBatter.m_bPowerL,
				structBatter.m_bPass,
				structBatter.m_bTRate,
				structBatter.m_bER1,
				structBatter.m_bER2,
				structBatter.m_bER3,
				structBatter.m_bER4,
				structBatter.m_bER5,
				structBatter.m_bER6,
				structBatter.m_bER7,
				structBatter.m_bER8,
				structBatter.m_bER9,
				structBatter.m_bBatterHits,
				myteamID,
				_ttof(structBatter.m_OBChanceHomeRun),
				_ttof(structBatter.m_OBChanceTriple),
				_ttof(structBatter.m_OBChanceDouble),
				_ttof(structBatter.m_OBChanceSingle),
				_ttof(structBatter.m_OBChanceWalk),
				_ttof(structBatter.m_ChanceDoublePlay),
				_ttof(structBatter.m_OBChanceHomeRunRight),
				_ttof(structBatter.m_OBChanceTripleRight),
				_ttof(structBatter.m_OBChanceDoubleRight),
				_ttof(structBatter.m_OBChanceSingleRight),
				_ttof(structBatter.m_OBChanceWalkRight),
				_ttof(structBatter.m_ChanceDoublePlayRight),
				_ttof(structBatter.m_OBChanceHomeRunLeft),
				_ttof(structBatter.m_OBChanceTripleLeft),
				_ttof(structBatter.m_OBChanceDoubleLeft),
				_ttof(structBatter.m_OBChanceSingleLeft),
				_ttof(structBatter.m_OBChanceWalkLeft),
				_ttof(structBatter.m_ChanceDoublePlayLeft),
				_ttof(structBatter.m_OBChanceBasic),
				_ttof(structBatter.m_OBChanceLeft),
				_ttof(structBatter.m_OBChanceRight)
				);
		}

			//GetLocalTime(&lt);

		// Retrieve the ID of the batter that was just inserted.
		myBatterID = 0;
//		myBatterID = GetBatterID(strBatterFirstName, strLastName.c_str(), myteamID);
		myBatterID = GetBatterID(strBatterFirstName, lLastName, myteamID);
		float AVG = 0.0f;
		float SLG = 0.0f;
		float OBP = 0.0f;
		if (structBatter.m_AB == 0)
		{
			AVG = 0.0f;
			SLG = 0.0f;
			OBP = 0.0f;
		}
		else
		{
			AVG = (float)structBatter.m_Hits / structBatter.m_AB;
			SLG = (float)((structBatter.m_Hits - (structBatter.m_2B + structBatter.m_3B + structBatter.m_HomeRuns)) + (2 * structBatter.m_2B) + (3 * structBatter.m_3B) + (4 * structBatter.m_HomeRuns)) / (structBatter.m_AB);
			OBP = (float)(structBatter.m_Hits + structBatter.m_Walk + structBatter.m_ReachedOnError + structBatter.m_Sacrifice + structBatter.m_StolenBase) / (structBatter.m_AB + structBatter.m_Walk + structBatter.m_ReachedOnError + structBatter.m_Sacrifice + structBatter.m_StolenBase);
		}
		// Create test to check for duplicate records here

		if (myBatterID > 0)
		{
			BatterStatsInsert(
				structBatter.m_AB,
				structBatter.m_Runs,
				structBatter.m_Hits,
				structBatter.m_RBI,
				structBatter.m_2B,
				structBatter.m_3B,
				structBatter.m_HomeRuns,
				structBatter.m_Walk,
				structBatter.m_StrikeOut,
				structBatter.m_ReachedOnError,
				structBatter.m_Sacrifice,
				structBatter.m_StolenBase,
				structBatter.m_CS,
				structBatter.m_Games,
				structBatter.m_HBP,
				AVG,
				SLG,
				OBP,
				myBatterID,
				myteamID
			);
		}
		else
		{
			// Batter already exists.
			//AfxMessageBox(_T("Database Batter is already in DB: ") + strBatterFirstName + _T(" ") + strLastName.c_str());
		}
	}

	// Process Pitcher file
	strTemp = _T("XP") + strTeamName.Left(20);
	exportFileName = strDir + _T("\\") + strTemp + _T(".txt"); // dir\XB000001.txt
	myFileName = strDir + _T("\\TP") + strTeamName.Right(10);

	// A Team was selected so export all of the players
	count = structPitcher.GetCountPitcher(myFileName);
	for (i = 0; i<count; i++)
	{
		position = lCountSection + (i*lPitcherSection);
		structPitcher.GetPitcher(myFileName, position);

	//	// When the last name is something like O'Tool, the "'" causes a problem
	//	// with the SQL search. By editing the string to insert a double "'"
	//	// in the search, the search works correctly.
		lLastName = structPitcher.m_PitcherName.Left(structPitcher.m_PitcherName.Find(','));
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

		strPitcherFirstName = structPitcher.m_PitcherName.Right(
			structPitcher.m_PitcherName.GetLength() - structPitcher.m_PitcherName.Find(_T(", ")) - 2).TrimRight(' ');

		// Create search for duplicate record here.
		myPitcherID = 0;
		myPitcherID = GetPitcherID(strPitcherFirstName, lLastName, myteamID);

		if (!myPitcherID)
		{
			// Batter does not exist Therefore add this batter
			PitcherInsert(
				strPitcherFirstName,
				lLastName,
				_ttof(structPitcher.m_OBChanceHomeRun),
				_ttof(structPitcher.m_OBChanceTriple),
				_ttof(structPitcher.m_OBChanceDouble),
				_ttof(structPitcher.m_OBChanceSingle),
				_ttof(structPitcher.m_OBChanceWalk),
				_ttof(structPitcher.m_ChanceDoublePlay),
				_ttof(structPitcher.m_OBChanceHomeRunRight),
				_ttof(structPitcher.m_OBChanceTripleRight),
				_ttof(structPitcher.m_OBChanceDoubleRight),
				_ttof(structPitcher.m_OBChanceSingleRight),
				_ttof(structPitcher.m_OBChanceWalkRight),
				_ttof(structPitcher.m_ChanceDoublePlayRight),
				_ttof(structPitcher.m_OBChanceHomeRunLeft),
				_ttof(structPitcher.m_OBChanceTripleLeft),
				_ttof(structPitcher.m_OBChanceDoubleLeft),
				_ttof(structPitcher.m_OBChanceSingleLeft),
				_ttof(structPitcher.m_OBChanceWalkLeft),
				_ttof(structPitcher.m_ChanceDoublePlayLeft),
				_ttof(structPitcher.m_OBChanceBasic),
				_ttof(structPitcher.m_OBChanceLeft),
				_ttof(structPitcher.m_OBChanceRight),
				structPitcher.m_Starter,
				structPitcher.m_Relief,
				structPitcher.m_Throws,
				structPitcher.m_Bunting,
				structPitcher.m_Hold,
				structPitcher.m_bWP,
				structPitcher.m_bBalk,
				structPitcher.m_PitcherField,
				structPitcher.m_bER1,
				myteamID
				);
		}


		// Retrieve the ID of the pitcher that was just inserted.
		myPitcherID = GetPitcherID(strPitcherFirstName, lLastName, myteamID);

		float ERA = 0.0f;
		float WHIP = 0.0f;
		if (_ttof(structPitcher.m_IP) == 0)
		{
			ERA = 0.0f;
			WHIP = 0.0f;
		}
		else
		{
			ERA = (float)(structPitcher.m_ER * 9) / (float)_ttof(structPitcher.m_IP);
			WHIP = (float)((structPitcher.m_Hits + structPitcher.m_Walks) * 9) / (float)_ttof(structPitcher.m_IP);
		}


		if (myPitcherID > 0)
		{
			// PitcherID always points back to initial Pitcher for fixed statistics
			// TeamID can point to any team as this connects the statistics
			// that change based on actual play.
			PitcherStatsInsert(
				structPitcher.m_Wins,
				structPitcher.m_Loss,
				structPitcher.m_Saves,
				(float)_ttof(structPitcher.m_IP),
				structPitcher.m_ER,
				structPitcher.m_Hits,
				structPitcher.m_Walks,
				structPitcher.m_Strikeouts,
				structPitcher.m_HomeRuns,
				structPitcher.m_Games,
				structPitcher.m_CompGames,
				structPitcher.m_Starts,
				ERA,
				WHIP,
				myPitcherID,
				myteamID
				);
		}
		else
		{
			// Pitcher already exists.
			//AfxMessageBox("Database Pitcher is already in DB: " + strPitcherFirstName + " " + strLastName.c_str());
		}
	}
}


void CStratOMaticSqLiteDoc::OnSqlInsertDefaultLeague()
{
	// TODO: Add your command handler code here
	int myLeagueID = 0;
	int myConfID = 0;

	LeagueInsert("DEFAULT", 1, 1, true, 1800);
	LeagueInsert("Base1965", 2, 0, true, 1965);
	LeagueInsert("Base1969", 2, 4, true, 1969);
	LeagueInsert("Base1997", 2, 6, true, 1997);
	LeagueInsert("Base Plano 97 in 99", 2, 0, true, 1999);
	LeagueInsert("The Gnews 1998", 2, 0, false, 1998);
	LeagueInsert("Plano 97 in 99", 2, 0, false, 1999);

	// Select the LeagueId
	myLeagueID = GetLeagueID("DEFAULT");
	ConferenceInsert("DEFAULT", myLeagueID, true);
	myConfID = GetConferenceID("DEFAULT");
	DivisionInsert("DEFAULT", myLeagueID, myConfID, true);

	myLeagueID = 0;
	myConfID = 0;

	// Select the LeagueId
	myLeagueID = GetLeagueID("Base1965");

	if (myLeagueID != 0)
	{
		ConferenceInsert("BaseAL1965", myLeagueID, true);
		ConferenceInsert("BaseNL1965", myLeagueID, true);
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

	myLeagueID = 0;
	myConfID = 0;

	// Select the LeagueId
	myLeagueID = GetLeagueID("Base1997");

	if (myLeagueID != 0)
	{
		ConferenceInsert("BaseAL1997", myLeagueID, true);
		ConferenceInsert("BaseNL1997", myLeagueID, true);
		myConfID = GetConferenceID("BaseAL1997");
		if (myConfID != 0)
		{
			DivisionInsert("BaseALEast1997", myLeagueID, myConfID, true);
			DivisionInsert("BaseALWest1997", myLeagueID, myConfID, true);
			DivisionInsert("BaseALCentral1997", myLeagueID, myConfID, true);
		}
		myConfID = 0;
		myConfID = GetConferenceID("BaseNL1997");
		if (myConfID != 0)
		{
			DivisionInsert("BaseNLEast1997", myLeagueID, myConfID, true);
			DivisionInsert("BaseNLWest1997", myLeagueID, myConfID, true);
			DivisionInsert("BaseNLCentral1997", myLeagueID, myConfID, true);
		}
	}

	myLeagueID = 0;
	myConfID = 0;

	// Select the LeagueId
	myLeagueID = GetLeagueID("Base Plano 97 in 99");

	if (myLeagueID != 0)
	{
		ConferenceInsert("Base Division A", myLeagueID, true);
		ConferenceInsert("Base Division B", myLeagueID, true);
	}

	myLeagueID = 0;
	myConfID = 0;

	// Select the LeagueId
	myLeagueID = GetLeagueID("The Gnews 1998");

	if (myLeagueID != 0)
	{
		ConferenceInsert("65 Conference", myLeagueID, false);
		ConferenceInsert("69 Conference", myLeagueID, false);
	}

	myLeagueID = 0;
	myConfID = 0;

	// Select the LeagueId
	myLeagueID = GetLeagueID("Plano 97 in 99");

	if (myLeagueID != 0)
	{
		ConferenceInsert("Division A", myLeagueID, false);
		ConferenceInsert("Division B", myLeagueID, false);
	}
}


int CStratOMaticSqLiteDoc::LeagueInsert(CStringA strName, int NumberConf, int NumDivisions, bool Base, int Year)
{
	char *sqlLeague;
	int rc;
	CHAR buffer[100];

	/* Create SQL statement */
	sqlLeague = "INSERT INTO LEAGUES("  \
		"LeagueName," \
		"NumberOfConferences," \
		"NumberOfDivisions," \
		"BaseLeague," \
		"LeagueYear"
		")" \
		"VALUES (" \
		"?1," \
		"?2," \
		"?3," \
		"?4," \
		"?5"
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
	rc = sqlite3_bind_int(m_stmt, 5, Year);
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

int CStratOMaticSqLiteDoc::GetTeamID(CStringA strTeamName, int Year, int LeagueID)
{
	int myTeamId = 0;
	int rc;
	CHAR buffer[100];
	char *sqlSelect;

	sqlSelect = "SELECT TeamId from TEAM WHERE TeamName = ?1 and TeamYear = ?2 and LeagueID = ?3";

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
	rc = sqlite3_bind_int(m_stmt, 3, LeagueID);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind LeagueID: %s\n", sqlite3_errmsg(m_db));
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

int CStratOMaticSqLiteDoc::GetTeamIDBase(CStringA strTeamName, int Year, bool Base)
{
	int myTeamId = 0;
	int rc;
	CHAR buffer[100];
	char *sqlSelect;

	sqlSelect = "SELECT TeamId from TEAM WHERE TeamName = ?1 and TeamYear = ?2 and BaseTeam = ?3";

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
	rc = sqlite3_bind_int(m_stmt, 3, Base);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind Base: %s\n", sqlite3_errmsg(m_db));
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
		sprintf_s(buffer, sizeof(buffer), "Could not bind strFirstName: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_text(m_stmt, 2, strLastName, strlen(strLastName), SQLITE_STATIC);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind strLastName: %s\n", sqlite3_errmsg(m_db));
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



int CStratOMaticSqLiteDoc::BatterStatsInsert(int AB, int Runs, int Hits, int RBI, int Doubles, int Triples, int HomeRuns, int Walk, 
	int Strikout, int ReachedOnError, int Sacrifice, int StolenBase, int CS, int Games, int HBP, float AVG, float SLG, float OBP, 
	int BatterID, int TeamID)
{
	int rc;
	CHAR buffer[100];
	char *sqlBatterStats;

	/* Create SQL statement */
	sqlBatterStats = "INSERT INTO BATTERSTATS("  \
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
		"OBP," \
		"BatterID," \
		"TeamID" \
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
		"?20" \
		");";

	rc = sqlite3_prepare_v2(m_db, sqlBatterStats, strlen(sqlBatterStats), &m_stmt, 0);
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
	rc = sqlite3_bind_int(m_stmt, 1, AB);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 2, Runs);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 3, Hits);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 4, RBI);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 5, Doubles);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 6, Triples);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 7, HomeRuns);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 8, Walk);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 9, Strikout);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 10, ReachedOnError);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 11, Sacrifice);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 12, StolenBase);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 13, CS);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 14, Games);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 15, HBP);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_double(m_stmt, 16, AVG);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_double(m_stmt, 17, SLG);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_double(m_stmt, 18, OBP);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 19, BatterID);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 20, TeamID);
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
		sprintf_s(buffer, sizeof(buffer), "Execute for BatterStats Insert OK.\n");
		AddToLog(buffer);
	}

	sqlite3_finalize(m_stmt);
	return 0;
}


int CStratOMaticSqLiteDoc::GetBatterID(CStringA strFirstName, CStringA strLastName)
{
	int rc;
	CHAR buffer[100];
	char *sqlSelect;
	int myBatterID = 0;
	sqlSelect = "SELECT BatterID from BATTER WHERE FirstName = ?1 and LastName = ?2";

	rc = sqlite3_prepare_v2(m_db, sqlSelect, strlen(sqlSelect), &m_stmt, 0);

	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Failed to fetch data: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}

	// Bind the data to field '1' which is the first '?' in the SELECT statement
	rc = sqlite3_bind_text(m_stmt, 1, strFirstName, strlen(strFirstName), SQLITE_STATIC);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind strFirstName: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_text(m_stmt, 2, strLastName, strlen(strLastName), SQLITE_STATIC);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind strLastName: %s\n", sqlite3_errmsg(m_db));
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

	return myBatterID;
}

int CStratOMaticSqLiteDoc::GetBatterID(CStringA strFirstName, CStringA strLastName, int TeamID)
{
	int rc;
	CHAR buffer[100];
	char *sqlSelect;
	int myBatterID = 0;
//	sqlSelect = "SELECT BatterID from BATTER AS B JOIN TEAM AS T ON C.TeamID=T.TeamID WHERE FirstName = ?1 and LastName = ?2 and TeamID = ?3";
	sqlSelect = "SELECT BatterID from BATTER WHERE FirstName = ?1 and LastName = ?2 and TeamID = ?3";

	rc = sqlite3_prepare_v2(m_db, sqlSelect, strlen(sqlSelect), &m_stmt, 0);

	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Failed to fetch data: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}

	// Bind the data to field '1' which is the first '?' in the SELECT statement
	rc = sqlite3_bind_text(m_stmt, 1, strFirstName, strlen(strFirstName), SQLITE_STATIC);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind strFirstName: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_text(m_stmt, 2, strLastName, strlen(strLastName), SQLITE_STATIC);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind strLastName: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 3, TeamID);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind TeamID: %s\n", sqlite3_errmsg(m_db));
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

	return myBatterID;
}

int CStratOMaticSqLiteDoc::GetPitcherID(CStringA strFirstName, CStringA strLastName)
{
	int rc;
	CHAR buffer[100];
	char *sqlSelect;
	int myPitcherID = 0;

	sqlSelect = "SELECT PitcherID from PITCHER WHERE FirstName = ?1 and LastName = ?2";

	rc = sqlite3_prepare_v2(m_db, sqlSelect, strlen(sqlSelect), &m_stmt, 0);

	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Failed to fetch data: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}

	// Bind the data to field '1' which is the first '?' in the SELECT statement
	rc = sqlite3_bind_text(m_stmt, 1, strFirstName, strlen(strFirstName), SQLITE_STATIC);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind strFirstName: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_text(m_stmt, 2, strLastName, strlen(strLastName), SQLITE_STATIC);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind strLastName: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}

	rc = sqlite3_step(m_stmt);

	if (rc == SQLITE_ROW)
	{
		sprintf_s(buffer, sizeof(buffer), "%s  %i\n", sqlite3_column_name(m_stmt, 0), sqlite3_column_int(m_stmt, 0));
		AddToLog(buffer);
		myPitcherID = sqlite3_column_int(m_stmt, 0);
	}
	else
	{
		sprintf_s(buffer, sizeof(buffer), "Select returned nothing: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}

	sqlite3_finalize(m_stmt);

	return myPitcherID;
}

int CStratOMaticSqLiteDoc::GetPitcherID(CStringA strFirstName, CStringA strLastName, int TeamID)
{
	int rc;
	CHAR buffer[100];
	char *sqlSelect;
	int myPitcherID = 0;

	sqlSelect = "SELECT PitcherID from PITCHER WHERE FirstName = ?1 and LastName = ?2 and TeamID = ?3";

	rc = sqlite3_prepare_v2(m_db, sqlSelect, strlen(sqlSelect), &m_stmt, 0);

	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Failed to fetch data: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}

	// Bind the data to field '1' which is the first '?' in the SELECT statement
	rc = sqlite3_bind_text(m_stmt, 1, strFirstName, strlen(strFirstName), SQLITE_STATIC);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind strFirstName: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_text(m_stmt, 2, strLastName, strlen(strLastName), SQLITE_STATIC);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind strLastName: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 3, TeamID);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind TeamID: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}

	rc = sqlite3_step(m_stmt);

	if (rc == SQLITE_ROW)
	{
		sprintf_s(buffer, sizeof(buffer), "%s  %i\n", sqlite3_column_name(m_stmt, 0), sqlite3_column_int(m_stmt, 0));
		AddToLog(buffer);
		myPitcherID = sqlite3_column_int(m_stmt, 0);
	}
	else
	{
		sprintf_s(buffer, sizeof(buffer), "Select returned nothing: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}

	sqlite3_finalize(m_stmt);

	return myPitcherID;
}


int CStratOMaticSqLiteDoc::PitcherInsert(CStringA strFirstName, CStringA strLastName, float OBChanceHomeRun, float OBChanceTriple, 
	float OBChanceDouble, float OBChanceSingle, float OBChanceWalk, float ChanceDoublePlay, float OBChanceHomeRunRight, 
	float OBChanceTripleRight, float OBChanceDoubleRight, float OBChanceSingleRight, float OBChanceWalkRight, float ChanceDoublePlayRight, 
	float OBChanceHomeRunLeft, float OBChanceTripleLeft, float OBChanceDoubleLeft, float OBChanceSingleLeft, float OBChanceWalkLeft, 
	float ChanceDoublePlayLeft, float OBChanceBasic, float OBChanceLeft, float OBChanceRight, int Starter, int Relief, int Throws, 
	int Bunting, int Hold, int WP, int Balk, int Pitcher, int ER1, int TeamID)
{
	int rc;
	CHAR buffer[100];
	char *sqlPitcher;

	/* Create SQL statement */
	sqlPitcher = "INSERT INTO PITCHER("  \
		"FirstName," \
		"LastName," \
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
		"OBChanceRight," \
		"Starter," \
		"Relief," \
		"Throws," \
		"Bunting," \
		"Hold," \
		"WP," \
		"Balk," \
		"Pitcher," \
		"ER1," \
		"TeamID" \
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
		"?33"  \
		");";

		rc = sqlite3_prepare_v2(m_db, sqlPitcher, strlen(sqlPitcher), &m_stmt, 0);
	if (rc != SQLITE_OK)
	{

		//fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(m_db));
		sprintf_s(buffer, sizeof(buffer), "Failed to fetch data: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	else
	{
		sprintf_s(buffer, sizeof(buffer), "Prepare for Pitcher Insert OK: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}

	// Bind the data to field '1' which is the first '?' in the INSERT statement
	rc = sqlite3_bind_text(m_stmt, 1, strFirstName, strlen(strFirstName), SQLITE_STATIC);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind strFirstName: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_text(m_stmt, 2, strLastName, strlen(strLastName), SQLITE_STATIC);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind strLastName: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_double(m_stmt, 3, OBChanceHomeRun);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind double: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_double(m_stmt, 4, OBChanceTriple);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind double: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_double(m_stmt, 5, OBChanceDouble);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind intdouble %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_double(m_stmt, 6, OBChanceSingle);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind double: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_double(m_stmt, 7, OBChanceWalk);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind double: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_double(m_stmt, 8, ChanceDoublePlay);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind double: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_double(m_stmt, 9, OBChanceHomeRunRight);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind double: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_double(m_stmt, 10, OBChanceTripleRight);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind double: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_double(m_stmt, 11, OBChanceDoubleRight);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind double: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_double(m_stmt, 12, OBChanceSingleRight);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind double: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_double(m_stmt, 13, OBChanceWalkRight);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind double: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_double(m_stmt, 14, ChanceDoublePlayRight);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind double: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_double(m_stmt, 15, OBChanceHomeRunLeft);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind double: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_double(m_stmt, 16, OBChanceTripleLeft);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind double: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_double(m_stmt, 17, OBChanceDoubleLeft);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind double: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_double(m_stmt, 18, OBChanceSingleLeft);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind double: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_double(m_stmt, 19, OBChanceWalkLeft);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind double: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_double(m_stmt, 20, ChanceDoublePlayLeft);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind double: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_double(m_stmt, 21, OBChanceBasic);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind double: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_double(m_stmt, 22, OBChanceLeft);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind double: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_double(m_stmt, 23, OBChanceRight);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind double: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 24, Starter);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 25, Relief);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 26, Throws);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 27, Bunting);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 28, Hold);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 29, WP);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 30, Balk);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 31, Pitcher);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 32, ER1);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 33, TeamID);
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
		sprintf_s(buffer, sizeof(buffer), "Execute for Pitcher Insert OK.\n");
		AddToLog(buffer);
	}

	sqlite3_finalize(m_stmt);
	return 0;
}


int CStratOMaticSqLiteDoc::PitcherStatsInsert(int Wins, int Loss, int Saves, float InningsPitched, int ER, int Hits, int Walks, 
	int Strikeouts, int HomeRuns, int Games, int CompleteGames, int Starts, float ERA, float WHIP, int PitcherID, int TeamID)
{
	int rc;
	CHAR buffer[100];
	char *sqlPitcherStats;

	/* Create SQL statement */
	sqlPitcherStats = "INSERT INTO PITCHERSTATS("  \
		"Wins," \
		"Loss," \
		"Saves," \
		"InningsPitched," \
		"ER," \
		"Hits," \
		"Walks," \
		"Strikeouts," \
		"HomeRuns," \
		"Games," \
		"CompleteGames," \
		"Starts," \
		"ERA," \
		"WHIP," \
		"PitcherID," \
		"TeamID" \
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
		"?16" \
		");";

	rc = sqlite3_prepare_v2(m_db, sqlPitcherStats, strlen(sqlPitcherStats), &m_stmt, 0);
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
	rc = sqlite3_bind_int(m_stmt, 1, Wins);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 2, Loss);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 3, Saves);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_double(m_stmt, 4, InningsPitched);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 5, ER);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 6, Hits);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 7, Walks);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 8, Strikeouts);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 9, HomeRuns);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 10, Games);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 11, CompleteGames);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 12, Starts);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_double(m_stmt, 13, ERA);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_double(m_stmt, 14, WHIP);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 15, PitcherID);
	if (rc != SQLITE_OK)
	{
		sprintf_s(buffer, sizeof(buffer), "Could not bind int: %s\n", sqlite3_errmsg(m_db));
		AddToLog(buffer);
	}
	rc = sqlite3_bind_int(m_stmt, 16, TeamID);
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
		sprintf_s(buffer, sizeof(buffer), "Execute for PitcherStats Insert OK.\n");
		AddToLog(buffer);
	}

	sqlite3_finalize(m_stmt);
	return 0;
}

