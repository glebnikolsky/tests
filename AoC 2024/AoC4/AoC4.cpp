// AoC4.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

int count_in_line(string& line, const char *pattern)
{
    int n{ 0 };
    int pos = line.find(pattern);
    while (pos != -1) {
        ++n;
        pos = line.find(pattern, pos+4);
    }
    return n;
}
int pivot_count(vector<string >& puzzle, int row, int col, vector<tuple<int, int>> pvt, const char* pattern)
{
    int n{ 1 };
    for (auto& i : pvt) if (puzzle[row + get<0>(i)][col + get<1>(i)] != pattern[n++]) return 0;
    return 1;
}

int solve1(vector<string >& puzzle)
{
    int n{ 0 };
    const char* kXMAS = "XMAS";
    const char* kSAMX = "SAMX";
    vector<tuple<int, int>> dr{ {1,1},{2,2},{3,3} }, dl{ {1,-1},{2,-2},{3,-3} }, dn{ {1,0},{2,0},{3,0} };
    for (int row = 0; row < puzzle.size(); ++row) {
        n += count_in_line(puzzle[row], kXMAS);
        n += count_in_line(puzzle[row], kSAMX);
        for (int col = 0; col < puzzle[row].length(); ++col) {
            if (puzzle[row][col] == *kXMAS && row <= puzzle.size() - 4) {
                n += pivot_count(puzzle, row, col, dn, kXMAS);
                if (col >= 3) n += pivot_count(puzzle, row, col, dl, kXMAS);
                if (col <= puzzle[row].length() - 4) n += pivot_count(puzzle, row, col, dr, kXMAS);
            }
            else if (puzzle[row][col] == *kSAMX && row <= puzzle.size() - 4) {
                n += pivot_count(puzzle, row, col, dn, kSAMX);
                if (col >= 3) n += pivot_count(puzzle, row, col, dl, kSAMX);
                if (col <= puzzle[row].length() - 4) n += pivot_count(puzzle, row, col, dr, kSAMX);
            }
        }
    }
    return n;
}

int check(vector<string >& puzzle, int row, int col, const char *ptrn) {
    static tuple<int, int> pivot[]{ {0,2},{1,1},{2,0},{2,2} };
    int n{ 1 };
    for (auto& i : pivot) 
        if (puzzle[row + get<0>(i)][col + get<1>(i)] != ptrn[n++]) 
            return 0;
    return 1;
}

int solve2(vector<string >& puzzle)
{
    int n{ 0 };
//  M M   M S    S M   S S
//   A     A      A     A
//  S S   M S    S M   M M
//
//    const char* patterns[] = { "MMASS","MSAMS","SMASM","SSAMM" };
    for(int row=0; row<= puzzle.size()-3; ++row)
        for (int col = 0; col <= puzzle[0].length() - 3; ++col) {
            if (puzzle[row][col] == 'M') {
                n += check(puzzle, row, col, "MMASS");
                n += check(puzzle, row, col, "MSAMS");
            }
            if (puzzle[row][col] == 'S') {
                n += check(puzzle, row, col, "SMASM");
                n += check(puzzle, row, col, "SSAMM");
            }
        }


    return n;
}

int main()
{
    string line;
    vector<string > puzzle;
    ifstream input("4.txt");
    while (getline(input, line)) puzzle.push_back(line);
    cout << solve1(puzzle)<<'\n';
    cout << solve2(puzzle);
}

