// LeetCode.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <vector>
#include <iostream>

using namespace std;

int main()
{
	enum class Type{anchor,point,asterix,eol};
	enum class State{skip_one,skip_any,find_anchor};
	string s = "bcdabc";
	string p = "d*";
	auto TypePattern = [&p](auto i) {
		if (i == p.end()) return Type::eol;
		switch (*i) {
		case '.': return Type::point;
		case '*': return Type::asterix;
		default: return Type::anchor;
		}
	};


    return 0;
}

