// FindGo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "FindGo.h"
#include <iostream>
#include <string>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/tokenizer.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

enum State{NewLine, Operator, MtlCommentA, MtlCommentZ, EOLComment, StringA, StringZ, Go, End};

State Classify(string::iterator &it, State st)
{

    auto Is_digraph = [&it](TCHAR *digraph){
        return *it == digraph[0] && *(it+1) == digraph[1];
    };
    auto Any_of = [](TCHAR *set, TCHAR ch){
        return !ch || strchr(set, ch) != NULL;
    };
    if ( !*it ) return End;
    if ( st == NewLine || st == Operator ){
        if ( *it == '\'' )  return StringA;
        if ( Is_digraph(_T("--"))) return EOLComment;
        if ( Is_digraph(_T("/*"))) return MtlCommentA;
        if ( Is_digraph(_T("go")) && Any_of(_T(" \t\r\n"), *(it+2)) ) return Go;
        if ( Any_of(_T("\r\n"), *it)) return NewLine;
        return Operator;
    }
    if ( st == EOLComment ){
        if ( Any_of(_T("\r\n"), *it)) return NewLine;
        return EOLComment;
    }
    if ( st == MtlCommentA ){
        if ( Is_digraph(_T("*/"))) return MtlCommentZ;
        return MtlCommentA;
    }
    if ( st == StringA ){
        if ( *it == '\'' )  return StringZ;
        return StringA;
    }
    return Operator;
}

void FindGo(string &sql, string &currentSQL)
{

    boost::trim(sql);
    string buf = sql;
    boost::to_lower(buf);
    auto Any_of = [](TCHAR *set, TCHAR ch){
        return !ch || strchr(set, ch) != NULL;
    };
    auto cpos = buf.begin();
    currentSQL = "";
    bool searching = true;
    while(searching ){
        State st =  Classify(cpos, NewLine);
        switch (st)
        {
        case Go:
            sql = sql.substr(cpos - buf.begin() + 2);
            currentSQL = string(buf.begin(), cpos);
            searching = false;
            break;
        case End:
            currentSQL = string(buf.begin(), cpos);
            searching = false;
            sql = "";
            break;
        case StringA:
            ++cpos;
            while( (st = Classify(cpos, StringA)) != StringZ) cpos++;
            cpos++;
            break;
        case Operator:
            ++cpos;
            while( (st = Classify(cpos, Operator)) == Operator) cpos++;
            break;
        case EOLComment:
            ++cpos;
            while( (st = Classify(cpos, EOLComment)) != NewLine) cpos++;
            break;
        case MtlCommentA:
            ++cpos;
            while( (st = Classify(cpos, MtlCommentA)) != MtlCommentZ) cpos++;
            cpos+=2;
            break;
        case NewLine:
            ++cpos;
            while( Any_of(_T(" \t\r\n"), *cpos)) cpos++;
        default:
            break;
        }
    }
    return;
}



bool DoLongScriptByParts(/*CDatabaseEx &db, */string &sql, string &currentSQL, string &err)
{
    if ( !sql.empty() && sql[0] )
        do{
            FindGo(sql, currentSQL);
            if ( !currentSQL.empty() )cout<<"Exec |"<<currentSQL<<"|"<<endl;
        }while(!sql.empty() && sql[0] && currentSQL.empty());
    return true;
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
			// TODO: change error code to suit your needs
			_tprintf(_T("Fatal Error: MFC initialization failed\n"));
			nRetCode = 1;
		}
		else
		{
			string sql = "go\n\r go\n\r\tselect * '111''2222'\n\rset a='qwe\n\r\
asdf' sadfasf asdfasdfas dfasdf\n\r\
/*fasdfasdf asf*/asdasdf asfasdfasdfasdfas\n\r\
'--dasf dasfasef asdf asdf asdfasdfasdfasdfasf\n\r\
go\r\n\
sadad asdASd'/*\n\r\
das asfasdf\n\r\
go\r\n\
*/\n\r\
gone\r\n\
go\r\n\
go\r\n\
select \r\n\
select \r\n\
go\r\n\
go\n\rselect";
            string sql1 = "go\n\rselect";
            sql += '\0';
            sql1 += '\0';
            string currentSQL, err;
            while( !sql.empty() && DoLongScriptByParts(sql, currentSQL, err)) cout<<"Exec |"<<currentSQL<<"|"<<endl;
            while( !sql1.empty() && DoLongScriptByParts(sql1, currentSQL, err)) cout<<"Exec |"<<currentSQL<<"|"<<endl;
		}
	}
	else
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: GetModuleHandle failed\n"));
		nRetCode = 1;
	}

	return nRetCode;
}
