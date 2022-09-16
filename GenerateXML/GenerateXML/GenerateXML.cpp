// ExecSQL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "GenerateXML.h"
#include "pugixml.hpp"

#include <algorithm>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <sstream>


#include <boost/timer/timer.hpp>
#include <boost/filesystem.hpp>
#include <boost\format.hpp>
#include <boost/regex.hpp>
#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_int_distribution.hpp>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;
using namespace boost::filesystem;

boost::minstd_rand gen( 37 );
const TCHAR *values[] = {"12","345","345","нах","пох","вых","зах","ух","ох","ах","ай","ой","ойойой","божемой","идидомой","вали","отвали","завали",
"навали","1","2"};

void ConvertToFIPI( path fn, int var_unchange = 0 )
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file( fn.c_str() );
	
	if ( doc.child("items").attribute( "name" ).as_int() == var_unchange ) return;

	//Выбираем файл для изменений с вероятностью 1/4
	boost::random::uniform_int_distribution<long> dist( 1, 4 );
	//Выбираем max_mark
	boost::random::uniform_int_distribution<long> maxmarkrkdist( 2, 5 );
	//Выбираем вероятность изменения item
	boost::random::uniform_int_distribution<long> dist_changes( 1, 10 );
	//Выбираем CDATA
	boost::random::uniform_int_distribution<long> valdist( 0, _countof(values)-1 );
	
	
	pugi::xpath_query item_q( "/items/item" );
	pugi::xpath_node_set items = item_q.evaluate_node_set( doc );
	for ( auto &i : items ){
		i.node().append_attribute( "sort" );
		pugi::xml_node answer = i.node().child("answers");
		int max_mark = atol(i.node().child( "max_mark" ).first_child().value());
		int amount = dist( gen );
		i.node().child( "amount" ).first_child().set_value( to_string(amount).c_str() );
		answer = answer.append_child( "answer" );
		pugi::xml_attribute attr =  answer.append_attribute( "index" );
		attr.set_value( 1 );
		answer.append_child( "value" ).append_child(pugi::node_cdata).set_value( values[valdist( gen )] );
		answer.append_child( "mark" ).append_child( pugi::node_pcdata ).set_value( to_string(max_mark >= 2? max_mark-2: 1).c_str() );
		answer.append_child( "amount" ).append_child( pugi::node_pcdata ).set_value( "1" );
		if ( max_mark > 1 ) answer.append_child( "new_mark" ).append_child( pugi::node_pcdata ).set_value( to_string(--max_mark).c_str() );
	}
	ostringstream os;

	doc.save( os );
	string s = os.str();
	int pos = s.find( "?>" );
	s.insert( pos, " encoding=\"windows-1251\"" );
	std::ofstream of;
	of.open( fn.c_str() );
	of << s;

}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;


	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// initialize MFC and print and error on failure
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			_tprintf(_T("Fatal Error: MFC initialization failed\n"));
			nRetCode = 1;
		}
		else
		{
			setlocale(LC_ALL, "ru-RU");
			boost::timer::auto_cpu_timer t;
			setlocale( LC_ALL, "ru-RU" );
			if ( argc < 2 ){
				std::cout << _T( "GenerateXML путь [вариант для файла без изменений] \n\
путь - директория выгрузки\n\
вариант для файла без изменений - останется файл выгрузки без изменений\n\
будут преобразованы файлы выгрузки в файлы для загрузки" );
				return 0;
			}
//			TCHAR xml[] = _T( "\
//<item id = \"495848\" name = \"B01\">\n\
//<answers>\n\
//< / answers>\n\
//<max_mark>6< / max_mark>\n\
//<amount>0< / amount>\n\
//< / item>\n" );
//			pugi::xml_document doc;
//			doc.load_buffer( xml, _countof( xml ) );
//			pugi::xml_node answer = doc.child( "item").child("answers" );
//			answer = answer.append_child( "answer" );
//			doc.save( cout ); cout << '\n';
//			/*pugi::xml_node value = */answer.append_child( "value" ).append_child( pugi::node_cdata ).set_value( "fuck" );
//			doc.save( cout ); cout << '\n';
//			/*value = */answer.append_child( "asdf" ).append_child( pugi::node_pcdata ).set_value( "1" );
//			doc.save( cout ); cout << '\n';







			if ( !exists( path( argv[1] ) ) ){
				cout << "Bad path " << argv[1];
				return 1;
			}
			path p( argv[1] );
			boost::regex e( "(?i)(.*\\\\)(.*)( - КИМ )\\d{6}\\.xml" );
			for ( directory_entry const& x : directory_iterator( p ) ){
				boost::smatch what;
				if ( is_regular_file( x ) && boost::regex_match( x.path().string(), what, e, boost::match_perl ) ){
					ConvertToFIPI( x.path(), argc > 2 ? atol(argv[2]) : 0 );
				}
			}
		}
	}
	else
	{
		_tprintf(_T("Fatal Error: GetModuleHandle failed\n"));
		nRetCode = 1;
	}

	return nRetCode;
}
