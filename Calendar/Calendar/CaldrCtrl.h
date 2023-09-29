#pragma once
#include <tuple>
#include <vector>

// CaldrCtrl

class CaldrCtrl : public CMonthCalCtrl
{
	DECLARE_DYNAMIC(CaldrCtrl)
	std::vector<std::tuple<short,short,short>> selected_days_;
	SYSTEMTIME cur_time_;

public:
	CaldrCtrl();
	virtual ~CaldrCtrl();
	CaldrCtrl& SetDate( const TCHAR* s );						//yyyy.mm.dd
	CaldrCtrl& RemoveDate( const TCHAR* s );						//yyyy.mm.dd
	void UpdateState();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnSelchange( NMHDR *pNMHDR, LRESULT *pResult );
	MONTHDAYSTATE SetMonthSel( short year, short month );

};


