// AoC4.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

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
        pos = line.find(pattern);
    }
    return n;
}
int pivot_count(vector<string >& puzzle, int row, int col, vector<tuple<int, int>> pvt, const char* pattern)
{
    int n{ 1 };
    for (auto &i : pvt) if (puzzle[row + get<0>(i)][col + get<1>(i)] != pattern[n++]) return 0;
    return 1;
}

int solve(vector<string >& puzzle)
{
    int n{ 0 };
    const char* kXMAS = "XMAS";
    const char* kSAMX = "SAMX";
    vector<tuple<int, int>> dl{ {1,1},{2,2},{3,3} }, dr{ {1,-1},{2,-2},{3,-3} }, dn{ {0,-1},{0,-2},{0,-3} };
    for(int row=0; row<puzzle.size(); ++row)
        for (int col = 0; col < puzzle[row].length(); ++col) {
            n += count_in_line(puzzle[row], kXMAS);
            n += count_in_line(puzzle[row], kSAMX);
            if (row <= puzzle.size() - 4 ) {
                if (puzzle[row][col] == *kXMAS) n += pivot_count(puzzle, row, col, dn, kXMAS);
                if (puzzle[row][col] == *kSAMX) n += pivot_count(puzzle, row, col, dn, kSAMX);
                if (puzzle[row][col] == *kXMAS && col>=3) n += pivot_count(puzzle, row, col, dr, kXMAS);
                if (puzzle[row][col] == *kSAMX && col >= 3) n += pivot_count(puzzle, row, col, dr, kSAMX);
                if (puzzle[row][col] == *kXMAS && col <= puzzle[row].length() -4) n += pivot_count(puzzle, row, col, dl, kXMAS);
                if (puzzle[row][col] == *kSAMX && col <= puzzle[row].length() - 4) n += pivot_count(puzzle, row, col, dl, kSAMX);
            }
        }
    return n;
}

int main()
{
    string line;
    vector<string > puzzle;
    ifstream input("4_1.txt");
    while (getline(input, line)) puzzle.push_back(line);
    cout << solve(puzzle);
}

