
// ShPDlg.h : файл заголовка
//

#pragma once
#include "afxwin.h"


// диалоговое окно CShPDlg
class CShPDlg : public CDialogEx
{
// Создание
public:
	CShPDlg(CWnd* pParent = NULL);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SHP_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV
	std::vector<std::shared_ptr<CFont>> fonts_;

// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
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
