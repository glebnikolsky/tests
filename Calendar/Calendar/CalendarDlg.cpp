
// CalendarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Calendar.h"
#include "CaldrCtrl.h"
#include "CalendarDlg.h"
#include "afxdialogex.h"
#include <sstream>
#include <string>
#include <vector>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

std::string ShowTime( SYSTEMTIME const &time )
{
	std::ostringstream os;
	os << time.wYear << ":" << time.wDay << ":" << time.wMonth;
	return os.str();
}
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCalendarDlg dialog



CCalendarDlg::CCalendarDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CALENDAR_DIALOG, pParent)
	, dt_( _T( "" ) )
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCalendarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange( pDX );
	DDX_Control( pDX, IDC_MONTHCALENDAR1, caldr_ );
	DDX_Text( pDX, IDC_EDIT1, dt_ );
}

BEGIN_MESSAGE_MAP(CCalendarDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED( IDC_BUTTON1, &CCalendarDlg::OnBnClickedButton1 )
END_MESSAGE_MAP()


// CCalendarDlg message handlers

BOOL CCalendarDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here


	// Allocate the state array based on the return value.

	caldr_.SetDate( _T("2023.09.01") ).SetDate( _T( "2023.09.02") );
	caldr_.SetDate( _T( "2023.09.04") ).SetDate( _T( "2023.09.05"));
	caldr_.UpdateState();
	caldr_.RemoveDate( _T( "2023.09.01" ) );
	//CTime tmp;
	//caldr_.GetCurSel( tmp );
	//caldr_.SetCurSel( tmp );

	UpdateData( TRUE );

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCalendarDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCalendarDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCalendarDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}





void CCalendarDlg::OnBnClickedButton1()
{
	UpdateData(TRUE);
	if ( dt_.Find( ";" ) != -1 ){
		int i{0};
		CString tmp;
		std::vector<TCHAR*> v;
		while ( (tmp = dt_.Tokenize( ";", i )) != "" ){
			caldr_.SetDate( tmp.GetString() );
		}
		
	}
	else{
		caldr_.SetDate( dt_.GetString() );
	}
}
