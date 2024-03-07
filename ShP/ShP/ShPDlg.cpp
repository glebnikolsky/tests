
// ShPDlg.cpp : ���� ����������
//

#include "stdafx.h"
#include "ShP.h"
#include "ShPDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

struct DBInfo{
	DBInfo( const CString &dbname, const CString &auth, int authNo = 0 ):
	authNo_( authNo )
	, isERBD_( true )          //���������� ������ ��� �������� �� �� �������
	, isSchema_( false )
	, isEmpty_( false )
	, isDepartment_( false )
	, shemeID_( 0.0 )
	, schemeDate_( 0 )
	, existingObj_( 0 )
	, chem_on_( 0 )
	, need_update_db_( false )
	, need_setup_control_( false )
	, no_(NextId())
{
	//no_ = NextId();
	txt_[0] = dbname;
	txt_[8] = _T( "" );
	txt_[2] = _T( "�����������" );
	txt_[3] = auth;
	txt_[7] = txt_[4] = txt_[5] = txt_[6] = _T( "0" );
	txt_[9] = _T( "�� ������" );
	txt_[10] = _T( "�� �������" );
}
#pragma warning(pop)
long	authNo_;
bool	isERBD_;
bool	isSchema_;
bool    isEmpty_;
bool    isDepartment_;
double  shemeID_;
long	schemeDate_;
long    existingObj_;
long    chem_on_;
bool    need_update_db_;
bool    need_setup_control_;
long	no_;
static  long id_;
CString txt_[13];		//0 ��� ���� ������
						//1 ��� �����������������
						//2 �����
						//3 �����������
						//4 �������
						//5 ������� �2
						//6 ������� �����������
						//7 ������
						//8 �������������
						//9 ��� ��
						//10 ������ ��
						//11 guid
						//12 chem ��� 0 /��� 1/���� -1
bool CanDel(){ return txt_[6] == "0" || txt_[6] == _T( "����������� [dbo].[res_HumanTests]" ); }
bool WarnDel(){ return txt_[5] != "0" && txt_[5] != _T( "����������� [dbo].[sht_Sheets_R]" ); }
long NextId(){
	return ++id_;
}
	};
long DBInfo::id_ = 0;
// ���������� ���� CAboutDlg ������������ ��� �������� �������� � ����������

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ������ ����������� ����
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // ��������� DDX/DDV

// ����������
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


// ���������� ���� CShPDlg



CShPDlg::CShPDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SHP_DIALOG, pParent)
	, txt_( _T( "SOme shit ������ ����" ) )
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CShPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange( pDX );
	DDX_Control( pDX, IDC_EDIT1, edt_ );
	DDX_Text( pDX, IDC_EDIT1, txt_ );
}

BEGIN_MESSAGE_MAP(CShPDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED( IDC_BUTTON1, &CShPDlg::OnBnClickedButton1 )
	ON_BN_CLICKED( IDC_BUTTON2, &CShPDlg::OnBnClickedButton2 )
END_MESSAGE_MAP()


// ����������� ��������� CShPDlg

BOOL CShPDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���������� ������ "� ���������..." � ��������� ����.

	// IDM_ABOUTBOX ������ ���� � �������� ��������� �������.
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

	// ������ ������ ��� ����� ����������� ����.  ����� ������ ��� �������������,
	//  ���� ������� ���� ���������� �� �������� ����������
	SetIcon(m_hIcon, TRUE);			// ������� ������
	SetIcon(m_hIcon, FALSE);		// ������ ������

	// TODO: �������� �������������� �������������

	return TRUE;  // ������� �������� TRUE, ���� ����� �� ������� �������� ����������
}

void CShPDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// ��� ���������� ������ ����������� � ���������� ���� ����� ��������������� ����������� ���� �����,
//  ����� ���������� ������.  ��� ���������� MFC, ������������ ������ ���������� ��� �������������,
//  ��� ������������� ����������� ������� ��������.

void CShPDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �������� ���������� ��� ���������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ������������ ������ �� ������ ����������� ��������������
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ��������� ������
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ������� �������� ��� ������� ��� ��������� ����������� ������� ��� �����������
//  ���������� ����.
HCURSOR CShPDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CShPDlg::OnBnClickedButton1()
{
	LOGFONT lf;
	CFontDialog f_dlg;
	if ( f_dlg.DoModal() != IDOK ) return;
	f_dlg.GetCurrentFont( &lf );
	for ( auto i : fonts_ ){
		LOGFONT tmp{0};
		i->GetLogFont( &tmp );
		if ( !memcmp(&tmp, &lf, sizeof(LOGFONT))) return;
	}
	std::shared_ptr<CFont> new_fnt = std::make_shared<CFont>( );
	new_fnt->CreateFontIndirect( &lf );
	fonts_.push_back( new_fnt );
	edt_.SetFont( new_fnt.get(), TRUE );
	
}


void CShPDlg::OnBnClickedButton2()
{
	std::vector<DBInfo> tmp;
	tmp.emplace_back( "a", "b" );
	tmp.emplace_back( "c", "b" );
	tmp.emplace_back( "d", "b" );
	tmp.emplace_back( "f", "b" );
	tmp.emplace_back( "g", "b" );
	tmp.emplace_back( "t", "b" );
	tmp.emplace_back( "v", "b" );
	tmp.emplace_back( "d", "b" );
}
