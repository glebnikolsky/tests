
// Librarian.cpp: îïðåäåëÿåò ïîâåäåíèå êëàññîâ äëÿ ïðèëîæåíèÿ.
//

#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "Librarian.h"
#include "MainFrm.h"

#include "LibrarianDoc.h"
#include "LibrarianView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLibrarianApp

BEGIN_MESSAGE_MAP(CLibrarianApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CLibrarianApp::OnAppAbout)
	// Ñòàíäàðòíûå êîìàíäû ïî ðàáîòå ñ ôàéëàìè äîêóìåíòîâ
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// Ñòàíäàðòíàÿ êîìàíäà íàñòðîéêè ïå÷àòè
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// Ñîçäàíèå CLibrarianApp

CLibrarianApp::CLibrarianApp() noexcept
{
	m_bHiColorIcons = TRUE;


	m_nAppLook = 0;
	// ïîääåðæêà äèñïåò÷åðà ïåðåçàãðóçêè
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// Åñëè ïðèëîæåíèå ïîñòðîåíî ñ ïîääåðæêîé ñðåäû Common Language Runtime (/clr):
	//     1) Ýòîò äîïîëíèòåëüíûé ïàðàìåòð òðåáóåòñÿ äëÿ ïðàâèëüíîé ïîääåðæêè ðàáîòû äèñïåò÷åðà ïåðåçàãðóçêè.
	//   2) Â ñâîåì ïðîåêòå äëÿ ñáîðêè íåîáõîäèìî äîáàâèòü ññûëêó íà System.Windows.Forms.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: çàìåíèòå íèæå ñòðîêó èäåíòèôèêàòîðà ïðèëîæåíèÿ ñòðîêîé óíèêàëüíîãî èäåíòèôèêàòîðà; ðåêîìåíäóåìûé
	// ôîðìàò äëÿ ñòðîêè: ÈìÿÊîìïàíèè.ÈìÿÏðîäóêòà.ÑóáÏðîäóêò.ÑâåäåíèÿÎÂåðñèè
	SetAppID(_T("Librarian.AppID.NoVersion"));

	// TODO: äîáàâüòå êîä ñîçäàíèÿ,
	// Ðàçìåùàåò âåñü âàæíûé êîä èíèöèàëèçàöèè â InitInstance
}

// Åäèíñòâåííûé îáúåêò CLibrarianApp

CLibrarianApp theApp;


// Èíèöèàëèçàöèÿ CLibrarianApp

BOOL CLibrarianApp::InitInstance()
{
	// InitCommonControlsEx() òðåáóþòñÿ äëÿ Windows XP, åñëè ìàíèôåñò
	// ïðèëîæåíèÿ èñïîëüçóåò ComCtl32.dll âåðñèè 6 èëè áîëåå ïîçäíåé âåðñèè äëÿ âêëþ÷åíèÿ
	// ñòèëåé îòîáðàæåíèÿ.  Â ïðîòèâíîì ñëó÷àå áóäåò âîçíèêàòü ñáîé ïðè ñîçäàíèè ëþáîãî îêíà.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Âûáåðèòå ýòîò ïàðàìåòð äëÿ âêëþ÷åíèÿ âñåõ îáùèõ êëàññîâ óïðàâëåíèÿ, êîòîðûå íåîáõîäèìî èñïîëüçîâàòü
	// â âàøåì ïðèëîæåíèè.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// Èíèöèàëèçàöèÿ áèáëèîòåê OLE
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// Äëÿ èñïîëüçîâàíèÿ ýëåìåíòà óïðàâëåíèÿ RichEdit òðåáóåòñÿ ìåòîä AfxInitRichEdit2()
	// AfxInitRichEdit2();

	// Ñòàíäàðòíàÿ èíèöèàëèçàöèÿ
	// Åñëè ýòè âîçìîæíîñòè íå èñïîëüçóþòñÿ è íåîáõîäèìî óìåíüøèòü ðàçìåð
	// êîíå÷íîãî èñïîëíÿåìîãî ôàéëà, íåîáõîäèìî óäàëèòü èç ñëåäóþùåãî
	// êîíêðåòíûå ïðîöåäóðû èíèöèàëèçàöèè, êîòîðûå íå òðåáóþòñÿ
	// Èçìåíèòå ðàçäåë ðååñòðà, â êîòîðîì õðàíÿòñÿ ïàðàìåòðû
	// TODO: ñëåäóåò èçìåíèòü ýòó ñòðîêó íà ÷òî-íèáóäü ïîäõîäÿùåå,
	// íàïðèìåð íà íàçâàíèå îðãàíèçàöèè
	SetRegistryKey(_T("Ëîêàëüíûå ïðèëîæåíèÿ, ñîçäàííûå ñ ïîìîùüþ ìàñòåðà ïðèëîæåíèé"));
	LoadStdProfileSettings(4);  // Çàãðóçèòå ñòàíäàðòíûå ïàðàìåòðû INI-ôàéëà (âêëþ÷àÿ MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Çàðåãèñòðèðóéòå øàáëîíû äîêóìåíòîâ ïðèëîæåíèÿ.  Øàáëîíû äîêóìåíòîâ
	//  âûñòóïàþò â ðîëè ïîñðåäíèêà ìåæäó äîêóìåíòàìè, îêíàìè ðàìîê è ïðåäñòàâëåíèÿìè
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CLibrarianDoc),
		RUNTIME_CLASS(CMainFrame),       // îñíîâíîå îêíî ðàìêè SDI
		RUNTIME_CLASS(CLibrarianView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// Ðàçðåøèòü èñïîëüçîâàíèå ðàñøèðåííûõ ñèìâîëîâ â ãîðÿ÷èõ êëàâèøàõ ìåíþ
	CMFCToolBar::m_bExtCharTranslation = TRUE;

	// Ñèíòàêñè÷åñêèé ðàçáîð êîìàíäíîé ñòðîêè íà ñòàíäàðòíûå êîìàíäû îáîëî÷êè, DDE, îòêðûòèå ôàéëîâ
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// Êîìàíäû äèñïåò÷åðèçàöèè, óêàçàííûå â êîìàíäíîé ñòðîêå.  Çíà÷åíèå FALSE áóäåò âîçâðàùåíî, åñëè
	// ïðèëîæåíèå áûëî çàïóùåíî ñ ïàðàìåòðîì /RegServer, /Register, /Unregserver èëè /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// Îäíî è òîëüêî îäíî îêíî áûëî èíèöèàëèçèðîâàíî, ïîýòîìó îòîáðàçèòå è îáíîâèòå åãî
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}

int CLibrarianApp::ExitInstance()
{
	//TODO: îáðàáîòàéòå äîïîëíèòåëüíûå ðåñóðñû, êîòîðûå ìîãëè áûòü äîáàâëåíû
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// Îáðàáîò÷èêè ñîîáùåíèé CLibrarianApp


// Äèàëîãîâîå îêíî CAboutDlg èñïîëüçóåòñÿ äëÿ îïèñàíèÿ ñâåäåíèé î ïðèëîæåíèè

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

// Äàííûå äèàëîãîâîãî îêíà
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // ïîääåðæêà DDX/DDV

// Ðåàëèçàöèÿ
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// Êîìàíäà ïðèëîæåíèÿ äëÿ çàïóñêà äèàëîãà
void CLibrarianApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// Ìåòîäû çàãðóçêè èëè ñîõðàíåíèÿ íàñòðîåê CLibrarianApp

void CLibrarianApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void CLibrarianApp::LoadCustomState()
{
}

void CLibrarianApp::SaveCustomState()
{
}

// Îáðàáîò÷èêè ñîîáùåíèé CLibrarianApp



