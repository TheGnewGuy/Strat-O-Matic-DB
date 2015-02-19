
// Strat-O-Matic-SqLiteView.h : interface of the CStratOMaticSqLiteView class
//

#pragma once


class CStratOMaticSqLiteView : public CScrollView
{
protected: // create from serialization only
	CStratOMaticSqLiteView();
	DECLARE_DYNCREATE(CStratOMaticSqLiteView)

// Attributes
public:
	CStratOMaticSqLiteDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CStratOMaticSqLiteView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
};

#ifndef _DEBUG  // debug version in Strat-O-Matic-SqLiteView.cpp
inline CStratOMaticSqLiteDoc* CStratOMaticSqLiteView::GetDocument() const
   { return reinterpret_cast<CStratOMaticSqLiteDoc*>(m_pDocument); }
#endif

