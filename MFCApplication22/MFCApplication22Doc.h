
// MFCApplication22Doc.h: èíòåðôåéñ êëàññà CMFCApplication22Doc 
//


#pragma once


class CMFCApplication22Doc : public CDocument
{
protected: // ñîçäàòü òîëüêî èç ñåðèàëèçàöèè
	CMFCApplication22Doc() noexcept;
	DECLARE_DYNCREATE(CMFCApplication22Doc)

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
	virtual ~CMFCApplication22Doc();
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
