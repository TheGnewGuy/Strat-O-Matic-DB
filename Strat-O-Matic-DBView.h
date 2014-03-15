
// Strat-O-Matic-DBView.h : interface of the CStratOMaticDBView class
//

#pragma once

class CStratOMaticDBSet;

class CStratOMaticDBView : public CRecordView
{
protected: // create from serialization only
//	CStratOMaticDBView();
	DECLARE_DYNCREATE(CStratOMaticDBView)

public:
//	enum{ IDD = IDD_STRATOMATICDB_FORM };
//	CStratOMaticDBSet* m_pSet;
	enum { IDD = IDD_CBATTER_MULTI };
	CBatter* m_pSet;
	// Inline function definition
	CStratOMaticDBDoc* GetDocument() const
	{
		return reinterpret_cast<CStratOMaticDBDoc*>(m_pDocument);
	}
	CBatter* GetRecordset();
	virtual CRecordset* OnGetRecordset();
	CStratOMaticDBView(); // constructor now public

// Attributes
public:
//	CStratOMaticDBDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
//	virtual CRecordset* OnGetRecordset();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CStratOMaticDBView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
	virtual void OnDraw(CDC* /*pDC*/);
};

#ifndef _DEBUG  // debug version in Strat-O-Matic-DBView.cpp
inline CStratOMaticDBDoc* CStratOMaticDBView::GetDocument() const
   { return reinterpret_cast<CStratOMaticDBDoc*>(m_pDocument); }
#endif

