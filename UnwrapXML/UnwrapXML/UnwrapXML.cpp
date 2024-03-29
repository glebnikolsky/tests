// ExecSQL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "UnwrapXML.h"
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

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;


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
			pugi::xml_document doc;
			pugi::xml_parse_result result = doc.load_file( "D:\\tmp\\1\\��������� ���� - ��� 000102.xml" );
			if ( !result ) return 1;
			pugi::xpath_query itemq( "/items/item" );
			pugi::xpath_node_set itemres = itemq.evaluate_node_set( doc );
			for ( auto &i : itemres ){
				std::cout << i.node().attribute( "name" ).as_string() << ' ' << i.node().attribute( "id" ).as_int() << ' ';
				cout << i.node().child_value( "max_mark" );
				pugi::xpath_query answerq( "answers/answer" );
				pugi::xpath_node_set answerres = answerq.evaluate_node_set( i.node());
				for ( auto j : answerres ){
					pugi::xml_node tmp = j.node().child( "mark" );
					if ( tmp ) std::cout << tmp.name()<< ' '<< tmp.first_child().value() << ' ';
					tmp = j.node().child( "new_mark" );
					if ( tmp ) std::cout << tmp.name() << ' ' << tmp.first_child().value() << ' ';
					tmp = j.node().child( "amount" );
					if ( tmp ) std::cout << tmp.name() << ' ' << tmp.first_child().value() << ' ';
					tmp = j.node().child( "value" );
					if ( tmp ) std::cout << "value = '" << tmp.first_child().value() << "'\n";
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
