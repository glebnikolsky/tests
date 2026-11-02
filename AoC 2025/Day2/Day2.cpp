// Day2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

bool parse(const string& line, string& lb, string& ub) {
    static auto b = line.begin();
    static auto e = b;
	if (b == line.end()) return false;
	while (e != line.end() && isdigit(*e)) ++e;
    lb = string(b, e);
    b = ++e;
    while (e != line.end() && isdigit(*e)) ++e;
    ub = string(b,e);
    if (e != line.end()) b = ++e;
    else b = e;
    return true;
}

bool adjust(string& ub, string& lb, long long &b, long long &e) {
        static long long ubound[16]{ 0,0,99,999,9999,99999,999999,9999999,99999999,999999999,9999999999,99999999999,999999999999,9999999999999, 99999999999999,999999999999999 };
        if (ub.length() % 2 && lb.length() % 2) return false;
        if ( !ub.length() % 2)
}
void part1(const char* flnm) {
    ifstream input(flnm);
    long long pds{ 50 };
    string lb, ub;
	string line;
    getline(input, line);
    while (parse(line, lb, ub)) {
        long long b, e;
        if ( !adjust(ub, lb, b, e)) continue;

        
        ++pds;
    }
    std::cout << "Day 1. Part 1 Input:" << flnm << ' ' << pds << endl;
}

void part2(const char* flnm) {
    long long  pds{ 0 };
    std::cout << "Day 1. Part 2 Input:" << flnm << ' ' << pds << endl;
}

int main()
{
    part1("2.1");
    //part1("1.2");
    //part2("1.1");
    //part2("1.2");
}