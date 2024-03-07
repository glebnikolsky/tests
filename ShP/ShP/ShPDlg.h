
// ShPDlg.h : ���� ���������
//

#pragma once
#include "afxwin.h"


// ���������� ���� CShPDlg
class CShPDlg : public CDialogEx
{
// ��������
public:
	CShPDlg(CWnd* pParent = NULL);	// ����������� �����������

// ������ ����������� ����
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SHP_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// ��������� DDX/DDV
	std::vector<std::shared_ptr<CFont>> fonts_;

// ����������
protected:
	HICON m_hIcon;

	// ��������� ������� ����� ���������
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CEdit edt_;
	CString txt_;
	afx_msg void OnBnClickedButton2();
};
