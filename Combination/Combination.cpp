// Combination.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <algorithm>
#include <bitset>
#include <iostream>
#include <iterator>
#include <vector>
#include <boost/multiprecision/cpp_int.hpp>

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

using Bits = bitset < 32 >;
//using BigInt = boost::multiprecision::int128_t;
using BigInt = int64_t;
vector<Bits> bits;

void InitBits()
{
    unsigned long ul{ 1 };
    for (int i = 1; i <= 32; ++i, ul <<= 1) bits.emplace_back(ul);
}

void Calcb(const int n, int m)
{
    vector<Bits> combs;
    combinations cs(n, m);
    while (!cs.completed)
    {
        combinations::combination_t c = cs.next();
        Bits tmp;
        for (auto& i : c) tmp |= bits[i-1];
        combs.push_back(tmp);
    }
    cout << "C(" << n << "," << m << ")=" << combs.size() << endl;
    vector<BigInt> res(m + 1, 0);
    for (int i = 0; i < combs.size() - 1; ++i) {
        for (int j = i + 1; j < combs.size(); ++j) {
            ++res[m - (combs[i] & combs[j]).count()];
        }
    }
    copy(res.begin(), res.end(), ostream_iterator<BigInt>(cout, ", "));
    cout << "\n\n";
}


int main(int argc, char** argv)
{
    InitBits();
    for (int n = 4; n < 21; ++n)
        for (int m = 2; m < n; ++m) Calcb(n, m);
    return 0;
}