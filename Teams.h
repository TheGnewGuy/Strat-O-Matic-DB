// Teams.h : Declaration of the CTeams

#pragma once

// code generated on Tuesday, March 25, 2014, 8:39 PM

class CTeams : public CRecordset
{
public:
	CTeams(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CTeams)

// Field/Param Data

// The string types below (if present) reflect the actual data type of the
// database field - CStringA for ANSI datatypes and CStringW for Unicode
// datatypes. This is to prevent the ODBC driver from performing potentially
// unnecessary conversions.  If you wish, you may change these members to
// CString types and the ODBC driver will perform all necessary conversions.
// (Note: You must use an ODBC driver version that is version 3.5 or greater
// to support both Unicode and these conversions).

	long	m_TeamID;
	CString	m_TeamName;
	CString	m_TeamNameShort;
	CString	m_BallparkName;
	int		m_HomeWins;
	int		m_HomeLosses;
	int		m_AwayWins;
	int		m_AwayLosses;
	long	m_LeagueID;
	long	m_ConferenceID;
	long	m_DivisionID;
	CString	m_TeamYear;
	COleDateTime	m_LastUpdateTime;

// Overrides
	// Wizard generated virtual function overrides
	public:
	virtual CString GetDefaultConnect();	// Default connection string

	virtual CString GetDefaultSQL(); 	// default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);	// RFX support

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

};


