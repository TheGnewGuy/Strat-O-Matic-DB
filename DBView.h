#pragma once

class CBatter;		// Declare the class name
class CStratOMaticDBDoc;	// Declare the class name
class CStratOMaticDBSet;

// COrderView form view
class CDBView : public CRecordView
{
	DECLARE_DYNCREATE(CDBView)
protected:
	virtual ~CDBView(){}
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
	virtual void OnInitialUpdate();
public:
	enum { IDD = IDD_CBATTER_MULTI };
	CBatter* m_pSet;
	// Inline function definition
	CStratOMaticDBDoc* GetDocument() const
	{
		return reinterpret_cast<CStratOMaticDBDoc*>(m_pDocument);
	}
	CBatter* GetRecordset();
	virtual CRecordset* OnGetRecordset();
	CDBView(); // constructor now public
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};