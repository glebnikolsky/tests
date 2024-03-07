
#include "pch.h"
#include "framework.h"

#include "PropertiesWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "Librarian.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

CPropertiesWnd::CPropertiesWnd() noexcept
{
	m_nComboHeight = 0;
}

CPropertiesWnd::~CPropertiesWnd()
{
}

BEGIN_MESSAGE_MAP(CPropertiesWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_EXPAND_ALL, OnExpandAllProperties)
	ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, OnUpdateExpandAllProperties)
	ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
	ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)
	ON_COMMAND(ID_PROPERTIES1, OnProperties1)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES1, OnUpdateProperties1)
	ON_COMMAND(ID_PROPERTIES2, OnProperties2)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES2, OnUpdateProperties2)
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// îáðàáîò÷èêè ñîîáùåíèé CResourceViewBar

void CPropertiesWnd::AdjustLayout()
{
	if (GetSafeHwnd () == nullptr || (AfxGetMainWnd() != nullptr && AfxGetMainWnd()->IsIconic()))
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndObjectCombo.SetWindowPos(nullptr, rectClient.left, rectClient.top, rectClient.Width(), m_nComboHeight, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndToolBar.SetWindowPos(nullptr, rectClient.left, rectClient.top + m_nComboHeight, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndPropList.SetWindowPos(nullptr, rectClient.left, rectClient.top + m_nComboHeight + cyTlb, rectClient.Width(), rectClient.Height() -(m_nComboHeight+cyTlb), SWP_NOACTIVATE | SWP_NOZORDER);
}

int CPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Ñîçäàòü ïîëå ñî ñïèñêîì:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndObjectCombo.Create(dwViewStyle, rectDummy, this, 1))
	{
		TRACE0("Íå óäàëîñü ñîçäàòü ïîëå ñî ñïèñêîì \"Ñâîéñòâà\" \n");
		return -1;      // íå óäàëîñü ñîçäàòü
	}

	m_wndObjectCombo.AddString(_T("Ïðèëîæåíèå"));
	m_wndObjectCombo.AddString(_T("Îêíî ñâîéñòâ"));
	m_wndObjectCombo.SetCurSel(0);

	CRect rectCombo;
	m_wndObjectCombo.GetClientRect (&rectCombo);

	m_nComboHeight = rectCombo.Height();

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("Íå óäàëîñü ñîçäàòü ñåòêó ñâîéñòâ\n");
		return -1;      // íå óäàëîñü ñîçäàòü
	}

	InitPropList();

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
	m_wndToolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE /* Çàáëîêèðîâàí */);
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_PROPERTIES_HC : IDR_PROPERTIES, 0, 0, TRUE /* Çàáëîêèðîâàí */);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);

	// Âñå êîìàíäû áóäóò ïåðåíàïðàâëåíû ÷åðåç ýòîò ýëåìåíò óïðàâëåíèÿ, à íå ÷åðåç ðîäèòåëüñêóþ ðàìêó:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	AdjustLayout();
	return 0;
}

void CPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CPropertiesWnd::OnExpandAllProperties()
{
	m_wndPropList.ExpandAll();
}

void CPropertiesWnd::OnUpdateExpandAllProperties(CCmdUI* /* pCmdUI */)
{
}

void CPropertiesWnd::OnSortProperties()
{
	m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnProperties1()
{
	// TODO: äîáàâüòå ñþäà êîä îáðàáîò÷èêà êîìàíä
}

void CPropertiesWnd::OnUpdateProperties1(CCmdUI* /*pCmdUI*/)
{
	// TODO: äîáàâüòå ñþäà êîä îáðàáîò÷èêà êîìàíä îáíîâëåíèÿ èíòåðôåéñà ïîëüçîâàòåëÿ
}

void CPropertiesWnd::OnProperties2()
{
	// TODO: äîáàâüòå ñþäà êîä îáðàáîò÷èêà êîìàíä
}

void CPropertiesWnd::OnUpdateProperties2(CCmdUI* /*pCmdUI*/)
{
	// TODO: äîáàâüòå ñþäà êîä îáðàáîò÷èêà êîìàíä îáíîâëåíèÿ èíòåðôåéñà ïîëüçîâàòåëÿ
}

void CPropertiesWnd::InitPropList()
{
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("Âèä"));

	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("Òðåõìåðíûé âèä"), (_variant_t) false, _T("Â îêíå èñïîëüçóåòñÿ îáû÷íûé (íå æèðíûé) øðèôò, à ó ýëåìåíòîâ óïðàâëåíèÿ åñòü âèçóàëüíî òðåõìåðíàÿ ãðàíèöà.")));

	CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(_T("Ãðàíèöà"), _T("Äèàëîãîâîå îêíî"), _T("Îäíî èç çíà÷åíèé: \"Íåò\", \"Òîíêàÿ\", \"Èçìåíÿåìàÿ\" èëè \"Äèàëîãîâîå îêíî\""));
	pProp->AddOption(_T("Íåò"));
	pProp->AddOption(_T("Òîíêàÿ"));
	pProp->AddOption(_T("Èçìåíÿåìàÿ"));
	pProp->AddOption(_T("Äèàëîãîâîå îêíî"));
	pProp->AllowEdit(FALSE);

	pGroup1->AddSubItem(pProp);
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("Çàãîëîâîê"), (_variant_t) _T("Î ïðîãðàììå"), _T("Óêàçûâàåò òåêñò, êîòîðûé áóäåò îòîáðàæàòüñÿ â ñòðîêå çàãîëîâêà îêíà")));

	m_wndPropList.AddProperty(pGroup1);

	CMFCPropertyGridProperty* pSize = new CMFCPropertyGridProperty(_T("Ðàçìåð îêíà"), 0, TRUE);

	pProp = new CMFCPropertyGridProperty(_T("Âûñîòà"), (_variant_t) 250l, _T("Óêàçûâàåò âûñîòó îêíà"));
	pProp->EnableSpinControl(TRUE, 50, 300);
	pSize->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty( _T("Øèðèíà"), (_variant_t) 150l, _T("Óêàçûâàåò øèðèíó îêíà"));
	pProp->EnableSpinControl(TRUE, 50, 200);
	pSize->AddSubItem(pProp);

	m_wndPropList.AddProperty(pSize);

	CMFCPropertyGridProperty* pGroup2 = new CMFCPropertyGridProperty(_T("Øðèôò"));

	LOGFONT lf;
	CFont* font = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	font->GetLogFont(&lf);

	_tcscpy_s(lf.lfFaceName, _T("Arial"));

	pGroup2->AddSubItem(new CMFCPropertyGridFontProperty(_T("Øðèôò"), lf, CF_EFFECTS | CF_SCREENFONTS, _T("Óêàçûâàåò øðèôò îêíà ïî óìîë÷àíèþ")));
	pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("Èñïîëüçîâàíèå ñèñòåìíîãî øðèôòà"), (_variant_t) true, _T("Óêàçûâàåò, ÷òî â îêíå èñïîëüçóåòñÿ øðèôò MS Shell Dlg")));

	m_wndPropList.AddProperty(pGroup2);

	CMFCPropertyGridProperty* pGroup3 = new CMFCPropertyGridProperty(_T("Ðàçíîå"));
	pProp = new CMFCPropertyGridProperty(_T("(Èìÿ)"), _T("Ïðèëîæåíèå"));
	pProp->Enable(FALSE);
	pGroup3->AddSubItem(pProp);

	CMFCPropertyGridColorProperty* pColorProp = new CMFCPropertyGridColorProperty(_T("Öâåò îêíà"), RGB(210, 192, 254), nullptr, _T("Óêàçûâàåò öâåò îêíà ïî óìîë÷àíèþ"));
	pColorProp->EnableOtherButton(_T("Äðóãîå..."));
	pColorProp->EnableAutomaticButton(_T("Ïî óìîë÷àíèþ"), ::GetSysColor(COLOR_3DFACE));
	pGroup3->AddSubItem(pColorProp);

	static const TCHAR szFilter[] = _T("Ôàéëû çíà÷êîâ(*.ico)|*.ico|Âñå ôàéëû(*.*)|*.*||");
	pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("Çíà÷îê"), TRUE, _T(""), _T("ico"), 0, szFilter, _T("Óêàçûâàåò çíà÷îê îêíà")));

	pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("Ïàïêà"), _T("c:\\")));

	m_wndPropList.AddProperty(pGroup3);

	CMFCPropertyGridProperty* pGroup4 = new CMFCPropertyGridProperty(_T("Èåðàðõèÿ"));

	CMFCPropertyGridProperty* pGroup41 = new CMFCPropertyGridProperty(_T("Ïåðâûé ïîäóðîâåíü"));
	pGroup4->AddSubItem(pGroup41);

	CMFCPropertyGridProperty* pGroup411 = new CMFCPropertyGridProperty(_T("Âòîðîé ïîäóðîâåíü"));
	pGroup41->AddSubItem(pGroup411);

	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("Ýëåìåíò 1"), (_variant_t) _T("Çíà÷åíèå 1"), _T("Ýòî îïèñàíèå")));
	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("Ýëåìåíò 2"), (_variant_t) _T("Çíà÷åíèå 2"), _T("Ýòî îïèñàíèå")));
	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("Ýëåìåíò 3"), (_variant_t) _T("Çíà÷åíèå 3"), _T("Ýòî îïèñàíèå")));

	pGroup4->Expand(FALSE);
	m_wndPropList.AddProperty(pGroup4);
}

void CPropertiesWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus();
}

void CPropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetPropListFont();
}

void CPropertiesWnd::SetPropListFont()
{
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	m_wndPropList.SetFont(&m_fntPropList);
	m_wndObjectCombo.SetFont(&m_fntPropList);
}
