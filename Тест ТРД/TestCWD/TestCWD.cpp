// TestCWD.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <Windows.h>
#include <boost\filesystem.hpp>

std::string GetCWD( TCHAR *module_name ){

	TCHAR lpModulePath[1024];
	::memset( lpModulePath, 0, 1024 );
	HMODULE h = ::GetModuleHandle( module_name );
	::GetModuleFileName( h, lpModulePath, 1024 );
	boost::filesystem::path p( lpModulePath );
	return p.parent_path().string();
}

std::string GetDB( const std::string& dir )
{
	for ( auto &fl : boost::filesystem::directory_iterator( dir ) ){
		if ( /*boost::filesystem::*/is_regular_file( fl ) && (fl.path().extension() == _T( ".pdb" ) || fl.path().extension() == _T( ".DB" )) )  return fl.path().string();
	}
	return std::string();
}

int main()
{
	std::string s = GetCWD( _T( "TestCWD"));
	std::string d = GetDB( s );
	DWORD blen = ::GetCurrentDirectory( 0, NULL );
	TCHAR *buf = new TCHAR[blen];
	DWORD res = ::GetCurrentDirectory( blen, buf );
	std::cout << res << '\t' << buf << '\n';
	res = ::SetCurrentDirectory( _T( "C:\\APPEAL" ) );
	res = ::GetCurrentDirectory( blen, buf );

	std::cout << res << '\t' << buf << '\n';

	std::cout << s << '\t' << d << '\t' << GetDB( GetCWD( _T( "TestCWD" ) ) ) << '\n';
	boost::filesystem::path p( "D:\\tmp\\Search KIM\\ERBD_EGE_MAIN_24.db" );
	std::cout << p.parent_path() << '\n';
	for ( auto &fl : boost::filesystem::directory_iterator( p.parent_path() ) ) std::cout << fl << '\t' << fl.path().filename()<<'\n';
	return 0;
}



