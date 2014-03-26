
// Strat-O-Matic-DBView.cpp : implementation of the CStratOMaticDBView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Strat-O-Matic-DB.h"
#endif

//#include "Strat-O-Matic-DBSet.h"
#include "Strat-O-Matic-DBDoc.h"
#include "Strat-O-Matic-DBView.h"
#include "Batter.h"
//#include "DBView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CStratOMaticDBView

IMPLEMENT_DYNCREATE(CStratOMaticDBView, CRecordView)

BEGIN_MESSAGE_MAP(CStratOMaticDBView, CRecordView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CRecordView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CRecordView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CStratOMaticDBView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_BN_CLICKED(IDNEXT, &CStratOMaticDBView::OnClickedIdnext)
END_MESSAGE_MAP()

// CStratOMaticDBView construction/destruction

CStratOMaticDBView::CStratOMaticDBView()
	: CRecordView(CStratOMaticDBView::IDD)
{
	m_pSet = NULL;
	// TODO: add construction code here
}

CStratOMaticDBView::~CStratOMaticDBView()
{
}

void CStratOMaticDBView::DoDataExchange(CDataExchange* pDX)
{
	CRecordView::DoDataExchange(pDX);
	// you can insert DDX_Field* functions here to 'connect' your controls to the database fields, ex.
	// DDX_FieldText(pDX, IDC_MYEDITBOX, m_pSet->m_szColumn1, m_pSet);
	// DDX_FieldCheck(pDX, IDC_MYCHECKBOX, m_pSet->m_bColumn2, m_pSet);
	// See MSDN and ODBC samples for more information
	CRecordView::DoDataExchange(pDX);
	DDX_FieldText(pDX, IDC_EDIT_FirstName,
		m_pSet->m_FirstName, m_pSet);
}

BOOL CStratOMaticDBView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CRecordView::PreCreateWindow(cs);
}

CBatter* CStratOMaticDBView::GetRecordset()
{
	ASSERT(m_pSet != NULL);
	return m_pSet;
}

CRecordset* CStratOMaticDBView::OnGetRecordset()
{
	return m_pSet;
}

void CStratOMaticDBView::OnInitialUpdate()
{
//	m_pSet = &GetDocument()->m_StratOMaticDBSet;
//	CRecordView::OnInitialUpdate();
	BeginWaitCursor();
	CStratOMaticDBDoc* pDoc = static_cast<CStratOMaticDBDoc*>(GetDocument());
	m_pSet = pDoc->m_pBatter_set; // Get a pointer to the recordset
	// Use the DB that is open for products recordset
	m_pSet->m_pDatabase = &pDoc->m_pDatabase;
	// Set the current product ID as parameter
	m_pSet->m_FirstName = pDoc->m_pBatter_set->m_FirstName;
	// Set the filter as product ID field
	//m_pSet->m_strFilter =
	//	“[ProductID] = ? AND[Orders].[OrderID] = [Order Details].[OrderID]”;
	CRecordView::OnInitialUpdate();
	EndWaitCursor();
}


// CStratOMaticDBView printing


void CStratOMaticDBView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CStratOMaticDBView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CStratOMaticDBView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CStratOMaticDBView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CStratOMaticDBView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CStratOMaticDBView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CStratOMaticDBView diagnostics

#ifdef _DEBUG
void CStratOMaticDBView::AssertValid() const
{
	CRecordView::AssertValid();
}

void CStratOMaticDBView::Dump(CDumpContext& dc) const
{
	CRecordView::Dump(dc);
}

//CStratOMaticDBDoc* CStratOMaticDBView::GetDocument() const // non-debug version is inline
//{
//	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CStratOMaticDBDoc)));
//	return (CStratOMaticDBDoc*)m_pDocument;
//}
#endif //_DEBUG


// CStratOMaticDBView database support
//CRecordset* CStratOMaticDBView::OnGetRecordset()
//{
//	return m_pSet;
//}



// CStratOMaticDBView message handlers


void CStratOMaticDBView::OnDraw(CDC* /*pDC*/)
{
	// TODO: Add your specialized code here and/or call the base class
}


void CStratOMaticDBView::OnClickedIdnext()
{
	// TODO: Add your control notification handler code here
}
