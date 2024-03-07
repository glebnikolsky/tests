
// LibrarianDoc.h: èíòåðôåéñ êëàññà CLibrarianDoc 
//


#pragma once


class CLibrarianDoc : public CDocument
{
protected: // ñîçäàòü òîëüêî èç ñåðèàëèçàöèè
	CLibrarianDoc() noexcept;
	DECLARE_DYNCREATE(CLibrarianDoc)

// Àòðèáóòû
public:

// Îïåðàöèè
public:

// Ïåðåîïðåäåëåíèå
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Ðåàëèçàöèÿ
public:
	virtual ~CLibrarianDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Ñîçäàííûå ôóíêöèè ñõåìû ñîîáùåíèé
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Âñïîìîãàòåëüíàÿ ôóíêöèÿ, çàäàþùàÿ ñîäåðæèìîå ïîèñêà äëÿ îáðàáîò÷èêà ïîèñêà
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
