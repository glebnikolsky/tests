// Combination.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>
#include <cstdlib>

using namespace std;

struct combinations
{
    typedef vector<int> combination_t;

    // initialize status
    combinations(int n, int m) :n_(n), m_(m),
        completed(false),
        generated(0)
    {
        for (int c = 1; c <= m_; ++c)
            curr.push_back(c);
    }

    // true while there are more solutions
    bool completed;

    // count how many generated
    int generated;

    // get current and compute next combination
    combination_t next()
    {
        combination_t ret = curr;

        // find what to increment
        completed = true;
        for (int i = m_ - 1; i >= 0; --i)
            if (curr[i] < n_ - m_ + i + 1)
            {
                int j = curr[i] + 1;
                while (i <= m_ - 1)
                    curr[i++] = j++;
                completed = false;
                ++generated;
                break;
            }

        return ret;
    }

private:
    int n_;
    int m_;
    combination_t curr;
};

void Calc(int n, int m)
{
    vector<vector<char>> combs;
    combinations cs(n, m);
    while (!cs.completed)
    {
        combinations::combination_t c = cs.next();
        vector<char> tmp;
        for (auto& i : c) tmp.push_back(static_cast<char>(i));
        combs.push_back(tmp);
    }
    cout << "C(" << n << "," << m << ")=" << combs.size() << endl;
    //for (auto& i : combs) {
    //    copy(i.begin(), i.end(), ostream_iterator<int>(cout, ","));
    //    cout << "\b\n";
    //}
    vector<long> res(m + 1, 0);
    for (int i = 0; i < combs.size(); ++i) {
        for (int j = i + 1; j < combs.size(); ++j) {
            vector<char> tmp;
            set_difference(combs[i].begin(), combs[i].end(), combs[j].begin(), combs[j].end(), inserter(tmp, tmp.begin()));
            ++res[tmp.size()];
        }
    }
    copy(res.begin(), res.end(), ostream_iterator<int>(cout, ", "));
    cout << "\b\b\b\n\n";
}

int main(int argc, char** argv)
{
    for (int n = 4; n < 21; ++n)
        for (int m = 2; m < n; ++m) Calc(n, m);
    return 0;
}