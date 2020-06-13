// XBrowseForFolderTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "XBrowseForFolderTest.h"
#include "XBrowseForFolderTestDlg.h"
#include "XBrowseForFolder.h"
#include "about.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const CXBrowseForFolderTestDlg::CSIDL CXBrowseForFolderTestDlg::m_nCsidl[] =
{
	{ _T("CSIDL_DESKTOP"),                   0x0000 },  // <desktop>
	{ _T("CSIDL_ADMINTOOLS"),                0x0030 },  // <user name>\Start Menu\Programs\Administrative Tools
	{ _T("CSIDL_ALTSTARTUP"),                0x001d },  // non localized startup
	{ _T("CSIDL_APPDATA"),                   0x001a },  // <user name>\Application Data
	{ _T("CSIDL_BITBUCKET"),                 0x000a },  // <desktop>\Recycle Bin
	{ _T("CSIDL_CDBURN_AREA"),               0x003b },  // USERPROFILE\Local Settings\Application Data\Microsoft\CD Burning
	{ _T("CSIDL_COMMON_ADMINTOOLS"),         0x002f },  // All Users\Start Menu\Programs\Administrative Tools
	{ _T("CSIDL_COMMON_ALTSTARTUP"),         0x001e },  // non localized common startup
	{ _T("CSIDL_COMMON_APPDATA"),            0x0023 },  // All Users\Application Data
	{ _T("CSIDL_COMMON_DESKTOPDIRECTORY"),   0x0019 },  // All Users\Desktop
	{ _T("CSIDL_COMMON_DOCUMENTS"),          0x002e },  // All Users\Documents
	{ _T("CSIDL_COMMON_FAVORITES"),          0x001f },
	{ _T("CSIDL_COMMON_MUSIC"),              0x0035 },  // All Users\My Music
	{ _T("CSIDL_COMMON_OEM_LINKS"),          0x003a },  // Links to All Users OEM specific apps
	{ _T("CSIDL_COMMON_PICTURES"),           0x0036 },  // All Users\My Pictures
	{ _T("CSIDL_COMMON_PROGRAMS"),           0X0017 },  // All Users\Start Menu\Programs
	{ _T("CSIDL_COMMON_STARTMENU"),          0x0016 },  // All Users\Start Menu
	{ _T("CSIDL_COMMON_STARTUP"),            0x0018 },  // All Users\Startup
	{ _T("CSIDL_COMMON_TEMPLATES"),          0x002d },  // All Users\Templates
	{ _T("CSIDL_COMMON_VIDEO"),              0x0037 },  // All Users\My Video
	{ _T("CSIDL_COMPUTERSNEARME"),           0x003d },  // Computers Near Me (computered from Workgroup membership)
	{ _T("CSIDL_CONNECTIONS"),               0x0031 },  // Network and Dial-up Connections
	{ _T("CSIDL_CONTROLS"),                  0x0003 },  // My Computer\Control Panel
	{ _T("CSIDL_COOKIES"),                   0x0021 },
	{ _T("CSIDL_DESKTOPDIRECTORY"),          0x0010 },  // <user name>\Desktop
	{ _T("CSIDL_DRIVES"),                    0x0011 },  // My Computer
	{ _T("CSIDL_FAVORITES"),                 0x0006 },  // <user name>\Favorites
	{ _T("CSIDL_FONTS"),                     0x0014 },  // windows\fonts
	{ _T("CSIDL_HISTORY"),                   0x0022 },
	{ _T("CSIDL_INTERNET"),                  0x0001 },  // Internet Explorer (icon on desktop)
	{ _T("CSIDL_INTERNET_CACHE"),            0x0020 },
	{ _T("CSIDL_LOCAL_APPDATA"),             0x001c },  // <user name>\Local Settings\Applicaiton Data (non roaming)
	{ _T("CSIDL_MYDOCUMENTS"),               0x000c },  // logical "My Documents" desktop icon
	{ _T("CSIDL_MYMUSIC"),                   0x000d },  // "My Music" folder
	{ _T("CSIDL_MYPICTURES"),                0x0027 },  // C:\Program Files\My Pictures
	{ _T("CSIDL_MYVIDEO"),                   0x000e },  // "My Videos" folder
	{ _T("CSIDL_NETHOOD"),                   0x0013 },  // <user name>\nethood
	{ _T("CSIDL_NETWORK"),                   0x0012 },  // Network Neighborhood (My Network Places)
	{ _T("CSIDL_PERSONAL"),                  0x0005 },  // My Documents
	{ _T("CSIDL_PRINTERS"),                  0x0004 },  // My Computer\Printers
	{ _T("CSIDL_PRINTHOOD"),                 0x001b },  // <user name>\PrintHood
	{ _T("CSIDL_PROFILE"),                   0x0028 },  // USERPROFILE
	{ _T("CSIDL_PROGRAMS"),                  0x0002 },  // Start Menu\Programs
	{ _T("CSIDL_PROGRAM_FILES"),             0x0026 },  // C:\Program Files
	{ _T("CSIDL_PROGRAM_FILESX86"),          0x002a },  // x86 C:\Program Files on RISC
	{ _T("CSIDL_PROGRAM_FILES_COMMON"),      0x002b },  // C:\Program Files\Common
	{ _T("CSIDL_PROGRAM_FILES_COMMONX86"),   0x002c },  // x86 Program Files\Common on RISC
	{ _T("CSIDL_RECENT"),                    0x0008 },  // <user name>\Recent
	{ _T("CSIDL_RESOURCES"),                 0x0038 },  // Resource Direcotry
	{ _T("CSIDL_RESOURCES_LOCALIZED"),       0x0039 },  // Localized Resource Direcotry
	{ _T("CSIDL_SENDTO"),                    0x0009 },  // <user name>\SendTo
	{ _T("CSIDL_STARTMENU"),                 0x000b },  // <user name>\Start Menu
	{ _T("CSIDL_STARTUP"),                   0x0007 },  // Start Menu\Programs\Startup
	{ _T("CSIDL_SYSTEM"),                    0x0025 },  // GetSystemDirectory()
	{ _T("CSIDL_SYSTEMX86"),                 0x0029 },  // x86 system directory on RISC
	{ _T("CSIDL_TEMPLATES"),                 0x0015 },
	{ _T("CSIDL_WINDOWS"),                   0x0024 },  // GetWindowsDirectory()
	{ NULL,                                  0 }
};

/////////////////////////////////////////////////////////////////////////////
// CXBrowseForFolderTestDlg dialog

BEGIN_MESSAGE_MAP(CXBrowseForFolderTestDlg, CDialog)
	//{{AFX_MSG_MAP(CXBrowseForFolderTestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnSHBrowseForFolder)
	ON_BN_CLICKED(IDC_BUTTON2, OnXBrowseForFolder)
	ON_BN_CLICKED(IDC_INITIAL, OnInitial)
	ON_BN_CLICKED(IDC_INITIAL2, OnInitial)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CXBrowseForFolderTestDlg::CXBrowseForFolderTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CXBrowseForFolderTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CXBrowseForFolderTestDlg)
	m_bEditBox = FALSE;
	m_nRootIndex = 0;
	m_nInitialIndex = 0;
	m_nInitial = 0;
	m_strInitialFolder = _T("");
	m_strCaption = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CXBrowseForFolderTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXBrowseForFolderTestDlg)
	DDX_Control(pDX, IDC_STATIC1, m_staticAsterisk1);
	DDX_Control(pDX, IDC_STATIC2, m_staticAsterisk2);
	DDX_Control(pDX, IDC_STATIC3, m_staticAsterisk3);
	DDX_Control(pDX, IDC_STATIC4, m_staticAsterisk4);
	DDX_Control(pDX, IDC_STATIC5, m_staticAsterisk5);
	DDX_Control(pDX, IDC_INITIAL_CSIDL, m_comboInitial);
	DDX_Control(pDX, IDC_ROOT, m_comboRoot);
	DDX_Control(pDX, IDC_RETURN_CODE, m_edtReturnCode);
	DDX_Control(pDX, IDC_FOLDER, m_edtFolder);
	DDX_Check(pDX, IDC_INCLUDE_EDIT, m_bEditBox);
	DDX_CBIndex(pDX, IDC_ROOT, m_nRootIndex);
	DDX_Text(pDX, IDC_INITIAL_FOLDER, m_strInitialFolder);
	DDX_Text(pDX, IDC_CAPTION, m_strCaption);
	DDX_CBIndex(pDX, IDC_INITIAL_CSIDL, m_nInitialIndex);
	DDX_Radio(pDX, IDC_INITIAL, m_nInitial);
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CXBrowseForFolderTestDlg message handlers

BOOL CXBrowseForFolderTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
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

	for (int i = 0; m_nCsidl[i].pszCsidl != NULL; i++)
	{
		m_comboRoot.AddString(m_nCsidl[i].pszCsidl);
		m_comboInitial.AddString(m_nCsidl[i].pszCsidl);
	}
	
	m_comboRoot.SetCurSel(0);
	int index = m_comboInitial.FindStringExact(-1, _T("CSIDL_WINDOWS"));
	m_comboInitial.SetCurSel(index);

	OnInitial();

	m_staticAsterisk1.SetFont(NULL, 12, FALSE);
	m_staticAsterisk1.SetTextColor(RGB(255,0,0), FALSE);
	m_staticAsterisk2.SetFont(NULL, 12, FALSE);
	m_staticAsterisk2.SetTextColor(RGB(255,0,0), FALSE);
	m_staticAsterisk3.SetFont(NULL, 12, FALSE);
	m_staticAsterisk3.SetTextColor(RGB(255,0,0), FALSE);
	m_staticAsterisk4.SetFont(NULL, 12, FALSE);
	m_staticAsterisk4.SetTextColor(RGB(255,0,0), FALSE);
	m_staticAsterisk5.SetFont(GetFont(), FALSE);			// "= XBrowseForFolder only"
	m_staticAsterisk5.SetTextColor(RGB(255,0,0), FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CXBrowseForFolderTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CXBrowseForFolderTestDlg::OnPaint() 
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

HCURSOR CXBrowseForFolderTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CXBrowseForFolderTestDlg::OnSHBrowseForFolder() 
{
	UpdateData(TRUE);

	m_edtReturnCode.SetWindowText(_T(""));
	m_edtFolder.SetWindowText(_T(""));

	BROWSEINFO bi = { 0 };

	bi.hwndOwner = m_hWnd;
	bi.ulFlags   = BIF_RETURNONLYFSDIRS;
	if (m_bEditBox)
		bi.ulFlags |= BIF_EDITBOX;
	bi.ulFlags |= BIF_NONEWFOLDERBUTTON;
	int nRootFolder = m_nCsidl[m_nRootIndex].nCsidl;
	LPITEMIDLIST pidlRoot = NULL;
	if (SUCCEEDED(SHGetSpecialFolderLocation(m_hWnd, nRootFolder, &pidlRoot)))
		bi.pidlRoot = pidlRoot;

	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);

	BOOL bRet = FALSE;

	TCHAR szFolder[MAX_PATH*2] = { _T('\0') };

	if (pidl)
	{
		if (SHGetPathFromIDList(pidl, szFolder))
		{
			bRet = TRUE;
		}

		IMalloc *pMalloc = NULL; 
		if (SUCCEEDED(SHGetMalloc(&pMalloc)) && pMalloc) 
		{  
			pMalloc->Free(pidl);  
			pMalloc->Release(); 
		}
	}

	m_edtReturnCode.SetWindowText(bRet ? _T("TRUE") : _T("FALSE"));
	m_edtFolder.SetWindowText(szFolder);
}

void CXBrowseForFolderTestDlg::OnXBrowseForFolder() 
{
	UpdateData(TRUE);

	m_edtReturnCode.SetWindowText(_T(""));
	m_edtFolder.SetWindowText(_T(""));

	TCHAR szFolder[MAX_PATH*2] = { _T('\0') };

	LPCTSTR lpszInitial = m_strInitialFolder;
	if (m_nInitial)
	{
		int nFolder = m_nCsidl[m_nInitialIndex].nCsidl;
		TCHAR szInitialPath[MAX_PATH*2] = { _T('\0') };
		SHGetSpecialFolderPath(m_hWnd, szInitialPath, nFolder, FALSE);
		if (szInitialPath[0] == _T('\0'))
		{
			AfxMessageBox(_T("The initial CSIDL must be a real folder.  \r\n\r\n")
						  _T("Please select another CSIDL."),
						  MB_OK);
			return;
		}
		lpszInitial = (LPCTSTR) nFolder;
	}

	BOOL bRet = XBrowseForFolder(m_hWnd,
								 lpszInitial,
								 m_nCsidl[m_nRootIndex].nCsidl,
								 m_strCaption,
								 szFolder,
								 sizeof(szFolder)/sizeof(TCHAR)-2,
								 m_bEditBox);

	m_edtReturnCode.SetWindowText(bRet ? _T("TRUE") : _T("FALSE"));
	m_edtFolder.SetWindowText(szFolder);
}

void CXBrowseForFolderTestDlg::OnInitial() 
{
	UpdateData(TRUE);

	if (m_nInitial)
	{
		GetDlgItem(IDC_INITIAL_FOLDER)->EnableWindow(FALSE);
		m_comboInitial.EnableWindow(TRUE);
	}
	else
	{
		m_comboInitial.EnableWindow(FALSE);
		GetDlgItem(IDC_INITIAL_FOLDER)->EnableWindow(TRUE);
	}
}
