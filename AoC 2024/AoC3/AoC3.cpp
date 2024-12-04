// AoC3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <regex>

using namespace std;


long long proc_line(string &s)
{
    int pos = s.find("mul(");
    long long res{ 0 };
    while (pos != -1) {
        int pop = pos + 4;
        pos += 4;
        while (isdigit(s[pos])) ++pos;
        int op1{ 0 };
        if (s[pos] == ',') op1 = stoi(s.substr(pop));
        else {
            pos = s.find("mul(", pos);
            continue;
        }
        pop = ++pos;
        while (isdigit(s[pos])) ++pos;
        if (s[pos] == ')') res += op1 * stoi(s.substr(pop));
        pos = s.find("mul(", pos);
    }
    return res;
}

void prepare(string& s)
{
    //int dont_pos = s.rfind("don't()");
    //if( dont_pos != -1) {
    //    int do_pos = s.find("do()", dont_pos);
    //    if (do_pos != -1) s = s.substr(0, dont_pos + 1) + s.substr(do_pos + 4);
    //    else s = s.substr(0, dont_pos + 1);
    //    dont_pos = s.rfind("don't()", dont_pos);
    //}
    vector<int> dont;
    vector<int> dos;
    int pos = s.find("do()");
    while (pos != -1) {
        dos.push_back(pos);
        pos = s.find("do()", pos+1);
    }
    pos = s.find("don't()");
    while (pos != -1) {
        dont.push_back(pos);
        pos = s.find("don't()", pos + 1);
    }
    copy(dos.begin(), dos.end(), ostream_iterator<int>(cout, "\t"));
    cout << '\n';
    copy(dont.begin(), dont.end(), ostream_iterator<int>(cout, "\t"));
    cout << '\n';

}



int main()
{ 
    ifstream input("3.txt");
    string s;
    long long res{ 0 };
    while (getline(input, s)) {
        prepare(s);
        res += proc_line(s);
     }
    cout << res << '\n';//88125394
    return 0;
}

