// Batter.h : Declaration of the CBatter

#pragma once

// code generated on Sunday, March 09, 2014, 4:40 PM

class CBatter : public CRecordset
{
public:
	CBatter(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CBatter)

// Field/Param Data

// The string types below (if present) reflect the actual data type of the
// database field - CStringA for ANSI datatypes and CStringW for Unicode
// datatypes. This is to prevent the ODBC driver from performing potentially
// unnecessary conversions.  If you wish, you may change these members to
// CString types and the ODBC driver will perform all necessary conversions.
// (Note: You must use an ODBC driver version that is version 3.5 or greater
// to support both Unicode and these conversions).

	long	m_BatterID;	//BatterID Key
	CStringW	m_FirstName;
	CStringW	m_LastName;
	BYTE	m_Pitcher;
	BYTE	m_Catcher;
	BYTE	m_FirstBase;
	BYTE	m_SecondBase;
	BYTE	m_ShortStop;
	BYTE	m_ThirdBase;
	BYTE	m_LeftField;
	BYTE	m_CenterField;
	BYTE	m_RightField;
	BYTE	m_Bunting;
	BYTE	m_HitRun;
	BYTE	m_Running;
	BYTE	m_Stealing;
	BYTE	m_CatchArm;
	BYTE	m_OutArm;
	BYTE	m_PowerRight;
	BYTE	m_PowerLeft;
	BYTE	m_Pass;
	BYTE	m_TRate;
	BYTE	m_ER1;
	BYTE	m_ER2;
	BYTE	m_ER3;
	BYTE	m_ER4;
	BYTE	m_ER5;
	BYTE	m_ER6;
	BYTE	m_ER7;
	BYTE	m_ER8;
	BYTE	m_ER9;
	CStringW	m_OBChanceHomeRun;
	CStringW	m_OBChanceTriple;
	CStringW	m_OBChanceDouble;
	CStringW	m_OBChanceSingle;
	CStringW	m_OBChanceWalk;
	CStringW	m_ChanceDoublePlay;
	CStringW	m_OBChanceHomeRunRight;
	CStringW	m_OBChanceTripleRight;
	CStringW	m_OBChanceDoubleRight;
	CStringW	m_OBChanceSingleRight;
	CStringW	m_OBChanceWalkRight;
	CStringW	m_ChanceDoublePlayRight;
	CStringW	m_OBChanceHomeRunLeft;
	CStringW	m_OBChanceTripleLeft;
	CStringW	m_OBChanceDoubleLeft;
	CStringW	m_OBChanceSingleLeft;
	CStringW	m_OBChanceWalkLeft;
	CStringW	m_ChanceDoublePlayLeft;
	CStringW	m_OBChanceBasic;
	CStringW	m_OBChanceLeft;
	CStringW	m_OBChanceRight;
	BYTE	m_BatterHits;
	COleDateTime 	m_LastUpdateTime;

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


