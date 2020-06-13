// Ranges.cpp : Defines the entry point for the console application.
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

struct Interval{
    Interval()
        :a_(-1)
        ,z_(-1)
    {}
    
    bool add(int no)
    {
        if ( a_ == -1 ){
            a_ = no;
            return true;
        }
        if ( z_ == -1 ){
            z_ = no;
            return true;
        }
        if ( (z_+1) == no ){
            z_ = no;
            return true;
        }
        return false;
    }
    string format()
    {
        string s;
        s += _T("[");
        s += boost::lexical_cast<string>(a_);
        if ( z_ != -1 ){
            s += _T(" , ");
            s += boost::lexical_cast<string>(z_);
            s += _T("] ");
        }else s += _T("[ ");
        return s;
    }

    long a_, z_;
};


void parse(string const &line, string &pref, long &no)
{
    boost::char_separator<char> field_sep(_T(";"));
    tokenizer field_tokens(line, field_sep);
    tokenizer::iterator tok_iter = field_tokens.begin();
    pref = *tok_iter++;
    pref += *tok_iter++;
    no = boost::lexical_cast<long>( tok_iter->substr(3,9));
}

void proc_file(istream &is, ostream &os)
{
    string line, oldpref;
    long no;
    getline( is, line);
    getline( is, line);
    parse(line, oldpref, no);
    Interval ii;
    ii.add(no);
    os<<oldpref<<' ';
    while( getline( is, line)){
        string pref;
        parse(line, pref, no);
        if ( pref != oldpref){
            oldpref = pref;
            if ( ii.a_ != -1)
                os<<ii.format();
            ii = Interval();
            os<<endl<<pref<<' ';
        }
        if ( !ii.add(no)){
            os<<ii.format();
            ii = Interval();
        }
    }
}

int _tmain(int argc, _TCHAR* argv[])
{
    ifstream ifl;
    ofstream ofl;
    if ( argc > 1)
        ifl.open(argv[1], ios::in );
    if ( argc > 2)
        ofl.open(argv[2], ios::out );
    proc_file( argc > 1? ifl : cin,  argc > 2 ? ofl : cout);

	return 0;
}

