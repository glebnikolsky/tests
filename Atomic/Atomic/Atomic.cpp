// Atomic.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <thread>
#include <vector>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <atomic>
#include <mutex>
#include <boost\interprocess\sync\scoped_lock.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include "boost/date_time/posix_time/posix_time.hpp"


struct Test {
	Test() : l_(0) {}
	std::mutex m_;
	long l_;
};


Test tst;

std::vector < std::pair<int, int> >res;

void f(int n, int delay)
{
	
	for (;;) {
		if (tst.l_ > 1000) return;
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		while (!tst.m_.try_lock()) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
		res.emplace_back(n, ++tst.l_);// push_back(std::make_pair(n, ++tst.l_));
		std::this_thread::sleep_for(std::chrono::milliseconds(delay));
		std::cout << "Output from thread " << n << '\t' << tst.l_ << '\n';
		tst.m_.unlock();
	}
}



int main()
{
	boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
	std::cout << to_iso_extended_string(now) << '\n';
	now += boost::posix_time::time_duration(0, 0, 100000);
	std::cout << to_iso_extended_string(now) << '\n';

	std::cout << to_iso_extended_string(boost::posix_time::second_clock::local_time()) << '\n';
	std::vector<int > t = { 41 , 7, 11, 13, 17, 19, 23, 29, 31, 37 };
	std::vector<std::thread> v;

	for (int n = 0; n < 100; ++n) {
		v.emplace_back(f, n, t[n%10]);
	}
	for (auto& t : v) {
		t.join();
	}
	
	std::sort(begin(res), end(res));
	for (auto i : res)
		std::cout << i.first << '\t' << i.second << '\n';
	boost::posix_time::ptime there = boost::posix_time::second_clock::local_time();

}