// CheckComboDlg.h : header file
//

#if !defined(AFX_CHECKCOMBODLG_H__E15F1A86_9C1C_11D3_847A_0050DA346D7F__INCLUDED_)
#define AFX_CHECKCOMBODLG_H__E15F1A86_9C1C_11D3_847A_0050DA346D7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CheckComboBox.h"

/////////////////////////////////////////////////////////////////////////////
// CCheckComboDlg dialog

class CCheckComboDlg : public CDialog
{
// Construction
public:
	CCheckComboDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCheckComboDlg)
	enum { IDD = IDD_CHECKCOMBO_DIALOG };
	CCheckComboBox	m_Combo;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCheckComboDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCheckComboDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnOK();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHECKCOMBODLG_H__E15F1A86_9C1C_11D3_847A_0050DA346D7F__INCLUDED_)
