// TreeContest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;

struct Task{
    Task(int t, int i):task_(t), id_theme_(i){}
    int task_;
    int id_theme_;
};

struct Theme{
    Theme(std::string p, std::string n):pfx_(p),full_name_(n){}
    string pfx_;
    string full_name_;
};

int _tmain(int argc, _TCHAR* argv[])
{
    int id_theme = -1;
    vector<Theme> themes;
    vector<Task> tasks;

    std::string input_line;
    while( getline(cin, input_line)){
        int pos;
        if ( (pos = input_line.rfind('.')) != -1 ){
            ++id_theme;
            std::string pfx = input_line.substr(0,pos+1);
            themes.push_back(Theme(pfx, input_line.substr(pos+2)));
            pfx = pfx.substr(0,pfx.length()-1);
            if ( (pos = pfx.rfind('.')) != -1){
                pfx = pfx.substr(0,pos+1);
                for( auto i = themes.begin(); i != themes.end(); ++i )
                    if ( i->pfx_ == pfx ){
                        themes[id_theme].full_name_ = i->full_name_ + " " + themes[id_theme].full_name_;
                        break;
                    }
            }
        }
        else tasks.push_back(Task(atoi(input_line.c_str()), id_theme));
    }
	return 0;
}

