// AoC6.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;


enum Dir { dir_up, dir_right, dir_down, dir_left };
char trace[4]{ 'u','r','d','l' };

struct Pos {
    Pos(int r, int c, int mr=0, int mc=0) : r_(r), c_(c), max_r_(mr), max_c_(mc) {}
    int r_;
    int c_;
    int max_r_;
    int max_c_;
    
    void SetMax(int mr, int mc) {
        max_r_ = mr;
        max_c_ = mc;
    }

    bool Move(Dir &d, vector<string>& v) {
        if (v[r_][c_] != '#') {
            v[r_][c_] = 'X';
            switch (d) {
            case dir_up: --r_; break;
            case dir_right: ++c_; break;
            case dir_down: ++r_; break;
            case dir_left: --c_; break;
            }
            if (r_ < 0 || r_ == max_r_) return false;
            if (c_ < 0 || c_ == max_c_) return false;
        }
        else {
            switch (d) {
                case dir_up: ++r_; break;
                case dir_right: --c_; break;
                case dir_down: --r_; break;
                case dir_left: ++c_; break;
            }
            d = (Dir)((d + 1) % 4);
        }
        return true;
    }

    int Move2(Dir& d, vector<string>& v) {
        if (v[r_][c_] != '#' && v[r_][c_] != 'O') {
            if ( v[r_][c_] == trace[(int)d]) 
                return 0;
            v[r_][c_] = trace[(int)d];
            switch (d) {
            case dir_up: --r_; break;
            case dir_right: ++c_; break;
            case dir_down: ++r_; break;
            case dir_left: --c_; break;
            }
        }
        else {
            switch (d) {
            case dir_up: ++r_; ++c_; d = dir_right; break;
            case dir_right: --c_; ++r_;  d = dir_down;  break;
            case dir_down: --r_; --c_;  d = dir_left;  break;
            case dir_left: ++c_; --r_;  d = dir_up;  break;
            }
        }
        if (r_ < 0 || r_ == max_r_ || c_ < 0 || c_ == max_c_) return 1;
        return 2;
    }

};

Pos GetInitPoint(vector<string>& v)
{
    for (int r = 0; r < v.size(); ++r) {
        int c = v[r].find('^');
        if( c >0) return Pos(r, c);
    }
    return Pos(0, 0);
}

void Walk(vector<string>& v)
{
    Pos p = GetInitPoint(v);
    p.SetMax(v.size(), v[0].size());
    Dir d = dir_up;
    while (p.Move(d, v)) {
    }
}

int Walk2(vector<string>& v)
{
    Pos p = GetInitPoint(v);
    p.SetMax(v.size(), v[0].size());
    Dir d = dir_up;
    int ret;
    while ((ret = p.Move2(d, v)) == 2) {
 /*       for (auto& i : v) cout << i << endl;
        cout << endl;*/
    }
    return !ret? 1:0;
}

int proc_1(vector<string>& v) 
{
    vector<string>& copy_v(v);
    Walk(v);
    int res{ 0 };
    for (auto s : v)
        for (auto& ch : s)
            if (ch == 'X')++res;
    v.swap(copy_v);
    return res;
}


int proc_2(vector<string>& v)
{
    vector<string> copy_v(v);
    int res{ 0 };
    for(int r=0; r < v.size(); ++r)
        for (int c = 0; c < v[r].length(); ++c) {
            if (v[r][c] == '.') {
                v[r][c] = 'O';
                cout << r << ',' << c << endl;
                int r = Walk2(v);
                res += r;
                /*if (r) {
                    for (auto& i : v) {
                        cout << i << endl;
                    }
                    cout << endl;
                }*/
                v= copy_v;
            }
        }

    return res;

}

int main()
{
    ifstream input("6.txt");
    string line;
    vector<string> v;
    while( getline(input,line)) v.push_back(line);
    cout << proc_1(v)<<'\t'<<proc_2(v);
}

