// pithagoras.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <algorithm>
#include <chrono>


template <typename T>
T intsqrt(const T& a)
{
	T r = a >> 1;
	T q;
	for (;;)
	{
		q = a / r;
		if (q >= r)
			break;
		else
			r = (r + q) >> 1;
	}

	return r;
}

void triples1(int count, char *file_name)
{
	std::ofstream ofs(file_name);
	std::vector<long long> hipv = { 9,16 };
	for (long long i = 5; i < count; ++i) {
		long long hip = i*i;
		hipv.push_back(hip);
		bool found = false;
		for (auto cat = hipv.begin(); cat != hipv.end() - 1; cat++) {
			long long other_cat = hip - *cat;
			if (other_cat < *cat) break;
			if (std::binary_search(cat, hipv.end() - 1, other_cat)) {
				if (found) ofs << '\t' << intsqrt(*cat) << '\t' << intsqrt(other_cat) << '\n';
				else {
					ofs << i << '\t' << intsqrt(*cat) << '\t' << intsqrt(other_cat) << '\n';
					found = true;
				}
			}

		}
	}
}

void triples11(int count, char *file_name)
{
	std::ofstream ofs(file_name);
	std::vector<long long> hipv = { 9,16 };
	for (long long i = 5; i < count; ++i) {
		long long hip = i*i;
		hipv.push_back(hip);
		bool found = false;
		auto end = hipv.end() - 1;
		for (auto cat = hipv.begin(); ; cat++) {
			long long other_cat = hip - *cat;
			if (other_cat < *cat) break;
			end = std::lower_bound(cat, end, other_cat);
			if (*end == other_cat ) {
				if (found) ofs << '\t' << intsqrt(*cat) << '\t' << intsqrt(other_cat) << '\n';
				else {
					ofs << i << '\t' << intsqrt(*cat) << '\t' << intsqrt(other_cat) << '\n';
					found = true;
				}
			}
		}
	}
}


void triples12(int count, char *file_name)
{
	std::ofstream ofs(file_name);
	std::vector<long long> hipv = { 9,16 };
	for (long long i = 5; i < count; ++i) {
		long long hip = i*i;
		hipv.push_back(hip);
		bool found = false;
		auto end = hipv.end() - 1;
		for (auto cat = hipv.begin(); ; cat++) {
			long long other_cat = hip - *cat;
			if (other_cat < *cat) break;
			end = std::lower_bound(cat, end, other_cat);
			if (*end == other_cat) {
				if (found) ofs << '\t' << static_cast<long>(sqrtl(static_cast<long double>(*cat))) << '\t' << static_cast<long>(sqrtl(static_cast<long double>(other_cat))) << '\n';
				else {
					ofs << i << '\t' << static_cast<long>(sqrtl(static_cast<long double>(*cat))) << '\t' << static_cast<long>(sqrtl(static_cast<long double>(other_cat))) << '\n';
					found = true;
				}
			}
		}
	}
}



void triples2(int count, char *file_name)
{
	std::ofstream ofs(file_name);
	for (long long hip = 5; hip < count; hip++) {
		long long hip2 = hip*hip;
		bool found = false;
		for (long cat = 1; cat < hip - 1; cat++) {
			long long cat2 = cat*cat;
			long long tst = intsqrt(hip2 - cat2);
			if (tst < cat) break;
			if (tst*tst + cat2 == hip2) {
				if (found) ofs << '\t' << cat << '\t' << tst << '\n';
				else {
					ofs << hip << '\t' << cat << '\t' << tst << '\n';
					found = true;
				}
				
			}
		}
	}
}

void triples3(int count, char *file_name)
{
	std::ofstream ofs(file_name);
	for (long long hip = 5; hip < count; hip++) {
		long long hip2 = hip*hip;
		bool found = false;
		for (long cat = 1; cat < hip - 1; cat++) {
			long long tst = intsqrt(hip2 - cat*cat);
			if (tst < cat) break;
			if (tst*tst + cat*cat == hip2) {
				if (found) ofs << '\t' << cat << '\t' << tst << '\n';
				else {
					ofs << hip << '\t' << cat << '\t' << tst << '\n';
					found = true;
				}
			}
		}
	}
}

void triples4(int count, char *file_name)
{
	std::ofstream ofs(file_name);
	for (long long hip = 5; hip < count; hip++) {
		long long hip2 = hip*hip;
		bool found = false;
		for (long cat = 1; cat < hip - 1; cat++) {
			long long tst = static_cast<long long>(sqrtl(static_cast<long double>(hip2 - cat*cat)));
			if (tst < cat) break;
			if (tst*tst + cat*cat == hip2) {
				if (found) ofs << '\t' << cat << '\t' << tst << '\n';
				else {
					ofs << hip << '\t' << cat << '\t' << tst << '\n';
					found = true;
				}
			}
		}
	}
}

void triples5(int count, char *file_name)
{
	std::ofstream ofs(file_name);
	for (long long hip = 5; hip < count; hip++) {
		long long hip2 = hip*hip;
		bool found = false;
		for (long cat = 1; cat < hip - 1; cat++) {
			long long cat2 = cat*cat;
			long long tst = static_cast<long long>(sqrtl(static_cast<long double>(hip2 - cat2)));
			if (tst < cat) break;
			if (tst*tst + cat2 == hip2) {
				if (found) ofs << '\t' << cat << '\t' << tst << '\n';
				else {
					ofs << hip << '\t' << cat << '\t' << tst << '\n';
					found = true;
				}
			}
		}
	}
}


void triples6(int count, char *file_name)
{
	std::ofstream ofs(file_name);
	long maxhip = 0;
	for (long long hip = 5; hip < count; hip++) {
		long found = 0;
		for (long cat = 1; cat < hip - 1; cat++) {
			long long tst = static_cast<long long>(sqrtl(static_cast<long double>(hip*hip - cat*cat)));
			if (tst < cat) break;
			if (tst*tst + cat*cat == hip*hip) {
				if (found) ofs << '\t' << cat << '\t' << tst << "\t#" << found <<'\n';
				else ofs << hip << '\t' << cat << '\t' << tst << '\n';
				++found;
			}
		}
		maxhip = std::max(maxhip, found);
	}
	ofs << maxhip;
}


int main()
{
	typedef std::chrono::high_resolution_clock Time;
	typedef std::chrono::nanoseconds ms;
	typedef std::chrono::duration<double> fsec;
	const int repeat_count = 1;
	const int pithagoras_max = 40000;
	std::ofstream ofs("d:\\tmp\\res.txt");
	auto t0 = Time::now();
	for(int i = 0; i < repeat_count; i++) triples1(pithagoras_max, "d:\\tmp\\t1.txt");
	auto t1 = Time::now();
	fsec fs = t1 - t0;
	ms d = std::chrono::duration_cast<ms>(fs);
	ofs<<"triples1\t repeat_count = "<< repeat_count<<"\tpithagoras_max = "<< pithagoras_max<< '\t' << d.count()/1000000000.<< "sec\n";
	std::cout << "triples1\t repeat_count = " << repeat_count << "\tpithagoras_max = " << pithagoras_max << '\t' << d.count() / 1000000000. << "sec\n";

	//t0 = Time::now();
	//for(int i = 0; i < repeat_count; i++) triples11(pithagoras_max, "d:\\tmp\\t11.txt");
	//t1 = Time::now();
	//fs = t1 - t0;
	//d = std::chrono::duration_cast<ms>(fs);
	//ofs << "triples11\t repeat_count = " << repeat_count << "\tpithagoras_max = " << pithagoras_max << '\t' << d.count() / 1000000000. << "sec\n";
	//std::cout << "triples11\t repeat_count = " << repeat_count << "\tpithagoras_max = " << pithagoras_max << '\t' << d.count() / 1000000000. << "sec\n";

	//t0 = Time::now();
	//for(int i = 0; i < repeat_count; i++) triples12(pithagoras_max, "d:\\tmp\\t12.txt");
	//t1 = Time::now();
	//fs = t1 - t0;
	//d = std::chrono::duration_cast<ms>(fs);
	//ofs << "triples12\t repeat_count = " << repeat_count << "\tpithagoras_max = " << pithagoras_max << '\t' << d.count() / 1000000000. << "sec\n";
	//std::cout << "triples12\t repeat_count = " << repeat_count << "\tpithagoras_max = " << pithagoras_max << '\t' << d.count() / 1000000000. << "sec\n";


	//t0 = Time::now();
	//for(int i = 0; i < repeat_count; i++) triples2(pithagoras_max, "d:\\tmp\\t2.txt");
	//t1 = Time::now();
	//fs = t1 - t0;
	//d = std::chrono::duration_cast<ms>(fs);
	//ofs << "triples2\t repeat_count = " << repeat_count << "\tpithagoras_max = " << pithagoras_max << '\t' << d.count() / 1000000000. << "sec\n";
	//std::cout << "triples2\t repeat_count = " << repeat_count << "\tpithagoras_max = " << pithagoras_max << '\t' << d.count() / 1000000000. << "sec\n";

	//t0 = Time::now();
	//for(int i = 0; i < repeat_count; i++) triples3(pithagoras_max, "d:\\tmp\\t3.txt");
	//t1 = Time::now();
	//fs = t1 - t0;
	//d = std::chrono::duration_cast<ms>(fs);
	//ofs << "triples3\t repeat_count = " << repeat_count << "\tpithagoras_max = " << pithagoras_max << '\t' << d.count() / 1000000000. << "sec\n";
	//std::cout << "triples3\t repeat_count = " << repeat_count << "\tpithagoras_max = " << pithagoras_max << '\t' << d.count() / 1000000000. << "sec\n";

	//t0 = Time::now();
	//for(int i = 0; i < repeat_count; i++) triples4(pithagoras_max, "d:\\tmp\\t4.txt");
	//t1 = Time::now();
	//fs = t1 - t0;
	//d = std::chrono::duration_cast<ms>(fs);
	//ofs << "triples4\t repeat_count = " << repeat_count << "\tpithagoras_max = " << pithagoras_max << '\t' << d.count() / 1000000000. << "sec\n";
	//std::cout << "triples4\t repeat_count = " << repeat_count << "\tpithagoras_max = " << pithagoras_max << '\t' << d.count() / 1000000000. << "sec\n";

	//t0 = Time::now();
	//for(int i = 0; i < repeat_count; i++) triples5(pithagoras_max, "d:\\tmp\\t5.txt");
	//t1 = Time::now();
	//fs = t1 - t0;
	//d = std::chrono::duration_cast<ms>(fs);
	//ofs << "triples5\t repeat_count = " << repeat_count << "\tpithagoras_max = " << pithagoras_max << '\t' << d.count() / 1000000000. << "sec\n";
	//std::cout << "triples5\t repeat_count = " << repeat_count << "\tpithagoras_max = " << pithagoras_max << '\t' << d.count() / 1000000000. << "sec\n";

	t0 = Time::now();
	for(int i = 0; i < repeat_count; i++) triples6(pithagoras_max, "d:\\tmp\\t6.txt");
	t1 = Time::now();
	fs = t1 - t0;
	d = std::chrono::duration_cast<ms>(fs);
	ofs << "triples6\t repeat_count = " << repeat_count << "\tpithagoras_max = " << pithagoras_max << '\t' << d.count() / 1000000000. << "sec\n";
	std::cout << "triples6\t repeat_count = " << repeat_count << "\tpithagoras_max = " << pithagoras_max << '\t' << d.count() / 1000000000. << "sec\n";


	return 0;
}

