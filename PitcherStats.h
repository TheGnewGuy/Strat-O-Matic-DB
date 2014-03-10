// PitcherStats.h : Declaration of the CPitcherStats

#pragma once

// code generated on Sunday, March 09, 2014, 4:43 PM

class CPitcherStats : public CRecordset
{
public:
	CPitcherStats(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CPitcherStats)

// Field/Param Data

// The string types below (if present) reflect the actual data type of the
// database field - CStringA for ANSI datatypes and CStringW for Unicode
// datatypes. This is to prevent the ODBC driver from performing potentially
// unnecessary conversions.  If you wish, you may change these members to
// CString types and the ODBC driver will perform all necessary conversions.
// (Note: You must use an ODBC driver version that is version 3.5 or greater
// to support both Unicode and these conversions).

	long	m_PitcherStatsID;	//Pitcher Stats Key
	BYTE	m_Wins;
	int	m_Loss;
	int	m_Saves;
	CStringW	m_InningsPitched;
	BYTE	m_ER;
	BYTE	m_Hits;
	BYTE	m_Walks;
	BYTE	m_Strikeouts;
	int	m_HomeRuns;
	int	m_Games;
	int	m_CompleteGames;
	int	m_Starts;
	long	m_PitcherID;	//Pointer to PitcherID Key
	long	m_TeamID;	//Pointer to TeamID Key
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


