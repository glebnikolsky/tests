// CaldrCtrl.cpp : implementation file
//

#include "stdafx.h"
#include <algorithm>
#include "CaldrCtrl.h"

// CaldrCtrl

IMPLEMENT_DYNAMIC(CaldrCtrl, CMonthCalCtrl)

CaldrCtrl::CaldrCtrl()
{

}

CaldrCtrl::~CaldrCtrl()
{
}


BEGIN_MESSAGE_MAP(CaldrCtrl, CMonthCalCtrl)
	ON_NOTIFY_REFLECT( MCN_SELCHANGE, OnSelchange )
END_MESSAGE_MAP()


void CaldrCtrl::OnSelchange( NMHDR *pNMHDR, LRESULT *pResult )
{
	LPNMSELCHANGE pSelChange = reinterpret_cast<LPNMSELCHANGE>(pNMHDR);
	UpdateState();
	*pResult = 0;
}


CaldrCtrl& CaldrCtrl::SetDate( const TCHAR* s )
{
	//yyyy.mm.dd
	std::tuple<short, short, short> day = std::make_tuple( atoi( s ), atoi( s + 5 ), atoi( s + 8 ) );
	if ( std::binary_search( begin( selected_days_ ), end( selected_days_ ), day ) ) return *this;
	
	selected_days_.push_back( day );
	sort( begin( selected_days_ ), end( selected_days_ ) );
	if ( std::get<0>( day ) == cur_time_.wYear && std::get<1>( day ) == cur_time_.wMonth ) UpdateState();
	cur_time_.wDay++;
	SetCurSel( cur_time_ );
	return *this;
}

CaldrCtrl& CaldrCtrl::RemoveDate( const TCHAR* s )
{
	std::tuple<short, short, short> day = std::make_tuple( atoi( s ), atoi( s + 5 ), atoi( s + 8 ) );
	if ( !std::binary_search( begin( selected_days_ ), end( selected_days_ ), day ) ) return *this;
	
	selected_days_.erase( std::remove( begin( selected_days_ ), end( selected_days_ ), day ) );
	if ( std::get<0>( day ) == cur_time_.wYear && std::get<1>( day ) == cur_time_.wMonth ) UpdateState();
	return *this;
}


MONTHDAYSTATE CaldrCtrl::SetMonthSel( short year, short month )
{
	auto i = std::find_if( begin( selected_days_ ), end( selected_days_ ), 
		[&]( const std::tuple<short, short, short> d ){ return std::get<0>( d ) == year && std::get<1>( d ) == month; }
	);
	MONTHDAYSTATE day{0};
	for ( ; i != end( selected_days_ ) && std::get<0>( *i ) == year && std::get<1>( *i ) == month; ++i ) day |= 1 << (std::get<2>( *i ) - 1);
	return day;
}

void CaldrCtrl::UpdateState()
{
	SYSTEMTIME from;
	SYSTEMTIME until;
	int range_size = GetMonthRange( &from, &until, GMR_DAYSTATE );
	if ( cur_time_.wMonth != from.wMonth || cur_time_.wYear != from.wYear ){
		cur_time_ = from;
		std::vector<MONTHDAYSTATE> days( range_size, 0 );
		for ( int i = 0; i < range_size; ++i ){
			days[i] = SetMonthSel( from.wYear, from.wMonth );
			from.wMonth++;
			if ( from.wMonth == 13 ){
				from.wMonth = 1;
				from.wYear++;
			}
		}
		VERIFY( SetDayState( range_size, &days[0] ) );
	}
}