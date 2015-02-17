
// Strat-O-Matic-SqLiteView.cpp : implementation of the CStratOMaticSqLiteView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Strat-O-Matic-SqLite.h"
#endif

#include "Strat-O-Matic-SqLiteDoc.h"
#include "Strat-O-Matic-SqLiteView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CStratOMaticSqLiteView

IMPLEMENT_DYNCREATE(CStratOMaticSqLiteView, CView)

BEGIN_MESSAGE_MAP(CStratOMaticSqLiteView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CStratOMaticSqLiteView construction/destruction

CStratOMaticSqLiteView::CStratOMaticSqLiteView()
{
	// TODO: add construction code here

}

CStratOMaticSqLiteView::~CStratOMaticSqLiteView()
{
}

BOOL CStratOMaticSqLiteView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CStratOMaticSqLiteView drawing

void CStratOMaticSqLiteView::OnDraw(CDC* /*pDC*/)
{
	CStratOMaticSqLiteDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CStratOMaticSqLiteView printing

BOOL CStratOMaticSqLiteView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CStratOMaticSqLiteView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CStratOMaticSqLiteView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CStratOMaticSqLiteView diagnostics

#ifdef _DEBUG
void CStratOMaticSqLiteView::AssertValid() const
{
	CView::AssertValid();
}

void CStratOMaticSqLiteView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CStratOMaticSqLiteDoc* CStratOMaticSqLiteView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CStratOMaticSqLiteDoc)));
	return (CStratOMaticSqLiteDoc*)m_pDocument;
}
#endif //_DEBUG


// CStratOMaticSqLiteView message handlers
