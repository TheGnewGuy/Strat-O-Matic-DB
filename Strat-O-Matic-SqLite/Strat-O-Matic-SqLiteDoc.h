
// Strat-O-Matic-SqLiteDoc.h : interface of the CStratOMaticSqLiteDoc class
//
#include "sqlite3.h"

#pragma once


class CStratOMaticSqLiteDoc : public CDocument
{
protected: // create from serialization only
	CStratOMaticSqLiteDoc();
	DECLARE_DYNCREATE(CStratOMaticSqLiteDoc)

// Attributes
public:

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
	virtual ~CStratOMaticSqLiteDoc();
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
	afx_msg void OnFileOpen();
	CString m_DBFileName;
	CStringArray m_str_array_logmsgs;
	sqlite3 *m_db;
	sqlite3_stmt *m_stmt;
	void AddToLog(LPCTSTR msg);
	int m_dbOpen;
	void AddToLog(char* msg);
	afx_msg void OnSqlCreateTable();
	afx_msg void OnSqlDeleteTable();
	afx_msg void OnSqlInsertTeam();
	afx_msg void OnSqlInsertBatter();
	afx_msg void OnSqlInsertBatterStats();
};
