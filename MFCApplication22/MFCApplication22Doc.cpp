
// MFCApplication22Doc.cpp: ðåàëèçàöèÿ êëàññà CMFCApplication22Doc 
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS ìîæíî îïðåäåëèòü â îáðàáîò÷èêàõ ôèëüòðîâ ïðîñìîòðà ðåàëèçàöèè ïðîåêòà ATL, ýñêèçîâ
// è ïîèñêà; ïîçâîëÿåò ñîâìåñòíî èñïîëüçîâàòü êîä äîêóìåíòà â äàííûì ïðîåêòå.
#ifndef SHARED_HANDLERS
#include "MFCApplication22.h"
#endif

#include "MFCApplication22Doc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMFCApplication22Doc

IMPLEMENT_DYNCREATE(CMFCApplication22Doc, CDocument)

BEGIN_MESSAGE_MAP(CMFCApplication22Doc, CDocument)
END_MESSAGE_MAP()


// Ñîçäàíèå èëè óíè÷òîæåíèå CMFCApplication22Doc

CMFCApplication22Doc::CMFCApplication22Doc() noexcept
{
	// TODO: äîáàâüòå êîä äëÿ îäíîðàçîâîãî âûçîâà êîíñòðóêòîðà

}

CMFCApplication22Doc::~CMFCApplication22Doc()
{
}

BOOL CMFCApplication22Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: äîáàâüòå êîä ïîâòîðíîé èíèöèàëèçàöèè
	// (Äîêóìåíòû SDI áóäóò ïîâòîðíî èñïîëüçîâàòü ýòîò äîêóìåíò)

	return TRUE;
}




// Ñåðèàëèçàöèÿ CMFCApplication22Doc

void CMFCApplication22Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: äîáàâüòå êîä ñîõðàíåíèÿ
	}
	else
	{
		// TODO: äîáàâüòå êîä çàãðóçêè
	}
}

#ifdef SHARED_HANDLERS

// Ïîääåðæêà äëÿ ýñêèçîâ
void CMFCApplication22Doc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Èçìåíèòå ýòîò êîä äëÿ îòîáðàæåíèÿ äàííûõ äîêóìåíòà
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Ïîääåðæêà îáðàáîò÷èêîâ ïîèñêà
void CMFCApplication22Doc::InitializeSearchContent()
{
	CString strSearchContent;
	// Çàäàíèå ñîäåðæèìîãî ïîèñêà èç äàííûõ äîêóìåíòà.
	// ×àñòè ñîäåðæèìîãî äîëæíû ðàçäåëÿòüñÿ òî÷êîé ñ çàïÿòîé ";"

	// Íàïðèìåð:  strSearchContent = _T("òî÷êà;ïðÿìîóãîëüíèê;êðóã;îáúåêò ole;");
	SetSearchContent(strSearchContent);
}

void CMFCApplication22Doc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// Äèàãíîñòèêà CMFCApplication22Doc

#ifdef _DEBUG
void CMFCApplication22Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMFCApplication22Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Êîìàíäû CMFCApplication22Doc
