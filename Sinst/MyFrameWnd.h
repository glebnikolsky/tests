#pragma once

class CMyFrameWnd : public CFrameWnd
{
public:
	CMyFrameWnd();

protected:
	DECLARE_DYNCREATE(CMyFrameWnd)

	virtual ~CMyFrameWnd();

  afx_msg LRESULT OnCopyData(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};
