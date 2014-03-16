
// Strat-O-Matic-DBDoc.h : interface of the CStratOMaticDBDoc class
//


#pragma once
#include "Strat-O-Matic-DBSet.h"
#include "Batter_MULTI_SET.h"
#include "afxdb.h"
#include "Batter.h"


class CStratOMaticDBDoc : public CDocument
{
protected: // create from serialization only
	CStratOMaticDBDoc();
	DECLARE_DYNCREATE(CStratOMaticDBDoc)

// Attributes
public:
	CStratOMaticDBSet m_StratOMaticDBSet;

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CStratOMaticDBDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnDbSelect();
	afx_msg void OnDbAdd();
	afx_msg void OnDbConvert();
	CBatter_MULTI_SET m_Batter_MULTI_set;
	CBatter* m_pBatter_set;
	CDatabase m_pDatabase;
	void ExportFileToDB(CString strDir, CString strTeamName);
};
