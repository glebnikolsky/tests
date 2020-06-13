
// TestMessagesDlg.h : header file
//

#pragma once


// CTestMessagesDlg dialog
class CTestMessagesDlg : public CDialogEx
{
// Construction
public:
	CTestMessagesDlg(CWnd* pParent = NULL);	// standard constructor
    ~CTestMessagesDlg();

// Dialog Data
	enum { IDD = IDD_TESTMESSAGES_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnClose();
    afx_msg void OnDestroy();
};
