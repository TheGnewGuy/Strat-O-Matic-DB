// Batter_MULTI.h : Declaration of the CBatter_MULTI

#pragma once

// code generated on Monday, March 10, 2014, 3:48 PM

class CBatter_MULTI_SET : public CRecordset
{
public:
	CBatter_MULTI_SET(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CBatter_MULTI_SET)

// Field/Param Data

// The string types below (if present) reflect the actual data type of the
// database field - CStringA for ANSI datatypes and CStringW for Unicode
// datatypes. This is to prevent the ODBC driver from performing potentially
// unnecessary conversions.  If you wish, you may change these members to
// CString types and the ODBC driver will perform all necessary conversions.
// (Note: You must use an ODBC driver version that is version 3.5 or greater
// to support both Unicode and these conversions).

	long	m_BatterBatterID;	//BatterID Key
	CString	m_BatterFirstName;
	CString	m_BatterLastName;
	BYTE	m_BatterPitcher;
	BYTE	m_BatterCatcher;
	BYTE	m_BatterFirstBase;
	BYTE	m_BatterSecondBase;
	BYTE	m_BatterShortStop;
	BYTE	m_BatterThirdBase;
	BYTE	m_BatterLeftField;
	BYTE	m_BatterCenterField;
	BYTE	m_BatterRightField;
	BYTE	m_BatterBunting;
	BYTE	m_BatterHitRun;
	BYTE	m_BatterRunning;
	BYTE	m_BatterStealing;
	BYTE	m_BatterCatchArm;
	BYTE	m_BatterOutArm;
	BYTE	m_BatterPowerRight;
	BYTE	m_BatterPowerLeft;
	BYTE	m_BatterPass;
	BYTE	m_BatterTRate;
	BYTE	m_BatterER1;
	BYTE	m_BatterER2;
	BYTE	m_BatterER3;
	BYTE	m_BatterER4;
	BYTE	m_BatterER5;
	BYTE	m_BatterER6;
	BYTE	m_BatterER7;
	BYTE	m_BatterER8;
	BYTE	m_BatterER9;
	CStringW	m_BatterOBChanceHomeRun;
	CStringW	m_BatterOBChanceTriple;
	CStringW	m_BatterOBChanceDouble;
	CStringW	m_BatterOBChanceSingle;
	CStringW	m_BatterOBChanceWalk;
	CStringW	m_BatterChanceDoublePlay;
	CStringW	m_BatterOBChanceHomeRunRight;
	CStringW	m_BatterOBChanceTripleRight;
	CStringW	m_BatterOBChanceDoubleRight;
	CStringW	m_BatterOBChanceSingleRight;
	CStringW	m_BatterOBChanceWalkRight;
	CStringW	m_BatterChanceDoublePlayRight;
	CStringW	m_BatterOBChanceHomeRunLeft;
	CStringW	m_BatterOBChanceTripleLeft;
	CStringW	m_BatterOBChanceDoubleLeft;
	CStringW	m_BatterOBChanceSingleLeft;
	CStringW	m_BatterOBChanceWalkLeft;
	CStringW	m_BatterChanceDoublePlayLeft;
	CStringW	m_BatterOBChanceBasic;
	CStringW	m_BatterOBChanceLeft;
	CStringW	m_BatterOBChanceRight;
	BYTE	m_BatterBatterHits;
	COleDateTime	m_BatterLastUpdateTime;
	long	m_BatterStatsBatterStatsID;	//Batter Stats Key
	int	m_BatterStatsAB;
	int	m_BatterStatsRuns;
	int	m_BatterStatsHits;
	int	m_BatterStatsRBI;
	int	m_BatterStats2B;
	int	m_BatterStats3B;
	int	m_BatterStatsHomeRuns;
	int	m_BatterStatsWalk;
	int	m_BatterStatsStrikeOut;
	int	m_BatterStatsReachedOnError;
	int	m_BatterStatsSacrifice;
	int	m_BatterStatsStolenBase;
	int	m_BatterStatsCS;
	int	m_BatterStatsGames;
	int	m_BatterStatsHBP;
	long	m_BatterStatsBatterID;	//Pointer to BatterID Key
	long	m_BatterStatsTeamID;	//Pointer to TeamID Key
	COleDateTime	m_BatterStatsLastUpdateTime;

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


