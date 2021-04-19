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

