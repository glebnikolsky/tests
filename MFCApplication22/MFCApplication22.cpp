
// MFCApplication22.cpp: îïðåäåëÿåò ïîâåäåíèå êëàññîâ äëÿ ïðèëîæåíèÿ.
//

#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "MFCApplication22.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "MFCApplication22Doc.h"
#include "MFCApplication22View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCApplication22App

BEGIN_MESSAGE_MAP(CMFCApplication22App, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CMFCApplication22App::OnAppAbout)
	// Ñòàíäàðòíûå êîìàíäû ïî ðàáîòå ñ ôàéëàìè äîêóìåíòîâ
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// Ñòàíäàðòíàÿ êîìàíäà íàñòðîéêè ïå÷àòè
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// Ñîçäàíèå CMFCApplication22App

CMFCApplication22App::CMFCApplication22App() noexcept
{

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
	SetAppID(_T("MFCApplication22.AppID.NoVersion"));

	// TODO: äîáàâüòå êîä ñîçäàíèÿ,
	// Ðàçìåùàåò âåñü âàæíûé êîä èíèöèàëèçàöèè â InitInstance
}

// Åäèíñòâåííûé îáúåêò CMFCApplication22App

CMFCApplication22App theApp;


// Èíèöèàëèçàöèÿ CMFCApplication22App

BOOL CMFCApplication22App::InitInstance()
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

	CWinApp::InitInstance();


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


	// Çàðåãèñòðèðóéòå øàáëîíû äîêóìåíòîâ ïðèëîæåíèÿ.  Øàáëîíû äîêóìåíòîâ
	//  âûñòóïàþò â ðîëè ïîñðåäíèêà ìåæäó äîêóìåíòàìè, îêíàìè ðàìîê è ïðåäñòàâëåíèÿìè
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_MFCApplication22TYPE,
		RUNTIME_CLASS(CMFCApplication22Doc),
		RUNTIME_CLASS(CChildFrame), // íàñòðàèâàåìàÿ äî÷åðíÿÿ ðàìêà MDI
		RUNTIME_CLASS(CMFCApplication22View));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// ñîçäàéòå ãëàâíîå îêíî ðàìêè MDI
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;


	// Ðàçðåøèòü èñïîëüçîâàíèå ðàñøèðåííûõ ñèìâîëîâ â ãîðÿ÷èõ êëàâèøàõ ìåíþ
	CMFCToolBar::m_bExtCharTranslation = TRUE;

	// Ñèíòàêñè÷åñêèé ðàçáîð êîìàíäíîé ñòðîêè íà ñòàíäàðòíûå êîìàíäû îáîëî÷êè, DDE, îòêðûòèå ôàéëîâ
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// Êîìàíäû äèñïåò÷åðèçàöèè, óêàçàííûå â êîìàíäíîé ñòðîêå.  Çíà÷åíèå FALSE áóäåò âîçâðàùåíî, åñëè
	// ïðèëîæåíèå áûëî çàïóùåíî ñ ïàðàìåòðîì /RegServer, /Register, /Unregserver èëè /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// Ãëàâíîå îêíî áûëî èíèöèàëèçèðîâàíî, ïîýòîìó îòîáðàçèòå è îáíîâèòå åãî
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}

int CMFCApplication22App::ExitInstance()
{
	//TODO: îáðàáîòàéòå äîïîëíèòåëüíûå ðåñóðñû, êîòîðûå ìîãëè áûòü äîáàâëåíû
	AfxOleTerm(FALSE);

	return CWinApp::ExitInstance();
}

// Îáðàáîò÷èêè ñîîáùåíèé CMFCApplication22App


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
void CMFCApplication22App::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// Îáðàáîò÷èêè ñîîáùåíèé CMFCApplication22App



