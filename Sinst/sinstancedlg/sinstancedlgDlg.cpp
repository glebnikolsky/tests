#include "stdafx.h"
#include "sinstancedlg.h"
#include "sinstancedlgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CSinstancedlgDlg::CSinstancedlgDlg(CWnd* pParent)	: CDialog(CSinstancedlgDlg::IDD, pParent), 
                                                    m_InstanceChecker(_T("CInstanceChecker Example Dialog App")),
                                                    m_hIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME))
{
}

void CSinstancedlgDlg::DoDataExchange(CDataExchange* pDX)
{
  //Let the base class do its thing
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSinstancedlgDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()

BOOL CSinstancedlgDlg::OnInitDialog()
{
  //Let the base class do its thing
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);	 //Set big icon
	SetIcon(m_hIcon, FALSE); //Set small icon

  //track this instance so any other instances can find us.
  m_InstanceChecker.TrackFirstInstanceRunning();
	
	return TRUE; //return TRUE  unless you set the focus to a control
}

void CSinstancedlgDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); //device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		//Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		//Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CSinstancedlgDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
