// Batter_MULTI.h : Implementation of the CBatter_MULTI class



// CBatter_MULTI implementation

// code generated on Monday, March 10, 2014, 3:48 PM

#include "stdafx.h"
#include "Batter_MULTI_SET.h"
IMPLEMENT_DYNAMIC(CBatter_MULTI_SET, CRecordset)

CBatter_MULTI_SET::CBatter_MULTI_SET(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_BatterBatterID = 0;
	m_BatterFirstName = L"";
	m_BatterLastName = L"";
	m_BatterPitcher = 0;
	m_BatterCatcher = 0;
	m_BatterFirstBase = 0;
	m_BatterSecondBase = 0;
	m_BatterShortStop = 0;
	m_BatterThirdBase = 0;
	m_BatterLeftField = 0;
	m_BatterCenterField = 0;
	m_BatterRightField = 0;
	m_BatterBunting = 0;
	m_BatterHitRun = 0;
	m_BatterRunning = 0;
	m_BatterStealing = 0;
	m_BatterCatchArm = 0;
	m_BatterOutArm = 0;
	m_BatterPowerRight = 0;
	m_BatterPowerLeft = 0;
	m_BatterPass = 0;
	m_BatterTRate = 0;
	m_BatterER1 = 0;
	m_BatterER2 = 0;
	m_BatterER3 = 0;
	m_BatterER4 = 0;
	m_BatterER5 = 0;
	m_BatterER6 = 0;
	m_BatterER7 = 0;
	m_BatterER8 = 0;
	m_BatterER9 = 0;
	m_BatterOBChanceHomeRun = L"";
	m_BatterOBChanceTriple = L"";
	m_BatterOBChanceDouble = L"";
	m_BatterOBChanceSingle = L"";
	m_BatterOBChanceWalk = L"";
	m_BatterChanceDoublePlay = L"";
	m_BatterOBChanceHomeRunRight = L"";
	m_BatterOBChanceTripleRight = L"";
	m_BatterOBChanceDoubleRight = L"";
	m_BatterOBChanceSingleRight = L"";
	m_BatterOBChanceWalkRight = L"";
	m_BatterChanceDoublePlayRight = L"";
	m_BatterOBChanceHomeRunLeft = L"";
	m_BatterOBChanceTripleLeft = L"";
	m_BatterOBChanceDoubleLeft = L"";
	m_BatterOBChanceSingleLeft = L"";
	m_BatterOBChanceWalkLeft = L"";
	m_BatterChanceDoublePlayLeft = L"";
	m_BatterOBChanceBasic = L"";
	m_BatterOBChanceLeft = L"";
	m_BatterOBChanceRight = L"";
	m_BatterBatterHits = 0;
	m_BatterLastUpdateTime;
	m_BatterStatsBatterStatsID = 0;
	m_BatterStatsAB = 0;
	m_BatterStatsRuns = 0;
	m_BatterStatsHits = 0;
	m_BatterStatsRBI = 0;
	m_BatterStats2B = 0;
	m_BatterStats3B = 0;
	m_BatterStatsHomeRuns = 0;
	m_BatterStatsWalk = 0;
	m_BatterStatsStrikeOut = 0;
	m_BatterStatsReachedOnError = 0;
	m_BatterStatsSacrifice = 0;
	m_BatterStatsStolenBase = 0;
	m_BatterStatsCS = 0;
	m_BatterStatsGames = 0;
	m_BatterStatsHBP = 0;
	m_BatterStatsBatterID = 0;
	m_BatterStatsTeamID = 0;
	m_BatterStatsLastUpdateTime;
	m_nFields = 73;
	m_nDefaultType = dynaset;
}
//#error Security Issue: The connection string may contain a password
// The connection string below may contain plain text passwords and/or
// other sensitive information. Please remove the #error after reviewing
// the connection string for any security related issues. You may want to
// store the password in some other form or use a different user authentication.
CString CBatter_MULTI_SET::GetDefaultConnect()
{
	return _T("DSN=MS Access Database;DBQ=C:\\Family\\SOURCE\\C\\Strat-O-Matic-DB\\Baseball.accdb;DefaultDir=C:\\Family\\SOURCE\\C\\Strat-O-Matic-DB;DriverId=25;FIL=MS Access;MaxBufferSize=2048;PageTimeout=5;UID=admin;");
}

CString CBatter_MULTI_SET::GetDefaultSQL()
{
	return _T("[Batter],[BatterStats]");
}

void CBatter_MULTI_SET::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// Macros such as RFX_Text() and RFX_Int() are dependent on the
// type of the member variable, not the type of the field in the database.
// ODBC will try to automatically convert the column value to the requested type
	RFX_Long(pFX, _T("[Batter].[BatterID]"), m_BatterBatterID);
	RFX_Text(pFX, _T("[Batter].[FirstName]"), m_BatterFirstName);
	RFX_Text(pFX, _T("[Batter].[LastName]"), m_BatterLastName);
	RFX_Byte(pFX, _T("[Batter].[Pitcher]"), m_BatterPitcher);
	RFX_Byte(pFX, _T("[Batter].[Catcher]"), m_BatterCatcher);
	RFX_Byte(pFX, _T("[Batter].[FirstBase]"), m_BatterFirstBase);
	RFX_Byte(pFX, _T("[Batter].[SecondBase]"), m_BatterSecondBase);
	RFX_Byte(pFX, _T("[Batter].[ShortStop]"), m_BatterShortStop);
	RFX_Byte(pFX, _T("[Batter].[ThirdBase]"), m_BatterThirdBase);
	RFX_Byte(pFX, _T("[Batter].[LeftField]"), m_BatterLeftField);
	RFX_Byte(pFX, _T("[Batter].[CenterField]"), m_BatterCenterField);
	RFX_Byte(pFX, _T("[Batter].[RightField]"), m_BatterRightField);
	RFX_Byte(pFX, _T("[Batter].[Bunting]"), m_BatterBunting);
	RFX_Byte(pFX, _T("[Batter].[HitRun]"), m_BatterHitRun);
	RFX_Byte(pFX, _T("[Batter].[Running]"), m_BatterRunning);
	RFX_Byte(pFX, _T("[Batter].[Stealing]"), m_BatterStealing);
	RFX_Byte(pFX, _T("[Batter].[CatchArm]"), m_BatterCatchArm);
	RFX_Byte(pFX, _T("[Batter].[OutArm]"), m_BatterOutArm);
	RFX_Byte(pFX, _T("[Batter].[PowerRight]"), m_BatterPowerRight);
	RFX_Byte(pFX, _T("[Batter].[PowerLeft]"), m_BatterPowerLeft);
	RFX_Byte(pFX, _T("[Batter].[Pass]"), m_BatterPass);
	RFX_Byte(pFX, _T("[Batter].[TRate]"), m_BatterTRate);
	RFX_Byte(pFX, _T("[Batter].[ER1]"), m_BatterER1);
	RFX_Byte(pFX, _T("[Batter].[ER2]"), m_BatterER2);
	RFX_Byte(pFX, _T("[Batter].[ER3]"), m_BatterER3);
	RFX_Byte(pFX, _T("[Batter].[ER4]"), m_BatterER4);
	RFX_Byte(pFX, _T("[Batter].[ER5]"), m_BatterER5);
	RFX_Byte(pFX, _T("[Batter].[ER6]"), m_BatterER6);
	RFX_Byte(pFX, _T("[Batter].[ER7]"), m_BatterER7);
	RFX_Byte(pFX, _T("[Batter].[ER8]"), m_BatterER8);
	RFX_Byte(pFX, _T("[Batter].[ER9]"), m_BatterER9);
	RFX_Text(pFX, _T("[Batter].[OBChanceHomeRun]"), m_BatterOBChanceHomeRun);
	RFX_Text(pFX, _T("[Batter].[OBChanceTriple]"), m_BatterOBChanceTriple);
	RFX_Text(pFX, _T("[Batter].[OBChanceDouble]"), m_BatterOBChanceDouble);
	RFX_Text(pFX, _T("[Batter].[OBChanceSingle]"), m_BatterOBChanceSingle);
	RFX_Text(pFX, _T("[Batter].[OBChanceWalk]"), m_BatterOBChanceWalk);
	RFX_Text(pFX, _T("[Batter].[ChanceDoublePlay]"), m_BatterChanceDoublePlay);
	RFX_Text(pFX, _T("[Batter].[OBChanceHomeRunRight]"), m_BatterOBChanceHomeRunRight);
	RFX_Text(pFX, _T("[Batter].[OBChanceTripleRight]"), m_BatterOBChanceTripleRight);
	RFX_Text(pFX, _T("[Batter].[OBChanceDoubleRight]"), m_BatterOBChanceDoubleRight);
	RFX_Text(pFX, _T("[Batter].[OBChanceSingleRight]"), m_BatterOBChanceSingleRight);
	RFX_Text(pFX, _T("[Batter].[OBChanceWalkRight]"), m_BatterOBChanceWalkRight);
	RFX_Text(pFX, _T("[Batter].[ChanceDoublePlayRight]"), m_BatterChanceDoublePlayRight);
	RFX_Text(pFX, _T("[Batter].[OBChanceHomeRunLeft]"), m_BatterOBChanceHomeRunLeft);
	RFX_Text(pFX, _T("[Batter].[OBChanceTripleLeft]"), m_BatterOBChanceTripleLeft);
	RFX_Text(pFX, _T("[Batter].[OBChanceDoubleLeft]"), m_BatterOBChanceDoubleLeft);
	RFX_Text(pFX, _T("[Batter].[OBChanceSingleLeft]"), m_BatterOBChanceSingleLeft);
	RFX_Text(pFX, _T("[Batter].[OBChanceWalkLeft]"), m_BatterOBChanceWalkLeft);
	RFX_Text(pFX, _T("[Batter].[ChanceDoublePlayLeft]"), m_BatterChanceDoublePlayLeft);
	RFX_Text(pFX, _T("[Batter].[OBChanceBasic]"), m_BatterOBChanceBasic);
	RFX_Text(pFX, _T("[Batter].[OBChanceLeft]"), m_BatterOBChanceLeft);
	RFX_Text(pFX, _T("[Batter].[OBChanceRight]"), m_BatterOBChanceRight);
	RFX_Byte(pFX, _T("[Batter].[BatterHits]"), m_BatterBatterHits);
	RFX_Date(pFX, _T("[Batter].[LastUpdateTime]"), m_BatterLastUpdateTime);
	RFX_Long(pFX, _T("[BatterStats].[BatterStatsID]"), m_BatterStatsBatterStatsID);
	RFX_Int(pFX, _T("[BatterStats].[AB]"), m_BatterStatsAB);
	RFX_Int(pFX, _T("[BatterStats].[Runs]"), m_BatterStatsRuns);
	RFX_Int(pFX, _T("[BatterStats].[Hits]"), m_BatterStatsHits);
	RFX_Int(pFX, _T("[BatterStats].[RBI]"), m_BatterStatsRBI);
	RFX_Int(pFX, _T("[BatterStats].[2B]"), m_BatterStats2B);
	RFX_Int(pFX, _T("[BatterStats].[3B]"), m_BatterStats3B);
	RFX_Int(pFX, _T("[BatterStats].[HomeRuns]"), m_BatterStatsHomeRuns);
	RFX_Int(pFX, _T("[BatterStats].[Walk]"), m_BatterStatsWalk);
	RFX_Int(pFX, _T("[BatterStats].[StrikeOut]"), m_BatterStatsStrikeOut);
	RFX_Int(pFX, _T("[BatterStats].[ReachedOnError]"), m_BatterStatsReachedOnError);
	RFX_Int(pFX, _T("[BatterStats].[Sacrifice]"), m_BatterStatsSacrifice);
	RFX_Int(pFX, _T("[BatterStats].[StolenBase]"), m_BatterStatsStolenBase);
	RFX_Int(pFX, _T("[BatterStats].[CS]"), m_BatterStatsCS);
	RFX_Int(pFX, _T("[BatterStats].[Games]"), m_BatterStatsGames);
	RFX_Int(pFX, _T("[BatterStats].[HBP]"), m_BatterStatsHBP);
	RFX_Long(pFX, _T("[BatterStats].[BatterID]"), m_BatterStatsBatterID);
	RFX_Long(pFX, _T("[BatterStats].[TeamID]"), m_BatterStatsTeamID);
	RFX_Date(pFX, _T("[BatterStats].[LastUpdateTime]"), m_BatterStatsLastUpdateTime);

}
/////////////////////////////////////////////////////////////////////////////
// CBatter_MULTI_SET diagnostics

#ifdef _DEBUG
void CBatter_MULTI_SET::AssertValid() const
{
	CRecordset::AssertValid();
}

void CBatter_MULTI_SET::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


