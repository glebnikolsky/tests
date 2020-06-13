// BadDateConversion.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <algorithm>
#include <sstream>
#include <string>
#include <vector>
#include <boost/timer/timer.hpp>
#include <boost/format.hpp>
#include <boost/endian/conversion.hpp>
#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_int_distribution.hpp>


#pragma pack(push)
#pragma pack(1)
struct Helper {
	int16_t d;
	char p1;
	int16_t m;
	char p2;
	int32_t y;
};
#pragma pack(pop)
int main()
{
	setlocale(LC_ALL, "ru-RU");
	
	std::stringstream date;
	const int count = 1000000;
	const int trys = 1000;
	std::vector<std::string> dt(count);
	std::vector<long> dl(count, 0);
	std::vector<long long > dll(count, 0);
	auto c2l = [](const char ch) { return (long)ch - '0'; };
	auto cnv = [&c2l](const char *d) {return ((((((c2l(d[6]) * 10 + c2l(d[7])) * 10 + c2l(d[8])) * 10 + c2l(d[9])) * 10 + c2l(d[3])) * 10 + c2l(d[4])) * 10 + c2l(d[0])) * 10 + c2l(d[1]); };
	auto cnvh = [&c2l](const char *d) {return ((((((c2l(d[6]) << 4 | c2l(d[7])) << 4 | c2l(d[8])) << 4 | c2l(d[9])) << 4 | c2l(d[3])) << 4 | c2l(d[4])) << 4 | c2l(d[0])) << 4 | c2l(d[1]); };
	auto convert = [](const char *d) { return ((((((((long)d[6] - '0') * 10 + d[7] - '0') * 10 + d[8] - '0') * 10 + d[9] - '0') * 10 + d[3] - '0') * 10 + d[4] - '0') * 10 + d[0] - '0') * 10 + d[1] - '0'; };
	auto converth = [](const char *d) { return ((((((((long)d[6] - '0') << 4 | (d[7] - '0')) << 4 | (d[8] - '0')) << 4 | (d[9] - '0')) << 4 | 
		(d[3] - '0')) << 4 | (d[4] - '0')) << 4 | (d[0] - '0')) << 4 | (d[1] - '0'); };
	auto conve = [](Helper*t) { return (((long long)boost::endian::endian_reverse(t->y)) << 32) | (boost::endian::endian_reverse(t->m) << 16) | boost::endian::endian_reverse(t->d); };
	//32303230
	Helper *h = (Helper*)"01.02.1989";
	std::cout << "01.02.1989"<< ' ' <<std::hex << (h->y & 0x0f0f0f0f)<<' '<<(((h->y & 0x0f000000) >> 12) | ((h->y & 0x000f0000) >>  8) | ((h->y & 0x00000f00) << 12) | ((h->y & 0x00000000f) << 16));
	//int n{ 0 };
	//for (int y = 0; y < count / 365 + 1 && n < count; y++)
	//	for (int m = 1; m < 13 && n < count; ++m)
	//		for (int d = 1; d < 31 && n < count; ++d, ++n) {
	//			date << boost::format("%02d.%02d.%04d") % d % m % y;
	//			dt[n] = date.str();
	//			date.str("");
	//		}

	//boost::minstd_rand gen(0);
	//boost::random::uniform_int_distribution<long> dist(0, dt.size() - 1);
	//for (auto &i : dt) std::swap(i, dt[dist(gen)]);
	//std::cout << "convert ";
	//{
	//	boost::timer::auto_cpu_timer t;
	//	for (int k = 0; k < trys; ++k) {
	//		int n = 0;
	//		for (auto i : dt) dl[n++] = convert(i.c_str());
	//		std::stable_sort(begin(dl), end(dl));
	//	}
	//}
	//std::cout << "converth ";
	//{
	//	boost::timer::auto_cpu_timer t;
	//	for (int k = 0; k < trys; ++k) {
	//		int n = 0;
	//		for (auto i : dt) dl[n++] = converth(i.c_str());
	//		std::stable_sort(begin(dl), end(dl));
	//	}
	//}
	//std::cout << "cnv ";
	//{
	//	boost::timer::auto_cpu_timer t;
	//	for (int k = 0; k < trys; ++k) {
	//		int n = 0;
	//		for (auto i : dt) dl[n++] = cnv(i.c_str());
	//		std::stable_sort(begin(dl), end(dl));
	//	}
	//}
	//std::cout << "cnvh ";
	//{
	//	boost::timer::auto_cpu_timer t;
	//	for (int k = 0; k < trys; ++k) {
	//		int n = 0;
	//		for (auto i : dt) dl[n++] = cnvh(i.c_str());
	//		std::stable_sort(begin(dl), end(dl));
	//	}
	//}
	//std::cout << "conve ";
	//{
	//	boost::timer::auto_cpu_timer t;
	//	for (int k = 0; k < trys; ++k) {
	//		int n = 0;
	//		for (auto i : dt) dll[n++] = conve((Helper*)i.c_str());
	//		std::stable_sort(begin(dll), end(dll));
	//	}
	//}
	return 0;
}

