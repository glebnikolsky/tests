// Chrono.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <chrono>
#include <algorithm>
#include <vector>
#include <iostream>
#include <iomanip>
#include <ctime>




void slow_motion()
{
	std::vector<int> a{ 1,2,3,4,5,6,7,8,9,10,11 };
	while ( std::next_permutation(begin(a), end(a)) )
	{
	} // generates 12! permutations
}

int main()
{
	using namespace std::literals; // enables the usage of 24h, 1ms, 1s instead of
								   // e.g. std::chrono::hours(24), accordingly

	const std::chrono::time_point<std::chrono::system_clock> now =
		std::chrono::system_clock::now();

	const std::time_t t_c = std::chrono::system_clock::to_time_t(now - 24h);

#pragma warning(disable : 4996)
	std::cout << "24 hours ago, the time was "
		<< std::put_time(std::localtime(&t_c), "%F %T.\n") << std::flush;

	std::chrono::time_point<std::chrono::steady_clock> start =
		std::chrono::steady_clock::now();

	slow_motion();

	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double, std::ratio<1, 1000000>> period = end - start;

	std::cout
		<< "Slow calculations took 3"
		<< "ms = " << std::chrono::duration_cast<std::chrono::microseconds>(period).count() << "\n";
	start = std::chrono::steady_clock::now();
	slow_motion();
	end = std::chrono::steady_clock::now();
	period += end - start;
	std::cout
		<< "Slow calculations took 2"
		<< "ms = " << std::chrono::duration_cast<std::chrono::microseconds>(period).count() << "\n";
	start = std::chrono::steady_clock::now();
	slow_motion();
	end = std::chrono::steady_clock::now();
	period += end - start;
	std::cout
		<< "Slow calculations took 3"
		<< "ms = " << std::chrono::duration_cast<std::chrono::microseconds>(period).count() << "\n";

}