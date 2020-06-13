// CSV.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <string>
#include <locale.h>
#include <vector>
#include <algorithm>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;
typedef boost::tokenizer<boost::char_separator<char> > tokenizer;

int _tmain(int argc, _TCHAR* argv[])
{
    ifstream hfile(_T("E:\\samsung\\OutlookContacts.csv"));
    ifstream infile(_T("E:\\samsung\\ онтакты002.csv"));
    setlocale(LC_ALL, "ru-RU");

    string str;
    getline(hfile,str);
    vector<string> hname; //ѕравильный заголовок
    {
        boost::char_separator<char> field_sep(_T(";"));
        tokenizer field_tokens(str, field_sep);
        for (tokenizer::iterator tok_iter = field_tokens.begin(); tok_iter != field_tokens.end(); ++tok_iter){
            hname.push_back(*tok_iter);
        }
    }

    string template_str(_T(","));
    boost::replace_all(str, _T(";"), _T(","));
    string t(str);
    boost::replace_all(t, _T(","), _T(";"));
    cout<<t<<endl;

    vector<string> spos;
    getline(infile,str);
    {
        vector<int> vpos;
        boost::char_separator<char> field_sep(_T(","));
        tokenizer field_tokens(str, field_sep);
        for (tokenizer::iterator tok_iter = field_tokens.begin(); tok_iter != field_tokens.end(); ++tok_iter){
            string tmp = *tok_iter;
            tmp = _T("\"") + tmp + _T("\"");
            int pos(0);
            for(auto i =hname.begin(); i != hname.end() && *i != tmp; ++i, ++pos);
            vpos.push_back(pos);
        }
        vector<int> ord(vpos);
        sort(ord.begin(), ord.end());
        auto j = ord.begin();
        for( size_t i = 0; i < hname.size(); ++i ){
            if ( j != ord.end() && i == *j ){
                //char temp[10];
                //_itoa_s(i, temp, 10, 10);
                template_str += boost::lexical_cast<string>(*j);
                if ( j != ord.end() ) ++j;
            }
            template_str += _T(",");
        }// Ћишн€€ зап€та€ в начале и в конце нужны дл€ того, чтобы делать replace вида ",11," после обработки надо их отрезать
        for( auto i:vpos){
            spos.push_back(_T(",")+boost::lexical_cast<string>(i)+_T(","));
        }
    }


    while( getline(infile,str))
    {
        string res(template_str);
        boost::char_separator<char> field_sep(_T(","));
        tokenizer field_tokens(str, field_sep);
        int i(0);
        for (tokenizer::iterator tok_iter = field_tokens.begin(); tok_iter != field_tokens.end(); ++tok_iter, ++i){
            string tmp = *tok_iter;
            if ( tmp[0] == '+')
                boost::replace_all(tmp, " ", "");
            tmp = ",\"" + tmp + "\",";
            boost::replace_first(res,spos[i], tmp);
        }
        res = res.substr(1, res.length()-2);
        boost::replace_all(res, _T(","), _T(";"));
        cout<<res<<endl;
    }

	return 0;
}

