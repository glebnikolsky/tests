// hackerrank.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <deque>
#include <algorithm>
#include <iostream>
#include <stack>
#include <set>
#include <string>
#include <tuple>
#include <vector>
#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/timer/timer.hpp>

using namespace std;


// Complete the highestValuePalindrome function below.
/*string highestValuePalindrome(string s, int n, int k) {
	stack< tuple<int, int> > pos;
	for (int f = 0, l = s.length() - 1; f < l; ++f, --l) {
		if (s[f] != s[l]) pos.push(make_tuple(f, l));
	}
	if (pos.size() > k) return "-1";
	cout << pos.size() << '\n';
	while (!pos.empty()) {
		int f, l;
		tie(f,l) = pos.top();
		pos.pop();
		int tmp = k - pos.size() * 2;
		if (tmp >= 2 ){
			if (s[f] != '9' && s[l] != '9') {
				k -= 2;
				s[f] = s[l] = '9';
			}
			else{
				if (s[f] == '9') s[l] = '9';
				else s[l] = '9';
				k -= 1;
			}

		}else{
			k -= 1;
			if (s[f] > s[l]) s[l] = s[f];
			else s[f] = s[l];
		}
	}
	for (int f = 0, l = s.length() - 1; f < l && k > 1; ++f, --l) {
		if (s[f] != '9') {
			k -= 2;
			s[f] = s[l] = '9';
		}
	}

	if (k && s.length() % 2 == 1) {
		s[s.length() / 2] = '9';
		k--;
	}
	cout << k << '\n';
	return s;
}
*/

string highestValuePalindrome(string s, int n, int k) {
	deque< tuple<int, int> > positions;
	for (int f = 0, l = n - 1; f < l; ++f, --l) {
		if (s[f] != s[l]) positions.push_back(make_tuple(f, l));
	}
	if (positions.size() > k) return "-1";
	cout << positions.size() << '\n';
	while (!positions.empty()) {
		int f, l;
		int tmp = k -  (positions.size() -1) * 2;
		if (tmp >= 2) {
			tie(f, l) = positions.front();
			positions.pop_front();
			if (s[f] != '9' && s[l] != '9') {
				k -= 2;
				s[f] = s[l] = '9';
			}
			else {
				if (s[f] == '9') s[l] = '9';
				else s[l] = '9';
				k -= 1;
			}
		}
		else {
			tie(f, l) = positions.back();
			positions.pop_back();
			k -= 1;
			if (s[f] > s[l]) s[l] = s[f];
			else s[f] = s[l];
		}
	}
	for (int f = 0, l = s.length() - 1; f < l && k > 1; ++f, --l) {
		if (s[f] != '9') {
			k -= 2;
			s[f] = s[l] = '9';
		}
	}

	if (k && s.length() % 2 == 1) {
		s[s.length() / 2] = '9';
		k--;
	}
	cout << k << '\n';
	return s;
}

string itos(long long n)
{
	string res;
	char tmp[2] = { '\0','\0' };
	do {
		tmp[0] = n % 10 + '0';
		res.insert(0, tmp);
		n /= 10;
	} while (n);
	return res;
}

void separateNumbers(string s) {
	for (size_t len = 1; len <= s.length() - len; ++len) {
		long long f = _atoi64(s.substr(0, len).c_str());
		long long cur = f;
		for (size_t shift = len; shift + len <= s.length(); ) {
			string curs(itos(++cur));
			if (s.substr(shift, curs.length()) == curs)
			{
				shift += curs.length();
				if (shift == s.length()) {
					cout << "YES " << f << '\n';
					return;
				}
			}
			else break;
		}
	}
	cout << "NO\n";
}

int alternatingCharacters(string s) {
	int n{ 0 };
	char ch = s[0];
	for (int i = 1; i < s.length(); ++i) {
		if (s[i] == ch) n++;
		else ch = s[i];
	}
	return n;
}

int beautifulBinaryString(string b) {
	int pos;
	int res{ 0 };
	while ((pos = b.find("010")) != -1) {
		b[pos + 2] = '1';
		res++;
	}
	return res;
}

int theLoveLetterMystery(string s) {
	auto b = s.begin(), e = s.end()-1;
	int res{ 0 };
		while (b < e) {
			if (*b < *e) res += *e - *b;
			else if (*b > *e) res += *b - *e;
			++b; --e;
	}
	return res;
}

struct Range{
	Range(std::string name, long seed, long begin, long end, long chunk): range_name_(name), seed_(seed), a_(begin), z_(end), sz_(chunk), currentA_(-1), currentZ_(-1), pos_(0)
	{
		ReadCurrentSettings();
	}

	~Range() {
		SaveCurrentSettings();
	}
	void InitRange();				// Создать и перетасовать вектор размера sz_ для текужщего диапазона

	void ReadCurrentSettings();     // Прочитать настройки вызвать InitRange

	void SaveCurrentSettings();
	void CreateNextRange();
	long GetNextNo();

	std::string range_name_;	//Название диапазона
	long seed_;					//Сид для распределения
	long a_;					//Начало диапазона a_..z_
	long z_;					//Конец диапазона
	long sz_;					//Размер текущего отрезка цифр currentA_..currentZ_
	long currentA_;				//Начало текущего отрезка 
	long currentZ_;				//Конец текущего отрезка currentA_+ sz_- 1
	size_t pos_;				//текущая позиция
	vector<long> numbers_;		//сортированый отрезок
};

void Range::ReadCurrentSettings()
{
	std::cout << "ReadCurrentSettings()\n";
	/* Прочитать
	long seed_;					//Сид для распределения
	long a_;					//Начало диапазона a_..z_
	long z_;					//Конец диапазона
	long sz_;					//Размер текущего отрезка цифр currentA_..currentZ_
	long currentA_;				//Начало текущего отрезка
	long currentZ_;				//Конец текущего отрезка currentA_+ sz_- 1
	size_t pos_;				//текущая позиция
	*/
	if (currentA_ < 0) CreateNextRange();
	else InitRange();
}

void Range::SaveCurrentSettings()
{
	std::cout << "SaveCurrentSettings()\n";

/* Сохранить
long seed_;					//Сид для распределения
long a_;					//Начало диапазона a_..z_
long z_;					//Конец диапазона
long sz_;					//Размер текущего отрезка цифр currentA_..currentZ_
long currentA_;				//Начало текущего отрезка
long currentZ_;				//Конец текущего отрезка currentA_+ sz_- 1
size_t pos_;				//текущая позиция
*/
}

long Range::GetNextNo()
{
	if (pos_ == numbers_.size()){
		CreateNextRange();
		SaveCurrentSettings();
	}
	return numbers_[pos_++];
}


void Range::CreateNextRange()
{
	pos_ = 0;
	if (currentA_ == -1) {
		currentA_ = a_;
		currentZ_ = a_ + sz_-1;
	}
	else {
		currentA_ += sz_;
		currentZ_ += sz_;
	}
	if (currentZ_ > z_) {
		std::string msg = _T("Исчерпан диапазон ") + range_name_;
		throw out_of_range(msg);
	}
	InitRange();
}

void Range::InitRange()
{
	vector<long> tmp(currentZ_ - currentA_ + 1);
	boost::minstd_rand gen(seed_);
	boost::random::uniform_int_distribution<long> dist(0, tmp.size() - 1);
	long j = currentA_;
	for (size_t i = 0; i < tmp.size(); ++i) tmp[i] = j++;
	for (auto &i : tmp) swap(i, tmp[dist(gen)]);
	swap(tmp, numbers_);
}



int main()
{
	//typedef tuple<string, long long> Gene;
	//int ngenes;
	//cin >> ngenes;
	//vector<Gene> genes;
	//int n = ngenes;
	//while (n--) {
	//	string s;
	//	cin >> s;
	//	genes.push_back(make_tuple(s, 0));
	//}
	//n = ngenes;
	//for (int i = 0; i < ngenes; ++i) {
	//	int h;
	//	cin >> h;
	//	genes[i] = make_tuple(get<0>(genes[i]), h);
	//}
	//int probes;
	//cin >> probes;
	//typedef tuple<int, int> Result;
	//long long maxres{ 0 }, minres{ -1 };
	//for (n = 0; n < probes; ++n) {
	//	int f, l;
	//	cin >> f >> l;
	//	string test;
	//	cin >> test;
	//	long long curr{ 0 };
	//	for (int i = f; i <= l; i++) {
	//		int pos = 0;
	//		while ((pos = test.find(get<0>(genes[i]), pos)) != -1) {
	//			curr += get<1>(genes[i]);
	//			++pos;
	//		}
	//	}
	//	maxres = max(maxres, curr);
	//	if (minres < 0) minres = curr;
	//	else minres = min(minres, curr);
	//}
	//cout << minres << ' ' << maxres;
	Range num("abc", 123, 1000001, 1900000, 10000);
	vector<long> test;
	try {
		for (int i = 0; i <= 100000000; i++) {
			test.push_back(num.GetNextNo());
	}
	}
	catch (out_of_range e) {
		cout << e.what()<<'\n';
	}
	sort(test.begin(), test.end());
	for (size_t i = 0, j = 1; j < test.size(); i++, j++) {
		if (test[i] + 1 != test[j]) {
			cout << "Pizdec!\n";
		}
	}

	return 0;
}

