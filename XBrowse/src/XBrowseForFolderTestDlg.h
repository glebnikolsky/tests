// XBrowseForFolderTestDlg.h : header file
//

#ifndef XBROWSEFORFOLDERTESTDLG_H
#define XBROWSEFORFOLDERTESTDLG_H

#include "XColorStatic.h"

/////////////////////////////////////////////////////////////////////////////
// CXBrowseForFolderTestDlg dialog

class CXBrowseForFolderTestDlg : public CDialog
{
// Construction
public:
	CXBrowseForFolderTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CXBrowseForFolderTestDlg)
	enum { IDD = IDD_XBROWSEFORFOLDERTEST_DIALOG };
	CComboBox	m_comboInitial;
	CXColorStatic	m_staticAsterisk1;
	CXColorStatic	m_staticAsterisk2;
	CXColorStatic	m_staticAsterisk3;
	CXColorStatic	m_staticAsterisk4;
	CXColorStatic	m_staticAsterisk5;
	CComboBox		m_comboRoot;
	CEdit			m_edtReturnCode;
	CEdit			m_edtFolder;
	BOOL			m_bEditBox;
	int				m_nRootIndex;
	CString			m_strInitialFolder;
	CString			m_strCaption;
	int				m_nInitialIndex;
	int		m_nInitial;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXBrowseForFolderTestDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	struct CSIDL
	{
		TCHAR * pszCsidl;
		int		nCsidl;
	};
	static const CSIDL m_nCsidl[];

	// Generated message map functions
	//{{AFX_MSG(CXBrowseForFolderTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSHBrowseForFolder();
	afx_msg void OnXBrowseForFolder();
	afx_msg void OnInitial();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif //XBROWSEFORFOLDERTESTDLG_H
