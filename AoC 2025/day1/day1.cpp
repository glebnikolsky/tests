// day1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

void part1(const char* flnm) {
    ifstream input(flnm);
    int res{ 50 };
    int turns[100]{ 0 };
    string line;
    while (getline(input, line)) {
        if (line[0] == 'L') line[0] = '-';
        else line = line.substr(1);
        res = (res + stoi(line) % 100 + 100) % 100;
        turns[res]++;
    }
    res = 0;
    for (auto& i : turns) if (i > res) res = i;
    std::cout << "Day 1. Part 1 Input:"<<flnm<<' ' << res << endl;
}

void part2(const char* flnm) {
    ifstream input(flnm);
    int res{ 50 };
    int turns{ 0 };
    string line;
    while (getline(input, line)) {
        if (line[0] == 'L') line[0] = '-';
        else line = line.substr(1);
		div_t d = div(stoi(line), 100);
        turns += abs(d.quot);
        int tmp = res + d.rem;
        if (res && (tmp < 0 || tmp > 100)) turns++;
        res = (res + d.rem % 100 + 100) % 100;
        if (!res) turns++;
    }
    std::cout << "Day 1. Part 2 Input:" << flnm << ' ' << turns << endl;
}

int main()
{
    part1("1.1");
    part1("1.2");
    part2("1.1");
    part2("1.2");
}

