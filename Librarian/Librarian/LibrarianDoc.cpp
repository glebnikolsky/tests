
// LibrarianDoc.cpp: ðåàëèçàöèÿ êëàññà CLibrarianDoc 
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS ìîæíî îïðåäåëèòü â îáðàáîò÷èêàõ ôèëüòðîâ ïðîñìîòðà ðåàëèçàöèè ïðîåêòà ATL, ýñêèçîâ
// è ïîèñêà; ïîçâîëÿåò ñîâìåñòíî èñïîëüçîâàòü êîä äîêóìåíòà â äàííûì ïðîåêòå.
#ifndef SHARED_HANDLERS
#include "Librarian.h"
#endif

#include "LibrarianDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CLibrarianDoc

IMPLEMENT_DYNCREATE(CLibrarianDoc, CDocument)

BEGIN_MESSAGE_MAP(CLibrarianDoc, CDocument)
END_MESSAGE_MAP()


// Ñîçäàíèå èëè óíè÷òîæåíèå CLibrarianDoc

CLibrarianDoc::CLibrarianDoc() noexcept
{
	// TODO: äîáàâüòå êîä äëÿ îäíîðàçîâîãî âûçîâà êîíñòðóêòîðà

}

CLibrarianDoc::~CLibrarianDoc()
{
}

BOOL CLibrarianDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: äîáàâüòå êîä ïîâòîðíîé èíèöèàëèçàöèè
	// (Äîêóìåíòû SDI áóäóò ïîâòîðíî èñïîëüçîâàòü ýòîò äîêóìåíò)

	return TRUE;
}




// Ñåðèàëèçàöèÿ CLibrarianDoc

void CLibrarianDoc::Serialize(CArchive& ar)
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
void CLibrarianDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void CLibrarianDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Çàäàíèå ñîäåðæèìîãî ïîèñêà èç äàííûõ äîêóìåíòà.
	// ×àñòè ñîäåðæèìîãî äîëæíû ðàçäåëÿòüñÿ òî÷êîé ñ çàïÿòîé ";"

	// Íàïðèìåð:  strSearchContent = _T("òî÷êà;ïðÿìîóãîëüíèê;êðóã;îáúåêò ole;");
	SetSearchContent(strSearchContent);
}

void CLibrarianDoc::SetSearchContent(const CString& value)
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

// Äèàãíîñòèêà CLibrarianDoc

#ifdef _DEBUG
void CLibrarianDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CLibrarianDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Êîìàíäû CLibrarianDoc
