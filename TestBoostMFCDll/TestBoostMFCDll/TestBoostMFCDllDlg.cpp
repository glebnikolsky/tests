
// TestBoostMFCDllDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestBoostMFCDll.h"
#include "TestBoostMFCDllDlg.h"
#include "afxdialogex.h"
#include "..\MFCLibrary1\TestDll.h"
#include <boost/log/trivial.hpp>
#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_file_backend.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTestBoostMFCDllDlg dialog



CTestBoostMFCDllDlg::CTestBoostMFCDllDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestBoostMFCDllDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestBoostMFCDllDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTestBoostMFCDllDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDOK, &CTestBoostMFCDllDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CTestBoostMFCDllDlg message handlers

BOOL CTestBoostMFCDllDlg::OnInitDialog()
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

    typedef boost::log::sinks::synchronous_sink< boost::log::sinks::text_file_backend > file_sink;
    boost::shared_ptr< file_sink > sink(new file_sink(
        boost::log::keywords::file_name = _T("qqqqq%Y%m%d_%H%M%S.log"), //При каждом запуске новый файл с датой и временем в имени
        boost::log::keywords::rotation_size = 16*1024*1024                   // максимальный размер
        ));
    sink->locked_backend()->set_file_collector(boost::log::sinks::file::make_collector(
        boost::log::keywords::target = ".\\log",
        boost::log::keywords::max_size = 64 * 1024 * 1024,                          // максимальный суммарный объём
        boost::log::keywords::min_free_space = 100 * 1024 * 1024                    // минимальное свободное место на диске - не проверял - что и как он сообщит
        // проверю позже, скорее всего бросит исключение
        ));

    sink->locked_backend()->auto_flush(true);
    sink->locked_backend()->scan_for_files();                                       // посмотрит - не стереть ли чего

    boost::log::core::get()->set_filter
        (
        boost::log::trivial::severity >= 0
        );
    boost::log::core::get()->add_sink(sink);

    boost::log::sources::severity_logger< boost::log::trivial::severity_level > lg;
    BOOST_LOG_SEV(lg, boost::log::trivial::trace) << __FILE__<<"End of init  "<<__FUNCTION__<<" "<<__LINE__;

    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTestBoostMFCDllDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTestBoostMFCDllDlg::OnPaint()
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
HCURSOR CTestBoostMFCDllDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTestBoostMFCDllDlg::OnBnClickedOk()
{
    // TODO: Add your control notification handler code here
    boost::log::sources::severity_logger< boost::log::trivial::severity_level > lg;
    BOOST_LOG_SEV(lg, boost::log::trivial::trace) << __FILE__<<"OK  "<<__FUNCTION__<<" "<<__LINE__;
    test1();
    test2();
    CDialogEx::OnOK();
}
