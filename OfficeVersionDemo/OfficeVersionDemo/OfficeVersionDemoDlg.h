// OfficeVersionDemoDlg.h : header file
//

#pragma once
#include "afxwin.h"


// COfficeVersionDemoDlg dialog
class COfficeVersionDemoDlg : public CDialog
{
// Construction
public:
	COfficeVersionDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_OFFICEVERSIONDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	static CString ShowResults();
// Implementation
protected:
	HICON m_hIcon;
	CString m_resultsString;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

};
