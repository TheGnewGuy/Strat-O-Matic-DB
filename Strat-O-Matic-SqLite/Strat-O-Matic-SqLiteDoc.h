
// Strat-O-Matic-SqLiteDoc.h : interface of the CStratOMaticSqLiteDoc class
//
#include "sqlite3.h"

#pragma once


class CStratOMaticSqLiteDoc : public CDocument
{
protected: // create from serialization only
	CStratOMaticSqLiteDoc();
	DECLARE_DYNCREATE(CStratOMaticSqLiteDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CStratOMaticSqLiteDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnFileOpen();
	CString m_DBFileName;
	CStringArray m_str_array_logmsgs;
	sqlite3 *m_db;
	sqlite3_stmt *m_stmt;
	void AddToLog(LPCTSTR msg);
	int m_dbOpen;
	void AddToLog(char* msg);
	afx_msg void OnSqlCreateTable();
	afx_msg void OnSqlDeleteTable();
	afx_msg void OnSqlInsertTeam();
	afx_msg void OnSqlInsertBatter();
	afx_msg void OnSqlInsertBatterStats();
	afx_msg void OnSqlDbconvert();
	bool CreateDefaultLeague();
	void ExportLeagueFileToDB(CString strLeagueDir, CString strteamName);
	void ExportBaseFileToDB(CString strDir, CString strTeamName);
	afx_msg void OnSqlInsertDefaultLeague();
	int LeagueInsert(CStringA strName, int NumberConf, int NumDivisions, bool Base);
	int ConferenceInsert(CStringA strConfName, int LeagueID, bool Base);
	int GetLeagueID(CStringA strLeagueName);
	int DivisionInsert(CStringA strDivisionName, int LeagueID, int ConferenceID, bool Base);
	int GetConferenceID(CStringA strConfName);
	int TeamInsert(CStringA strTeamName, CStringA strTeamNameShort, CStringA strBallParkName, int HomeWins, int HomeLosses, int AwayWins, int AwayLosses, int LeagueID, int ConferenceID, int DivisionID, int Year, bool Base);
	int GetDivisionID(CStringA strDivisionName);
	int BatterInsert(CStringA strFirstName, CStringA strLastName, int Pitcher, int Catcher, int FirstBase, int SecondBase, int ShortStop, int ThirdBase, int LeftField, int CenterField, int RightField, int Bunting, int HitRun, int Running, int Stealing, int CatchArm, int OutArm, int PowerRight, int PowerLeft, int Pass, int TRate, int ER1, int ER2, int ER3, int ER4, int ER5, int ER6, int ER7, int ER8, int ER9, bool BatterHits, int TeamID, float OBChanceHomeRun, float OBChanceTriple, float OBChanceDouble, float OBChanceSingle, float OBChanceWalk, float ChanceDoublePlay, float OBChanceHomeRunRight, float OBChanceTripleRight, float OBChanceDoubleRight, float OBChanceSingleRight, float OBChanceWalkRight, float ChanceDoublePlayRight, float OBChanceHomeRunLeft, float OBChanceTripleLeft, float OBChanceDoubleLeft, float OBChanceSingleLeft, float OBChanceWalkLeft, float ChanceDoublePlayLeft, float OBChanceBasic, float OBChanceLeft, float OBChanceRight);
	int GetTeamID(CStringA strTeamName, int Year);
	int BatterStatsInsert(int AB, int Runs, int Hits, int RBI, int Doubles, int Triples, int HomeRuns, int Walk, int Strikout, int ReachedOnError, int Sacrifice, int StolenBase, int CS, int Games, int HBP, float AVG, float SLG, float OBP, int BatterID, int TeamID);
	int GetBatterID(CStringA strFirstName, CStringA strLastName);
	int GetPitcherID(CStringA strFirstName, CStringA strLastName);
	int PitcherInsert(CStringA strFirstName, CStringA strLastName, float OBChanceHomeRun, float OBChanceTriple, float OBChanceDouble, float OBChanceSingle, float OBChanceWalk, float ChanceDoublePlay, float OBChanceHomeRunRight, float OBChanceTripleRight, float OBChanceDoubleRight, float OBChanceSingleRight, float OBChanceWalkRight, float ChanceDoublePlayRight, float OBChanceHomeRunLeft, float OBChanceTripleLeft, float OBChanceDoubleLeft, float OBChanceSingleLeft, float OBChanceWalkLeft, float ChanceDoublePlayLeft, float OBChanceBasic, float OBChanceLeft, float OBChanceRight, int Starter, int Relief, int Throws, int Bunting, int Hold, int WP, int Balk, int Pitcher, int ER1, int TeamID);
	int PitcherStatsInsert(int Wins, int Loss, int Saves, int InningsPitched, int ER, int Hits, int Walks, int Strikeouts, int HomeRuns, int Games, int CompleteGames, int Starts, float ERA, float WHIP, int PitcherID, int TeamID);
	afx_msg void OnSqlInsertPitcher();
	afx_msg void OnSqlInsertPitcherStats();
	int GetBatterID(CStringA strFirstName, CStringA strLastName, int TeamID);
	int GetPitcherID(CStringA strFirstName, CStringA strLastName, int TeamID);
	int GetTeamID(CStringA strTeamName, int Year, int LeagueID);
	int GetTeamIDBase(CStringA strTeamName, int Year, bool Base);
};
