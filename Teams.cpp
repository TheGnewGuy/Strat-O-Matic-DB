// Teams.h : Implementation of the CTeams class



// CTeams implementation

// code generated on Tuesday, March 18, 2014, 9:21 PM

#include "stdafx.h"
#include "Teams.h"
IMPLEMENT_DYNAMIC(CTeams, CRecordset)

CTeams::CTeams(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_TeamID = 0;
	m_TeamName = L"";
	m_TeamNameShort = L"";
	m_BallparkName = L"";
	m_HomeWins = 0;
	m_HomeLosses = 0;
	m_AwayWins = 0;
	m_AwayLosses = 0;
	m_DivisionID = 0;
	m_TeamYear = 0;
	m_LastUpdateTime;
	m_nFields = 11;
	m_nDefaultType = dynaset;
}
//#error Security Issue: The connection string may contain a password
// The connection string below may contain plain text passwords and/or
// other sensitive information. Please remove the #error after reviewing
// the connection string for any security related issues. You may want to
// store the password in some other form or use a different user authentication.
CString CTeams::GetDefaultConnect()
{
	return _T("DSN=MS Access Database;DBQ=C:\\Family\\SOURCE\\C\\Strat-O-Matic-DB\\Baseball.accdb;DefaultDir=C:\\Family\\SOURCE\\C\\Strat-O-Matic-DB;DriverId=25;FIL=MS Access;MaxBufferSize=2048;PageTimeout=5;UID=admin;");
}

CString CTeams::GetDefaultSQL()
{
	return _T("[Teams]");
}

void CTeams::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// Macros such as RFX_Text() and RFX_Int() are dependent on the
// type of the member variable, not the type of the field in the database.
// ODBC will try to automatically convert the column value to the requested type
	RFX_Long(pFX, _T("[TeamID]"), m_TeamID);
	RFX_Text(pFX, _T("[TeamName]"), m_TeamName);
	RFX_Text(pFX, _T("[TeamNameShort]"), m_TeamNameShort);
	RFX_Text(pFX, _T("[BallparkName]"), m_BallparkName);
	RFX_Int(pFX, _T("[HomeWins]"), m_HomeWins);
	RFX_Int(pFX, _T("[HomeLosses]"), m_HomeLosses);
	RFX_Int(pFX, _T("[AwayWins]"), m_AwayWins);
	RFX_Int(pFX, _T("[AwayLosses]"), m_AwayLosses);
	RFX_Long(pFX, _T("[DivisionID]"), m_DivisionID);
	RFX_Long(pFX, _T("[TeamYear]"), m_TeamYear);
	RFX_Date(pFX, _T("[LastUpdateTime]"), m_LastUpdateTime);

}
/////////////////////////////////////////////////////////////////////////////
// CTeams diagnostics

#ifdef _DEBUG
void CTeams::AssertValid() const
{
	CRecordset::AssertValid();
}

void CTeams::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


