// CheckComboDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CheckCombo.h"
#include "CheckComboDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCheckComboDlg dialog

CCheckComboDlg::CCheckComboDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCheckComboDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCheckComboDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCheckComboDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCheckComboDlg)
	DDX_Control(pDX, IDC_COMBO, m_Combo);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCheckComboDlg, CDialog)
	//{{AFX_MSG_MAP(CCheckComboDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheckComboDlg message handlers

BOOL CCheckComboDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// Add the colors
	m_Combo.AddString("Red");
	m_Combo.AddString("Green");
	m_Combo.AddString("Blue");
	m_Combo.AddString("Black");
	m_Combo.AddString("White");
	m_Combo.AddString("Yellow");
	m_Combo.AddString("Brown");

	// Select some colors
	m_Combo.SetCheck(0, TRUE);
	m_Combo.SetCheck(1, TRUE);
	m_Combo.SetCheck(6, TRUE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCheckComboDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCheckComboDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CCheckComboDlg::OnOK()
{
	CString res;
	for (int i = 0; i < m_Combo.GetCount(); ++i) {
		bool b = m_Combo.GetItemData(i);
		if (b) {
			CString str;
			m_Combo.GetLBText(i, str);
			res += str + _T("\n");
		}
	}

}