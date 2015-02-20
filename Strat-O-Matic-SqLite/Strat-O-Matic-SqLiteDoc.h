
// Strat-O-Matic-SqLiteDoc.h : interface of the CStratOMaticSqLiteDoc class
//


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
};
