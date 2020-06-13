#pragma once

#include "..\sinstance.h"

class CSinstancedlgDlg : public CDialog
{
public:
	CSinstancedlgDlg(CWnd* pParent = NULL);	// standard constructor

	enum { IDD = IDD_SINSTANCEDLG_DIALOG };

  CInstanceChecker m_InstanceChecker;

	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

protected:
	HICON m_hIcon;

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	DECLARE_MESSAGE_MAP()
};

