// AoC.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <algorithm>
#include <array>
#include <cstdint>
#include <deque>
#include <iostream>
#include <iterator>
#include <fstream>
#include <functional>
#include <map>
#include <numeric>
#include <set>
#include <string>
#include <sstream>
#include <tuple>
#include <vector>

#include <boost/integer/common_factor.hpp>

using namespace std;

//void Day1_1()
//{
//    int res{ 0 };
//    ifstream ifs("day1_1.txt");
//    string line;
//    while (getline(ifs,line)) {
//        char first{ '\0' }, last{'0'};
//        for (const auto &i : line) {
//            if (!isdigit(i)) continue;
//            if (!first) first = i;
//            last = i;
//        }
//        res += (first - '0') * 10 + last - '0';
//    }
//    cout << line<<' '<<res<<'\n';
//}
//
//void FindAndReplaceDig(string& line)
//{
//    string digits[]{ "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
//    int pos{ -1 };
//    for (int l{ 0 }; l < line.length(); ++l) {
//        int n{ -1 };
//        for (int i{ 0 }; i < 9; ++i) {
//            if (line[l] != digits[i][0]) continue;
//            int d{ 1 };
//            for (int ll{l + 1}; d < digits[i].size() && ll < line.size() && digits[i][d] == line[ll]; ++d,++ll);
//            if (d != digits[i].size()) continue;
//            line[l] = i + 1+'0';
//            break;
//        }
//    }
//}
//
//
//void Day1_2()
//{
//    int res{ 0 };
//    ifstream ifs("day1_2.txt");
//    string line;
//    while (getline(ifs, line)) {
//        FindAndReplaceDig(line);
//        char first{ '\0' }, last{ '0' };
//        for (const auto& i : line) {
//            if (!isdigit(i)) continue;
//            if (!first) first = i;
//            last = i;
//        }
//        res += (first - '0') * 10 + last - '0';
//    }
//    cout << line << ' ' << res << '\n';
//}
//
//bool CheckSet(string::iterator b, const string::iterator e)
//{
//    //12 red cubes, 13 green cubes, and 14 blue cubes
//    static map<char, int> len{ {'r',3},{'g',5},{'b',4} },
//        limit{ {'r',12},{'g',13},{'b',14} };
//    map<char, int> res{ {'r',0},{'g',0},{'b',0} };
//    while (b != e) {
//        b = find_if(b, e, [](char c) {return isdigit(c); });
//        int val = atol(string(b, e).c_str());
//        b = find_if(b, e, [](char c) { return isalpha(c); });
//        res[*b] += val;
//        if (res[*b] > limit[*b]) return false;
//        b += len[*b];
//    }
//    return true;
//}
//
//void Day2_1()
//{
//    int result{ 0 };
//    ifstream ifs("day2_1.txt");
//    string line;
//
//    while (getline(ifs, line)) {
//        int game = atol(line.c_str() + 5);
//        cout << line << '\n';
//        auto Is_space = [&line](char p) { return p == ' '; };
//        auto Is_eog = [&line](char p) {    return p == ';'; };
//        auto pos = line.begin() + 6;
//        bool cont{ true };
//        pos = find_if(pos, line.end(), Is_space);
//        while (cont && pos != line.end()) {
//            //Game 1: 4 blue, 7 red, 5 green; 3 blue, 4 red, 16 green; 3 red, 11 green
//            auto end = find_if(pos, line.end(), Is_eog);
//            if (!(cont = CheckSet(pos, end))) break;
//            if (end != line.end())
//                pos = end + 1;
//            else
//                pos = end;
//        }
//        if (cont) result += game;
//    }
//    cout << result;
//}
//
//int GameProd(string::iterator b, const string::iterator e)
//{
//    static map<char, int> len{ {'r',3},{'g',5},{'b',4} };
//    map<char, int> res{ {'r',0},{'g',0},{'b',0} };
//    while (b != e) {
//        b = find_if(b, e, [](char c) {return isdigit(c); });
//        int val = atol(string(b, e).c_str());
//        b = find_if(b, e, [](char c) { return isalpha(c); });
//        res[*b] = max(res[*b], val);
//        b += len[*b];
//    }
//    return res['r'] * res['g'] * res['b'];
//}
//
//
//void Day2_2()
//{
//    int result{ 0 };
//    ifstream ifs("day2_2.txt");
//    string line;
//    auto Is_bog = [](char p) {    return p == ':'; };
//    while (getline(ifs, line)) {
//        auto b = find_if(line.begin(), line.end(), Is_bog);
//        result += GameProd(b, line.end());
//    }
//    cout << result;
//}
//
//int ProcLine(string& f, string& s)
//{
//
//    int sum{ 0 };
//    auto Sign = [](char c) {return !isdigit(c) && !(c == '.'); };
//    for (int turn{ 0 }; turn < 2; ++turn) {
//        auto fb = f.begin();
//        auto fe = fb;
//        while ((fb = find_if(fe, f.end(), isdigit)) != f.end()) {
//            fe = find_if_not(fb, f.end(), isdigit) + 1;
//            --fb;
//            auto sb = s.begin() + distance(f.begin(), fb);
//            auto se = s.begin() + distance(f.begin(), fe);
//            if (Sign(*fb) || Sign(*(fe-1)) || find_if(sb, se, Sign) != se) {
//                int no = *++fb - '0';
//                *fb = '.';
//                while (isdigit(*++fb)) {
//                    no = no * 10 + *fb - '0';
//                    *fb = '.';
//                }
//                sum += no;
//            }
//        }
//        f.swap(s);
//    }
//    return sum;
//}
//
//void Day3_1()
//{
//    int result{ 0 };
//    ifstream ifs("day3_1.txt");
//    string first,second;
//    getline(ifs, first);
//    auto Expand = [](string& s) { s = "." + s + ".'"; };
//    Expand(first);
//    while (getline(ifs, second)) {
//        Expand(second);
//        result += ProcLine(first, second);
//        first = second;
//    }
//    cout << result;
//}
//
//struct Num {
//    int a;
//    int b;
//    int val;
//};
//
//vector < vector<Num>> nums;
//vector < vector<int>> gears;
//
//void Parse3_2()
//{
//    ifstream ifs("day3_2.txt");
//    string line;
//    nums.push_back(vector<Num>());
//    gears.push_back(vector<int>());
//    for (int row{ 1 }; getline(ifs, line); ++row) {
//        vector<Num> tmp_nums;
//        vector<int> tmp_gears;
//        for (int col = 0; col < line.length(); ++col) {
//            if (isdigit(line[col])) {
//                Num num;
//                num.a = col;
//                num.val = line[col] - '0';
//                while (isdigit(line[col + 1])) {
//                    ++col;
//                    num.val = num.val * 10 + line[col] - '0';
//                }
//                num.b = col;
//                tmp_nums.push_back(num);
//            }
//            else if (line[col] == '*')tmp_gears.push_back(col);
//        }
//        nums.push_back(tmp_nums);
//        gears.push_back(tmp_gears);
//    }
//    nums.push_back(vector<Num>());
//    gears.push_back(vector<int>());
//}
//
//long long FindAllGears()
//{
//    long long res{ 0 };
//    vector<int>resv;
//    auto Up = [&resv](int r, int c) {
//        for (int i = 0; i < nums[r].size(); ++i){
//            if (nums[r][i].a <= c && c <= nums[r][i].b) {
//                resv.push_back(nums[r][i].val);
//                return;
//            }
//        }
//    };
//    auto Right = [&resv](int r, int c) {
//        for (int i = 0; i < nums[r].size() && nums[r][i].a <= c + 1; ++i){
//            if (nums[r][i].a == c + 1) {
//                resv.push_back(nums[r][i].val);
//                return;
//            }
//        }
//    };
//    auto Left = [&resv](int r, int c) {
//        for (int i = 0; i < nums[r].size() && nums[r][i].a <= c+1; ++i) {
//            if (nums[r][i].b == c - 1) {
//                resv.push_back(nums[r][i].val);
//                return;
//            }
//        }
//    };
//    for (int r{ 1 }; r < gears.size()-1; ++r) {
//        for (int i{ 0 }; i < gears[r].size(); ++i) {
//            int& c = gears[r][i];
//            Up(r - 1, c);
//            Left(r - 1, c);
//            Right(r - 1, c);
//            Left(r, c);
//            Right(r, c);
//            Up(r+1, c);
//            Left(r+1, c);
//            Right(r+1, c);
//            if (resv.size() > 1) {
//                for (int j{ 0 }; j < resv.size() - 1; ++j)
//                    for (int k{ j+1 }; k < resv.size(); ++k)
//                        res += resv[j] * resv[k];
//            }
//            resv.clear();
//        }
//    }
//    return res;
//}
//
//void Day3_2()
//{
//    long long result{ 0 };
//    Parse3_2();
//    
//    cout << '\n'<< FindAllGears();
//}
//
//set<int> GetNums4_1(string& line, size_t a, size_t b)
//{
//    set<int> s;
//    while (a < b) {
//        while (line[a] ==' ')++a;
//        s.insert(atol(&line[a]));
//        while (a < b && line[a] != ' ')++a;
//    }
//    return s;
//}
//
//void Day4_1()
//{
//    /*
//Card   1: 13  4 61 82 80 41 31 53 50  2 | 38 89 26 79 94 50  2 74 31 92 80 41 13 97 61 82 68 45 64 39  4 53 90 84 54
//    */
//    long long result{ 0 };
//    ifstream ifs("day4_1.txt");
//    string line;
//    while (getline(ifs, line)) {
//        size_t a = line.find(':');
//        size_t b = line.find('|');
//        set<int> win = GetNums4_1(line, a+1, b-1);
//        set<int> card = GetNums4_1(line, b + 1, line.length());
//        long long prod{ 0 };
//        for (auto& i : win) {
//            if (card.count(i)) 
//                prod = prod ? prod * 2 : 1;
//        }
//        result += prod;
//    }
//    cout << result << '\n';
//}
//
//void Day4_2()
//{
//    ifstream ifs("day4_2.txt");
//    string line;
//    struct Cards {
//        Cards(int w) :win(w), copy(1) {}
//        int win;
//        int copy;
//    };
//    vector<Cards> v;
//    while (getline(ifs, line)) {
//        size_t a = line.find(':');
//        size_t b = line.find('|');
//        set<int> win = GetNums4_1(line, a + 1, b - 1);
//        set<int> card = GetNums4_1(line, b + 1, line.length());
//        long long prod{ 0 };
//        for (auto& i : win) if (card.count(i)) ++prod;
//        v.emplace_back(prod);
//    }
//    for (int i = 0; i < v.size(); ++i) {
//        for (int j = i + 1; j < i+1 + v[i].win; j++) {
//            v[j].copy += v[i].copy;
//        }
//    }
//    int res{ 0 };
//    for (auto& i : v) res += i.copy;
//    cout << res << '\n';
//
//}

//struct Map {
//    Map() :data[0]
//    {
//    }
//    long long data[8];
//    //long long location_;
//    //long long humidity_;
//    //long long temperature_;
//    //long long light_;
//    //long long water_;
//    //long long fertilizer_;
//    //long long soil_;
//    //long long seed_;
//    //bool operator<(const Map& r) {
//        //return make_tuple(this->data[0], this->data[1], this->data[2], this->data[3], this->data[4], this->data[5], this->data[6], this->data[7])
//               //< make_tuple(r.data[0], r.data[1], r.data[2], r.data[3], r.data[4], r.data[5], r.data[6], r.data[7]);
//    //}
//};
//bool operator<(const Map& l, const Map& r) {
//    return make_tuple(l.data[0], l.data[1], l.data[2], l.data[3], l.data[4], l.data[5], l.data[6], l.data[7])
//        < make_tuple(r.data[0], r.data[1], r.data[2], r.data[3], r.data[4], r.data[5], r.data[6], r.data[7]);
//}

//struct Range {
//    Range():a1(0),b1(0),a2(0),b2(0){}
//    long long a1;
//    long long b1;
//    long long a2;
//    long long b2;
//    bool In(long long probe, long long& found) {
//        if (probe >= a1 && probe <= b1) {
//            found = a2 + probe - a1;
//            return true;
//        }
//        return false;
//    }
//}; 

//vector<long long> seed_map;
//
//void ProcRange(vector<Range> &r)
//{
//    for (auto &i : seed_map) {
//        long long found = -1;
//        for (auto& j : r) if (j.In( i, found)) break;
//        if (found >= 0) i = found;
//        else i = i;
//    }
//}
//
//
//void Day5_1()
//{
//    ifstream ifs("day5_1.txt");
//    string line;
//    getline(ifs, line);
//    for (int i = 0; i < line.length(); ++i) {
//        if (isdigit(line[i])) {
//            seed_map.push_back(atoll(&line[i]));
//            while (isdigit(line[++i]));
//        }
//    }
//    getline(ifs, line);
//    getline(ifs, line);
//    vector<Range> r;
//    while (getline(ifs, line)) {
//        if (line.empty()) {
//            getline(ifs, line);
//            ProcRange(r);
//            r.clear();
//            continue;
//        }
//        istringstream is;
//        is.str(line);
//        Range tmp;
//        long long len;
//        is >> tmp.a2 >> tmp.a1 >> len;
//        tmp.b1 = tmp.a1 + len - 1;
//        tmp.b2 = tmp.a2 + len - 1;
//        r.push_back(tmp);
//    }
//    ProcRange(r);
//
//    sort(seed_map.begin(), seed_map.end());
//
//}

//struct RangeMap {
//    RangeMap() :src_b(0), src_e(0), dest_b(0), dest_e(0) {}
//    long long src_b;        
//    long long src_e;
//    long long dest_b;
//    long long dest_e;
//
//};
//
//bool operator<(const RangeMap& l, const RangeMap& r)
//{
//    return make_tuple(l.src_b, l.src_e) < make_tuple(r.src_b, r.src_e);
//}
//
//
//struct Range {
//    Range(long long beg, long long end): b(beg), e(end){}
//    long long b;
//    long long e;
//};
//
//bool operator<(const Range& l, const Range& r)
//{
//    return make_tuple(l.b, l.e) < make_tuple(r.b, r.e);
//}
//
//vector<Range> seed_map;
//
//void ProcRanges(const vector<RangeMap>& rm/*, vector<Range> &nr*/)
//{
//    vector<Range> nr;
//    for (auto& range_map : rm) {
//        for (int i = 0; i < seed_map.size(); ) {
//            auto Encode = [&range_map](long long probe) { return range_map.dest_b + (probe - range_map.src_b); };
//            auto In = [&range_map](long long probe) { return probe >= range_map.src_b && probe <= range_map.src_e; };
//            if ( seed_map[i].e < range_map.src_b || seed_map[i].b > range_map.src_e) { // 1..2 || 7..8  vs  5..6 
//                ++i;
//                continue;
//            }
//            if (seed_map[i].b >= range_map.src_b && seed_map[i].e <= range_map.src_e) {// 10..12  in 9..15 encode
//                nr.emplace_back(Encode(seed_map[i].b), Encode(seed_map[i].e));
//                seed_map.erase(seed_map.begin()+i);
//                continue;
//            }
//            if (seed_map[i].b < range_map.src_b && seed_map[i].e > range_map.src_e) {// 1..20 in 5..15  encode 5..15   store 1..4 16..20
//                nr.emplace_back(range_map.dest_b, range_map.dest_e);
//                seed_map.insert(seed_map.begin()+i+1, Range(seed_map[i].b, range_map.src_b - 1));
//                seed_map.insert(seed_map.begin() + i + 2, Range(range_map.src_e + 1, seed_map[i].e));
//                seed_map.erase(seed_map.begin() + i);
//                i += 2;
//                continue;
//            }
//            if (seed_map[i].b < range_map.src_b && In(seed_map[i].e) ) {// 1..14 in 5..15  encode 5..14   store 1..4
//                nr.emplace_back(range_map.dest_b, Encode(seed_map[i].e));
//                seed_map.insert(seed_map.begin() + i + 1, Range(seed_map[i].b, range_map.src_b - 1));
//                seed_map.erase(seed_map.begin() + i);
//                ++i;
//                continue;
//            }
//            if (In(seed_map[i].b) && range_map.src_e < seed_map[i].e) {// 5..20 in 5..15  encode 5..15   store 16..20
//                nr.emplace_back(Encode(seed_map[i].b), range_map.dest_e);
//                seed_map.insert(seed_map.begin() + i + 1, Range(range_map.src_e+1, seed_map[i].e));
//                seed_map.erase(seed_map.begin() + i);
//                ++i;
//                continue;
//            }
//        }
//    }
//    if (!seed_map.empty()) {
//        nr.insert(nr.end(), seed_map.begin(), seed_map.end());
//    }
//    seed_map.swap(nr);
//
//}
//
//
//void Day5_2()
//{
//        ifstream ifs("day5_1.txt");
//        string line;
//        getline(ifs, line);
//        auto seeds = find_if(line.begin(), line.end(), [](char ch) {return isdigit(ch); });
//        {
//            istringstream is;
//            is.str(string(seeds, line.end()));
//            while (!is.eof()) {
//                long long b, len;
//                is >> b >> len;
//                seed_map.emplace_back(b, b + len - 1);
//            }
//        }
//        getline(ifs, line);
//        getline(ifs, line);
//        vector<RangeMap> r;
//    //Семена 79, почва 81, удобрения 81, вода 81, свет 74, температура 78, влажность 78, местоположение 82.
//    //Семена 14, почва 14, удобрения 53, вода 49, свет 42, температура 42, влажность 43, местоположение 43.
//    //Семена 55, почва 57, удобрения 57, вода 53, свет 46, температура 82, влажность 82, местоположение 86.
//    //Семена 13, почва 13, удобрения 52, вода 41, свет 34, температура 34, влажность 35, местоположение 35.
//        while (getline(ifs, line)) {
//            if (line.empty()) {
//                getline(ifs, line);
//                sort(r.begin(), r.end());
//                sort(seed_map.begin(), seed_map.end());
//                ProcRanges(r);
//                r.clear();
//                continue;
//            }
//            istringstream is;
//            is.str(line);
//            RangeMap tmp;
//            long long len;
//            is >> tmp.dest_b >> tmp.src_b >> len;
//            tmp.dest_e = tmp.dest_b + len - 1;
//            tmp.src_e = tmp.src_b + len - 1;
//            r.push_back(tmp);
//        }
//        ProcRanges(r);
//    
//        sort(seed_map.begin(), seed_map.end());
//        std::cout << seed_map[0].b << '\n';
//}

//void Day6_1()
//{
//    ifstream ifs("day6_1.txt");
//    string line;
//    getline(ifs, line);
//    vector<long long> time;
//    vector<long long> dist;
//    istringstream is;
//    is.str(string(find_if(line.begin(), line.end(), [](char ch) {return isdigit(ch); }), line.end()));
//    while (!is.eof()) {
//        long long val;
//        is >> val;
//        time.push_back(val);
//    }
//    is.clear();
//    getline(ifs, line);
//    is.str(string(find_if(line.begin(), line.end(), [](char ch) {return isdigit(ch); }), line.end()));
//    while (!is.eof()) {
//        long long val;
//        is >> val;
//        dist.push_back(val);
//    }
//    long long res{ 1 };
//    for (int i = 0; i < time.size(); ++i) {
//        long long cres{ 0 };
//        for(int j=1; j < time[i] - 1; ++j) if ( (time[i] - j)*j > dist[i] ) ++cres;
//        if (cres) res *= cres;
//    }
//    cout <<res;
//}

//void Day6_2()
//{
//    ifstream ifs("day6_1.txt");
//    string line;
//    string val;
//    long long res{ 0 };
//    getline(ifs, line);
//    copy_if(line.begin(), line.end(), back_inserter(val), [](char ch) {return isdigit(ch); });
//    long long time = atoll(val.c_str());
//    val.clear();
//    getline(ifs, line);
//    copy_if(line.begin(), line.end(), back_inserter(val), [](char ch) {return isdigit(ch); });
//    long long dist = atoll(val.c_str());
//    for (long long j = 1; j < time - 1; ++j) if ((time - j) * j > dist) ++res;
//    cout << res;
//}

//6 Five of a kind, where all five cards have the same label : AAAAA
//5 Four of a kind, where four cards have the same label and one card has a different label : AA8AA
//4 Full house, where three cards have the same label, and the remaining two cards share a different label : 23332
//3 Three of a kind, where three cards have the same label, and the remaining two cards are each different from any other card in the hand : TTT98
//2 Two pair, where two cards share one label, two other cards share a second label, and the remaining card has a third label : 23432
//1 One pair, where two cards share one label, and the other three cards have a different label from the pair and each other : A23A4
//0 High card, where all cards' labels are distinct: 23456

//
//struct Hand {
//    Hand(string h, int b) :hand(h), bid(b) { rank = Classify(); }
//    string hand;
//    int rank;
//    int bid;
//    int jokers;
//    int Classify();
//};
//
//bool operator<(const Hand& l, const Hand& r)
//{
//    static char card_rank[] = "AKQT98765432J";
//    if (l.rank > r.rank) return true;
//    if (l.rank < r.rank) return false;
//    for (int pos{ 0 }; pos < 5; ++pos) {
//        if (strchr(card_rank, l.hand[pos]) > strchr(card_rank, r.hand[pos])) return false;
//        else if (strchr(card_rank, l.hand[pos]) < strchr(card_rank, r.hand[pos])) return true;
//    }
//    return true;
//}
//
//int Hand::Classify()
//{
//    map<char, int> tmp;
//    auto FindN = [&tmp](int n) {
//        for (auto& i : tmp) if (i.second == n ) return true;
//        return false;
//        };
//    jokers = 0;
//    for (auto& ch : hand) if (ch == 'J') ++jokers; else  ++tmp[ch];
//    if (!jokers) 
//        switch (tmp.size())
//        {
//        case 1: return 6;
//        case 2: if (FindN(3)) return 4; else return 5;
//        case 3: if (FindN(3)) return 3; else return 2;
//        case 4: return 1;
//        case 5: return 0;
//        }
//    int max_len{ 0 };
//    for (auto& i : tmp) max_len = max(max_len, i.second);
//    switch (max_len + jokers) {
//    case 5: return 6;
//    case 4: return 5;
//    case 3: if (tmp.size() == 2) return 4; else return 3;
//    case 2: return 1;
//    }
//}
//
//void Day7_2()
//{
//    ifstream ifs("day7_1.txt");
//    string line;
//    vector<Hand> hand;
//    while (!ifs.eof()) {
//        string h;
//        int b;
//        ifs >> h >> b;
//        hand.emplace_back(h, b);
//    }
//    sort(hand.begin(), hand.end());
//    long long res{ 0 };
//    for (int i = 0; i < hand.size(); ++i) {
//        res += (hand.size() - i) * hand[i].bid;
//    }
//    cout << res;
//}

//struct Move {
//    Move() {}
//    Move(string l, string r) :left(l), right(r) {}
//    string left;
//    string right;
//    string Turn(char dir)
//    {
//        return (dir == 'L') ? left : right;
//    }
//};
//
//void Day8_1()
//{
//    ifstream ifs("day8_1.txt");
//    string line;
//    string dir;
//    map<string, Move> m;
//    getline(ifs, dir);
//    getline(ifs, line);
//    while (getline(ifs, line)) {
//        m[line.substr(0, 3)] = Move(line.substr(7, 3), line.substr(12, 3));
//    }
//    long long moves{ 1 };
//    size_t turn{ 0 };
//    string next = m["AAA"].Turn(dir[turn]);
//    while (next != "ZZZ") {
//        turn = ++turn% dir.length();
//        next = m[next].Turn(dir[turn]);
//        ++moves;
//    }
//    cout << moves;
//
//}


//struct Move {
//    Move() {}
//    Move(string l, string r) :left(l), right(r) {}
//    const string& Turn(char dir) const
//    {
//        return (dir == 'L') ? left : right;
//    }
//private:
//    string left;
//    string right;
//};
//
//void Day8_2()
//{
//    ifstream ifs("day8_2.txt");
//    string line;
//    string dir;
//    map<string, Move> m;
//    getline(ifs, dir);
//    getline(ifs, line);
//    while (getline(ifs, line)) m[line.substr(0, 3)] = Move(line.substr(7, 3), line.substr(12, 3));
//    vector<string> nodes;
//    for (auto& i : m) if (i.first[2] == 'A') nodes.push_back(i.first);
//    vector<uint64_t> periods;
//    for (auto& i : nodes) {
//        uint64_t move{ 0 };
//        cout << i;
//        do {
//            i = m[i].Turn(dir[move++ % dir.length()]);
//        } while (i[2] != 'Z');
//        periods.push_back(move);
//        cout << ' ' << move;
//        cout << '\n';
//    }
//    uint64_t res = 
//        boost::math::lcm(periods[0],periods[1]);
//    for (size_t i{ 2 }; i < periods.size(); ++i) res = boost::math::lcm(res, periods[i]);
//    cout << '\n'<<res;
//}

//void Day9_1()
//{
//    ifstream ifs("day9_1.txt");
//    string line;
//    uint64_t res{ 0 };
//    while (getline(ifs, line)) {
//        vector<vector< uint64_t>> seq;
//        istringstream is;
//        is.str(line);
//        seq.push_back(vector<uint64_t>(istream_iterator<uint64_t>(is), istream_iterator<uint64_t>()));
//        size_t prev{ 0 };
//        size_t next{ 1 };
//        bool not_all_zeroes{ false };
//        do {
//            not_all_zeroes = false;
//            seq.push_back(vector<uint64_t>());
//            for (int i = 1; i < seq[prev].size(); ++i) {
//                uint64_t tmp = seq[prev][i] - seq[prev][i-1];
//                seq[next].push_back(tmp);
//                not_all_zeroes |= (tmp != 0);
//            }
//            ++prev; ++next;
//            
//        } while (not_all_zeroes);
//        for (auto& i : seq) res += i.back();
//    }
//    cout << res << '\n';
//}

//void Day9_2()
//{
//    ifstream ifs("day9_1.txt");
//    string line;
//    int64_t res{ 0 };
//    while (getline(ifs, line)) {
//        vector<vector< int64_t>> seq;
//        istringstream is;
//        is.str(line);
//        seq.push_back(vector<int64_t>(istream_iterator<int64_t>(is), istream_iterator<int64_t>()));
//        int prev{ 0 };
//        int next{ 1 };
//        bool not_all_zeroes{ false };
//        do {
//            not_all_zeroes = false;
//            seq.push_back(vector<int64_t>());
//            for (int i = 1; i < seq[prev].size(); ++i) {
//                uint64_t tmp = seq[prev][i] - seq[prev][i - 1];
//                seq[next].push_back(tmp);
//                not_all_zeroes |= (tmp != 0);
//            }
//            ++prev; ++next;
//        } while (not_all_zeroes);
//        int i = seq.size() - 2;
//        int64_t val = seq[i].front();
//        for (--i; i >= 0; --i) val = seq[i].front() - val;
//        res += val;
//    }
//    cout << res << '\n';
//}

//struct Dot
//{
//    Dot(int r=0, int c=0) :point_(make_tuple(r,c)) {}
//    Dot(const Dot& d) :point_(d.point_) {}
//    int R() const 
//    {
//        return get<0>(point_);
//    }
//    int C() const
//    {
//        return get<1>(point_);
//    }
//    const Dot operator +=(Dot r) { return Dot(get<0>(point_) + get<0>(r.point_), get<1>(point_) + get<1>(r.point_)); }
//    bool operator==(const Dot& r) const { return point_ == r.point_; }
//    bool operator!=(const Dot& r) const { return point_ != r.point_; }
//    bool operator < (const Dot& r) const { return point_ < r.point_; }
//    friend ostream& operator<<(ostream& os, const Dot& r) { os << '(' << r.R() << ',' << r.C() << ')'; return os; }
//protected:
//    tuple<int, int> point_;
//};
//
//const Dot& operator +(const Dot &l, const Dot& r) { Dot tmp(l); return tmp += r; }
//
//Dot S{ 1,0 }, N{ -1, 0 }, W{ 0, -1 }, E{ 0, 1 };
//
//struct Connector
//{
//    Connector(const Dot& d1, const Dot& d2) :m1_(d1), m2_(d2) {}
//    Connector(int r1 = 0, int c1 = 0, int r2 = 0, int c2 = 0) :m1_(Dot(r1, c1)), m2_(Dot(r2, c2)) {}
//    Dot m1_;
//    Dot m2_;
//    bool Connected(const Dot& d) {
//        if (d == N) return m1_ == S || m2_ == S;
//        if (d == S) return m1_ == N || m2_ == N;
//        if (d == W) return m1_ == E || m2_ == E;
//        if (d == E) return m1_ == W || m2_ == W;
//    }
//};
//
//map<char, Connector> Connectors{ {'|',{N,S}},{'-',{E,W}},{'L',{N,E}},{'J',{N,W}},{'7',{S,W}},{'F',{S,E}} };
//
//    //    | is a vertical pipe connecting north and south.
//    //    - is a horizontal pipe connecting east and west.
//    //    L is a 90 - degree bend connecting north and east.
//    //    J is a 90 - degree bend connecting north and west.
//    //    7 is a 90 - degree bend connecting south and west.
//    //    F is a 90 - degree bend connecting south and east.
//
//vector<vector<char>> board;
//deque<Dot> to_view;
//set<Dot> cycle;
//vector<vector<int>> dist;
//
//bool CanMove(const Dot &pos, const Dot& dir)
//{
//    if (board[pos.R()][pos.C()] == '.' || dist[pos.R()][pos.C()] != -1) return false;
//    return Connectors[board[pos.R()][pos.C()]].Connected(dir);
//}
//
//
//int Day10_1Solve()
//{
//    for (int r=0; to_view.empty() && r < board.size() ; ++r) {
//        for (int c = 0; c < board[r].size(); ++c)
//            if (board[r][c] == 'S') {
//                to_view.push_front(Dot(r, c));
//                break;
//            }
//    }
//    for(auto& i : board) dist.push_back(vector<int>(board[0].size(), -1));
//    dist[to_view.front().R()][to_view.front().C()] = 0;
//    Dot start = to_view.front();
//    cycle.insert(start);
//    int max_path{ 0 };
//    while( !to_view.empty()){
//        Dot current_cell{ to_view.front()};
//        auto CheckDir = [&](const Dot& dir) {
//            Dot check = current_cell + dir;
//            if (CanMove(check, dir)) {
//                dist[check.R()][check.C()] = dist[check.R()][check.C()] == -1? dist[current_cell.R()][current_cell.C()] + 1: 
//                    min(dist[check.R()][check.C()], dist[current_cell.R()][current_cell.C()]+1);
//                max_path = max(max_path, dist[check.R()][check.C()]);
//                to_view.push_back(check);
//                cycle.insert(check);
//            }
//        };
//        if ( current_cell.R()) CheckDir(N);
//        if (current_cell.R() < board.size()-1 ) CheckDir(S);
//        if (current_cell.C() ) CheckDir(W);
//        if (current_cell.C() <  board[0].size()-1) CheckDir(E);
//        to_view.pop_front();
//        //for (auto& i : to_view) cout << i << ' ';
//        //cout << '\n';
//        //for (auto& i : board) {
//        //    for (auto& j : i) cout << j;
//        //    cout << '\n';
//        //}
//        //for (auto& i : dist) {
//        //    for (auto& j : i) cout << '\t'<<j;
//        //    cout << '\n';
//        //}
//    } 
//    return max_path;//6754
//}
//
//void Day10_1()
//{
//    ifstream ifs("day10_1.txt");
//    string line;
//    while (getline(ifs, line)) board.emplace_back(line.begin(), line.end());
//    cout<<Day10_1Solve()<<'\n';
//}

struct Dot
{
    Dot(int r = 0, int c = 0) :point_(make_tuple(r, c)) {}
    Dot(const Dot& d) :point_(d.point_) {}
    int R() const
    {
        return get<0>(point_);
    }
    int C() const
    {
        return get<1>(point_);
    }
    const Dot operator +=(Dot r) { return Dot(get<0>(point_) + get<0>(r.point_), get<1>(point_) + get<1>(r.point_)); }
    bool operator==(const Dot& r) const { return point_ == r.point_; }
    bool operator!=(const Dot& r) const { return point_ != r.point_; }
    bool operator < (const Dot& r) const { return point_ < r.point_; }
    friend ostream& operator<<(ostream& os, const Dot& r) { os << '(' << r.R() << ',' << r.C() << ')'; return os; }
protected:
    tuple<int, int> point_;
};

const Dot& operator +(const Dot& l, const Dot& r) { Dot tmp(l); return tmp += r; }

Dot S{ 1,0 }, N{ -1, 0 }, W{ 0, -1 }, E{ 0, 1 };

struct Connector
{
    Connector(const Dot& d1, const Dot& d2) :m1_(d1), m2_(d2) {}
    Connector(int r1 = 0, int c1 = 0, int r2 = 0, int c2 = 0) :m1_(Dot(r1, c1)), m2_(Dot(r2, c2)) {}
    Dot m1_;
    Dot m2_;
    bool Connected(const Dot& d) {
        if (d == N) return m1_ == S || m2_ == S;
        if (d == S) return m1_ == N || m2_ == N;
        if (d == W) return m1_ == E || m2_ == E;
        if (d == E) return m1_ == W || m2_ == W;
    }
};

map<char, Connector> Connectors{ {'|',{N,S}},{'-',{E,W}},{'L',{N,E}},{'J',{N,W}},{'7',{S,W}},{'F',{S,E}} };

//    | is a vertical pipe connecting north and south.
//    - is a horizontal pipe connecting east and west.
//    L is a 90 - degree bend connecting north and east.
//    J is a 90 - degree bend connecting north and west.
//    7 is a 90 - degree bend connecting south and west.
//    F is a 90 - degree bend connecting south and east.

vector<vector<char>> board;
deque<Dot> to_view;
set<Dot> cycle;
vector<vector<int>> dist;

bool CanMove(const Dot& pos, const Dot& dir)
{
    if (board[pos.R()][pos.C()] == '.' || dist[pos.R()][pos.C()] != -1) return false;
    return Connectors[board[pos.R()][pos.C()]].Connected(dir);
}

void ProcOuter(int r, int c)
{
    to_view.emplace_back(r, c);
    while (!to_view.empty()) {
        Dot current_cell{ to_view.front() };
        board[current_cell.R()][current_cell.C()] = 'O';
        auto TryMove = [&](Dot& dir) {
            Dot check = current_cell + dir;
            if (board[check.R()][check.C()] != '.') return;
            board[check.R()][check.C()] = 'O';
            to_view.push_back(check);
            };
        if (current_cell.R()) TryMove(N);
        if (current_cell.R() != board.size()-1) TryMove(S);
        if (current_cell.C()) TryMove(W);
        if (current_cell.C() != board[0].size()-1) TryMove(E);
        to_view.pop_front();
    }
}


int Day10_2Solve()
{
    for (int r = 0; to_view.empty() && r < board.size(); ++r) {
        for (int c = 0; c < board[r].size(); ++c)
            if (board[r][c] == 'S') {
                to_view.push_front(Dot(r, c));
                break;
            }
    }
    for (auto& i : board) dist.push_back(vector<int>(board[0].size(), -1));
    dist[to_view.front().R()][to_view.front().C()] = 0;
    Dot start = to_view.front();
    cycle.insert(start);
    while (!to_view.empty()) {
        Dot current_cell{ to_view.front() };
        auto CheckDir = [&](const Dot& dir) {
            Dot check = current_cell + dir;
            if (CanMove(check, dir)) {
                dist[check.R()][check.C()] = 1;
                to_view.push_back(check);
                cycle.insert(check);
            }
            };
        if (current_cell.R()) CheckDir(N);
        if (current_cell.R() < board.size() - 1) CheckDir(S);
        if (current_cell.C()) CheckDir(W);
        if (current_cell.C() < board[0].size() - 1) CheckDir(E);
        to_view.pop_front();
        //for (auto& i : to_view) cout << i << ' ';
        //cout << '\n';
        //for (auto& i : board) {
        //    for (auto& j : i) cout << j;
        //    cout << '\n';
        //}
        //for (auto& i : dist) {
        //    for (auto& j : i) cout << '\t'<<j;
        //    cout << '\n';
        //}
    }
    //for (auto& i : board) {
    //    for (auto& j : i) cout << j;
    //    cout << '\n';
    //}
    //for (auto& i : dist) {
    //    for (auto& j : i) cout << '\t'<<j;
    //    cout << '\n';
    //}
    for (int r{ 0 }; r < board.size(); ++r) {
        for (int c{ 0 }; c < board[0].size(); c += board[0].size())
            if (board[r][c] == '.') ProcOuter(r, c);
    }
    for (int r{ 0 }; r < board.size(); r+= board.size()-1) {
        for (int c{ 0 }; c < board[0].size(); c++) {
            if (board[r][c] == '.') ProcOuter(r, c);
        }
    }
    for (int r{ 0 }; r < board.size(); ++r) {
        for (int c{ 0 }; c < board[0].size(); ++c)
            if (board[r][c] == '.') ProcInner(r, c);
    }
    for (auto& i : board) {
        for (auto& j : i) cout << j;
        cout << '\n';
    }
    int res{ 0 };
    for (auto& i : board) 
        for (auto& j : i) 
            if (j == '.') 
                ++res;

    return res;//6754
}

void Day10_2()
{
    ifstream ifs("day10_2_.txt");
    string line;
    while (getline(ifs, line)) board.emplace_back(line.begin(), line.end());
    cout << Day10_2Solve() << '\n';
}

int main()
{
    setlocale(LC_ALL, "ru-RU");
//    Day1_1();
//    Day1_2();
//    Day2_1();
//    Day3_1();
    //Day3_2();
//    Day4_1();
//    Day4_2();
    //Day5_1();
    //Day5_2();
    //Day6_1();
   // Day6_2();
//    Day7_1();
//    Day7_2();
    //Day8_1();
//    Day8_2();
//    Day9_1();
    //Day9_2();
    //Day10_1();
    Day10_2();

}
