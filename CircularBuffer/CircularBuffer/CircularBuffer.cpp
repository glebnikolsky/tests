// CircularBuffer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"



#include <algorithm>
#include <iostream>
#include <chrono>
#include <numeric>
#include <thread>
#include <mutex>
#include <memory>
#include <vector>
#include <assert.h>
#include <sstream>
#include <boost/circular_buffer.hpp>
#include <boost/format.hpp>
#include <boost/timer/timer.hpp>

#include "ThreadSafeQueue.h"



//template<typename T>
//struct queue_ts {
//	queue_ts(size_t bs = 100) : buf_size_(bs), q_(boost::circular_buffer<T>(buf_size_))
//	{
//		
//	}
//
//	void push_back(const T &ts)
//	{
//		std::lock_guard<std::mutex> guard(mtx_);
//		if (q_.full()) {
//			boost::circular_buffer<T> tmp(q_.capacity() + buf_size_);
//			std::copy(begin(q_), end(q_), back_inserter(tmp));
//			q_.swap(tmp);
//		}
//		q_.push_back(ts);
//		if (!(q_.size() % 10)) std::cout << q_.size() << '\n';
//
//	}
//	
//	T&	top()
//	{
//		std::lock_guard<std::mutex> guard(mtx_);
//		return q_.front();
//	}
//
//	T&	back()
//	{
//		std::lock_guard<std::mutex> guard(mtx_);
//		return q_.back();
//	}
//
//	size_t size()
//	{
//		return q_.size();
//	}
//
//	void pop()
//	{
//		std::lock_guard<std::mutex> guard(mtx_);
//		q_.pop_front();
//	}
//
//	bool empty()
//	{
//		return !q_.size();
//	}
//
//	long queue(long lid)
//	{
//		std::lock_guard<std::mutex> guard(mtx_);
//		return 1;
//	}
//	
//	long count(long lid)
//	{
//		std::lock_guard<std::mutex> guard(mtx_);
//		return 2;
//	}
//
//private:
//	size_t buf_size_;
//	boost::circular_buffer<T> q_;
//	std::mutex mtx_;
//};
//

long GenKimNo( long seq, bool oral, int year )
{
	if ( oral ) {
		int code = 2 + year* 3;
		long tmp = seq;
		for ( int i{0}; tmp; ++i, tmp /= 10 ) code += (i % 2 ? 1 : 3) * (tmp % 10);
		return seq * 10 + (code % 10 ? 10 - code % 10 : 0);
	}
	else return seq;
}

long GenKimNo1( long seq, bool oral, int year )
{
	if ( oral ){
		std::pair<long, long> no = std::make_pair( 200000 + year * 10000, seq );
		int veight[13] = {1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1};
		long tmp = no.first;
		int code( 0 );
		for ( int i = 5; tmp && i >= 0; --i, tmp /= 10 ) code += veight[i] * (tmp % 10);
		tmp = no.second;
		for ( int i = 11; tmp && i > 5; --i, tmp /= 10 ) code += veight[i] * (tmp % 10);
		no.second *= 10;
		no.second += code % 10 ? 10 - code % 10 : 0;
		return no.second;
	}
	else return seq;
}

std::string ToEAN13N1( std::pair<long, long> &no )
{

	int veight[13] = {1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1};
	std::string ret;
	std::ostringstream ean;
	ean << boost::format( _T( "%1$3d%2$09d0" ) ) % no.first % no.second;
	ret = ean.str();
	int code( 0 );
	for ( int i = 0; i < 12; ++i ) code += veight[i] * (ret[i] - '0');
	code = code % 10 ? 10 - code % 10 : 0;
	if ( code ) ret[12] = ret[12] + code;

	return ret;
}

std::string ToEAN13N2( std::pair<long, long> &no )
{
	std::string ret;
	std::ostringstream ean;
	ean << boost::format( _T( "%1$3d%2$09d0" ) ) % no.first % no.second;
	ret = ean.str();
	int code( 0 );
	for ( int i = 0; i < 12; ++i ) code += (i % 2 ? 3 : 1) * (ret[i] - '0');
	code = code % 10 ? 10 - code % 10 : 0;
	ret[12] = ret[12] + code;
	return ret;
}

std::string ToEAN13N3( std::pair<long, long> &no )
{
	auto tmp = no;
	int code{0};
	for ( int i{0}; tmp.first; ++i, tmp.first /= 10 ) code += (i % 2 ? 3 : 1) * ( tmp.first % 10);
	for ( int i{0}; tmp.second; ++i, tmp.second /= 10 ) code += (i % 2 ? 1 : 3) * (tmp.second % 10);
	no.second = no.second *10 +(code % 10 ? 10 - code % 10 : 0);
	std::ostringstream ean;
	ean << boost::format( _T( "%1$3d%2$010d" ) ) % no.first % no.second;
	return ean.str();
}

std::string ToEAN13N4( long long &&no )
{
	//0123456789123
	int code{0};
	long long tmp{no};
	for ( int i{1}; tmp; ++i, tmp /= 10 ) code += (i % 2 ? 3 : 1) * (tmp % 10);
	return std::to_string( no * 10 + (code % 10 ? 10 - code % 10 : 0));
}

std::string ToEAN13N5( long long &&no )
{
	//0123456789123
	int code{0};
	long long tmp{no};
	std::string res(13,'0');
	for ( int i{1}, j{11}; tmp; ++i, tmp /= 10,--j ){
		int r = tmp % 10;
		res[j] += r;
		code += (i % 2 ? 3 : 1) * r;
	}
	res[12] += code % 10 ? 10 - code % 10 : 0;
	return res;
}



int main()
{

	//boost::circular_buffer<std::shared_ptr<test_struct>> cb(100);
	//std::shared_ptr<test_struct> p = std::make_shared<test_struct>();
	//std::cout << p->a_ << ' ' << p->b_ << '\n';

	//for (long n = 0; n < 200; ++n) {		
	//	if (!cb.full()) 
	//		cb.push_back(std::make_shared<test_struct>(1, n));
	//	else {
	//		boost::circular_buffer<std::shared_ptr<test_struct>> tmp(cb.capacity() * 2);
	//		//for (auto i : cb) tmp.push_back(i);
	//		std::copy(begin(cb), end(cb), back_inserter(tmp));
	//		cb.swap(tmp);
	//		cb.push_back(std::make_shared<test_struct>(2, n));
	//	}
	//}
	//for (auto i : cb) std::cout << '\t' << i->a_ << ' ' << i->b_ << '\t';
	//std::cout << '\n';
	//std::cout << '\n';
	//const long Z = 100000;
	//for ( int i = 0; i < 10; ++i )
	//	for ( long j : {1, 2, 3, 4} )
	//		for ( long k{0}; k < Z; ++k )
	//			if ( ToEAN13N5( (200 + i * 10 + j) * 1000000000ll + k ) != ToEAN13N1( std::make_pair( 200 + i * 10 + j, std::move( k ) ) ) )
	//				std::cout << ToEAN13N5( (200 + i * 10 + j) * 1000000000ll + k   ) << ' ' << ToEAN13N1( std::make_pair( 200 + i * 10 + j, std::move( k ) ) ) << '\n';
	//{
	//	boost::timer::auto_cpu_timer t;
	//	for ( int i = 0; i < 10; ++i )
	//		for ( long j : {1, 2, 3, 4} )
	//			for ( long k{0}; k < Z; ++k )
	//				ToEAN13N1( std::make_pair( 200 + i * 10 + j, std::move( k ) ) );
	//}
	//{
	//	boost::timer::auto_cpu_timer t;
	//	for ( int i = 0; i < 10; ++i )
	//		for ( long j : {1, 2, 3, 4} )
	//			for ( long k{0}; k < Z; ++k )
	//				ToEAN13N2( std::make_pair( 200 + i * 10 + j, std::move( k ) ) );
	//}
	//{
	//	boost::timer::auto_cpu_timer t;
	//	for ( int i = 0; i < 10; ++i )
	//		for ( long j : {1, 2, 3, 4} )
	//			for ( long k{0}; k < Z; ++k )
	//				ToEAN13N3( std::make_pair( 200 + i * 10 + j, std::move( k ) ) );
	//}
	//{
	//	boost::timer::auto_cpu_timer t;
	//	for ( int i = 0; i < 10; ++i )
	//		for ( long j : {1, 2, 3, 4} )
	//			for ( long k{0}; k < Z; ++k )
	//				ToEAN13N4( (200 + i * 10 + j) * 1000000000 +k  );
	//}
	//{
	//	boost::timer::auto_cpu_timer t;
	//	for ( int i = 0; i < 10; ++i )
	//		for ( long j : {1, 2, 3, 4} )
	//			for ( long k{0}; k < Z; ++k )
	//				ToEAN13N5( (200 + i * 10 + j) * 1000000000 + k );
	//}

	//return 0;
	//for ( int y = 0; y < 10; ++y ){
	//	std::vector<int> hit( 10000000, 0 );
	//	for ( long i = 1; i <= 9999999; ++i )
	//		++hit[GenKimNo( i, true, y ) % 10000000];
	//	for ( long i = 1; i < 10000000; ++i ) if ( hit[i] != 1 ) std::cout << i << '=' << hit[i] << '\t';
	//}
	//return 0;
	boost::circular_buffer<int> tst( 10 );
	tst.push_back( 1 );
	tst.push_back( 2 );
	tst.push_back( 3 );
	tst.push_back( 4 );
	for ( auto &i : tst )
		std::cout << i << ' ';
	std::cout << '\n';
	tst.pop_front();
	tst.push_back( 5 );
	tst.front() = 10;
	for ( auto &i : tst )
		std::cout << i << ' ';
	std::vector<int> tmp{9,8,7};
	std::copy( begin( tmp ), end( tmp ), back_inserter( tst ) );
	std::cout << '\n';
	for ( auto &i : tst )
		std::cout << i << ' ';
	std::cout << '\n';
	return 0;

	ThreadSafeQueue q;
	std::thread::id id = std::this_thread::get_id();
	for (int i = 0; i < 100; ++i) q.push_back(test_struct(-1, i), id);

	using namespace std::chrono_literals;


	std::vector<std::thread> t;
	for (int i = 1; i <= 21; ++i) t.emplace_back([&q](int k) {
		std::thread::id id = std::this_thread::get_id();
		for (int n = 100*k; q.size() && q.size() < 3000;++n) {
			if (k % 2) {
				q.push_back(test_struct(k, q.back().b_+1), id);
				std::this_thread::sleep_for(10ms);
			}
			else {
				q.pop();
				std::this_thread::sleep_for(10ms);
			}
		}
	},i);
	for (auto &f : t) f.join();
	std::cout << '\n' << q.size() << '\n';
	for (; !q.empty(); q.pop() ) std::cout << '\t' << q.top().a_ << ' ' << q.top().b_;
	std::cout << '\n';

	return 0;
}

