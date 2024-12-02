// AoC2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

int signum(int d)
{
    if (d < 0) return -1;
    else if (d > 0) return 1;
    else return 0;
}

bool test_rep(string& s)
{
    istringstream in(s);
    int prev;
    int next;
    in >> prev >> next;
    int dir{ next-prev };
    prev = next;
    while (in >> next) {
        int newdir = next - prev;
        if (!newdir || signum(dir) != signum(newdir) || abs(dir) > 3 || abs(newdir) > 3) return false;
        dir = newdir;
        prev = next;
        if (in.eof()) break;
    }
    return true;
}

int main()
{
    ifstream input("2.txt");
    int res{ 0 };
    string rep;;
    while (getline(input, rep))
        if (test_rep(rep)) ++res;
    cout << res<<'\n';
}

