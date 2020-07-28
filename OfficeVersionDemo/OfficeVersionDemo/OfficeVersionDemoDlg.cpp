// OfficeVersionDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OfficeVersionDemo.h"
#include "OfficeVersionDemoDlg.h"
#include "OfficeVersion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COfficeVersionDemoDlg dialog



COfficeVersionDemoDlg::COfficeVersionDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COfficeVersionDemoDlg::IDD, pParent)
	, m_resultsString(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COfficeVersionDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_RESULTS, m_resultsString);
}

BEGIN_MESSAGE_MAP(COfficeVersionDemoDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// COfficeVersionDemoDlg message handlers

BOOL COfficeVersionDemoDlg::OnInitDialog()
{
	m_resultsString = ShowResults();

	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	return TRUE;  // return TRUE  unless you set the focus to a control
}

CString COfficeVersionDemoDlg::ShowResults()
{
	// Note:
	// Aside from [#include "OfficeVersion.h"] at the top of this file
	// all interesting code is in this function

	CString returnString( _T("Office Version: ") );
	returnString += OfficeVersion::GetVersionAsString( OfficeVersion::GetOfficeVersion() );
	returnString += _T("\r\n");

	static const OfficeVersion::eOfficeApp appsToCheck[] = {	
		OfficeVersion::eOfficeApp_Word,
		OfficeVersion::eOfficeApp_Excel,
		OfficeVersion::eOfficeApp_Outlook,
		OfficeVersion::eOfficeApp_Access,
		OfficeVersion::eOfficeApp_PowerPoint,
	};
	const int numItems( sizeof(appsToCheck) / sizeof(appsToCheck[0]) );
	
	for(int i=0; i<numItems; ++i){
		returnString += _T("\r\n");
		returnString += OfficeVersion::GetApplicationAsString(appsToCheck[i]);
		returnString += _T(": ");
		returnString += OfficeVersion::GetVersionAsString( OfficeVersion::GetApplicationVersion(appsToCheck[i]) );
	}

	return returnString;
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void COfficeVersionDemoDlg::OnPaint() 
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
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR COfficeVersionDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
