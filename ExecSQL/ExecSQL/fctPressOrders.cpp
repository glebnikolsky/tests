// fctPressOrders.h : Implementation of the CfctPressOrders class

// CfctPressOrders implementation

// code generated on 15 апреля 2021 г., 11:46

#include "stdafx.h"
#include "fctPressOrders.h"
IMPLEMENT_DYNAMIC(CfctPressOrders, CRecordset)

CfctPressOrders::CfctPressOrders(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_sFilename = "";
	m_nFilesize = 0;
	m_sXML = "";
	m_nFields = 3;
	m_nLoadNo = 0;
	m_nType = 0;
	m_nParams = 2;
	m_nDefaultType = CRecordset::forwardOnly;
}
CString CfctPressOrders::GetDefaultConnect()
{
	//return _T("Description=N;DRIVER=SQL Server Native Client 11.0;SERVER=10.0.18.3;UID=ra;PWD=Njkmrjcdjb;APP=Microsoft\x00ae Visual Studio\x00ae 2015;WSID=39-01;DATABASE=ERBD_EGE_MAIN_19_20190430;");
	return _T("");
}

CString CfctPressOrders::GetDefaultSQL()
{
	return _T("{CALL fct.ccp_GeneratePressXml(?,?)}");
}

void CfctPressOrders::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX, _T("[FileName]"), m_sFilename);
	RFX_Long(pFX, _T("[FileSize]"), m_nFilesize);
	RFX_Text(pFX, _T("[XMLData]"), m_sXML, INT_MAX);

	pFX->SetFieldType(CFieldExchange::inputParam);
	RFX_Long(pFX, _T("[load_no]"), m_nLoadNo);
	RFX_Long(pFX, _T("[xml_type]"), m_nType);
}
/////////////////////////////////////////////////////////////////////////////
// CfctPressOrders diagnostics

#ifdef _DEBUG
void CfctPressOrders::AssertValid() const
{
	CRecordset::AssertValid();
}

void CfctPressOrders::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


