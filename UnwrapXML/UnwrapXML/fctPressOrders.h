// fctPressOrders.h : Declaration of the CfctPressOrders

#pragma once

// code generated on 15 апреля 2021 г., 11:46

class CfctPressOrders : public CRecordset
{
public:
	CfctPressOrders(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CfctPressOrders)

// Field/Param Data
	CStringA	m_sFilename;
	long		m_nFilesize;
	CStringA	m_sXML;
	long		m_nLoadNo;
	long		m_nType;


// Overrides
	// Wizard generated virtual function overrides
	public:
	virtual CString GetDefaultConnect();	// Default connection string
	virtual CString GetDefaultSQL(); 	// default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);	// RFX support

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

};


