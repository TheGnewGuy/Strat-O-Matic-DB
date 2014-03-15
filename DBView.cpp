
#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Strat-O-Matic-DB.h"
#endif

#include "Strat-O-Matic-DBSet.h"
#include "Strat-O-Matic-DBDoc.h"
#include "Strat-O-Matic-DBView.h"
#include "Batter_MULTI_SET.h"
#include "DBView.h"

IMPLEMENT_DYNCREATE(CDBView, CRecordView)

CDBView::CDBView()
: CRecordView(CDBView::IDD), m_pSet(NULL)
{
}

// COrderView diagnostics
#ifdef _DEBUG
void CDBView::AssertValid() const
{
	CRecordView::AssertValid();
}
void CDBView::Dump(CDumpContext& dc) const
{
	CRecordView::Dump(dc);
}
#endif //_DEBUG

void CDBView::DoDataExchange(CDataExchange* pDX)
{
	CRecordView::DoDataExchange(pDX);
	DDX_FieldText(pDX, IDC_EDIT_FirstName,
		m_pSet->m_BatterFirstName, m_pSet);
}

CBatter_MULTI_SET* CDBView::GetRecordset()
{
	ASSERT(m_pSet != NULL);
	return m_pSet;
}

CRecordset* CDBView::OnGetRecordset()
{
	return m_pSet;
}

void CDBView::OnInitialUpdate()
{
	BeginWaitCursor();
	CStratOMaticDBDoc* pDoc = static_cast<CStratOMaticDBDoc*>(GetDocument());
	m_pSet = &pDoc->m_Batter_MULTI_set; // Get a pointer to the recordset
	// Use the DB that is open for products recordset
	m_pSet->m_pDatabase = pDoc->m_Batter_MULTI_set.m_pDatabase;
	// Set the current product ID as parameter
	m_pSet->m_BatterFirstName = pDoc->m_Batter_MULTI_set.m_BatterFirstName;
	// Set the filter as product ID field
	//m_pSet->m_strFilter =
	//	“[ProductID] = ? AND[Orders].[OrderID] = [Order Details].[OrderID]”;
	CRecordView::OnInitialUpdate();
	EndWaitCursor();
}