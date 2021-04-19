#include "stdafx.h"

#include <chrono>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>


#include "ThreadSafeQueue.h"

#include <boost/circular_buffer.hpp>

using namespace std::chrono_literals;

class ThreadSafeQueue::Impl
{
public:
	Impl(size_t bs = 100): 
		buf_size_(bs), q_(boost::circular_buffer<test_struct>(buf_size_))
	{
	}

	~Impl() = default;

	void push_back(const test_struct& ts, std::thread::id id)
	{
		std::lock_guard<std::mutex> guard(mtx_);
		if (q_.full()) {
			boost::circular_buffer<test_struct> tmp(q_.capacity() + buf_size_);
			std::copy(begin(q_), end(q_), back_inserter(tmp));
			q_.swap(tmp);
			std::cout << id << " Resize to " << q_.capacity() << '\n';
		}
		q_.push_back(ts);
		//if (!(q_.size() % 10)) std::cout << q_.size() << '\n';

	}

	test_struct&	top()
	{
		std::lock_guard<std::mutex> guard(mtx_);
		return q_.front();
	}

	test_struct&	back()
	{
		std::lock_guard<std::mutex> guard(mtx_);
		return q_.back();
	}

	size_t size()
	{
		return q_.size();
	}

	void pop()
	{
		std::lock_guard<std::mutex> guard(mtx_);
		q_.pop_front();
	}

	bool empty()
	{
		return !q_.size();
	}

	long queue(long lid)
	{
		std::lock_guard<std::mutex> guard(mtx_);
		return 1;
	}

	long count(long lid)
	{
		std::lock_guard<std::mutex> guard(mtx_);
		return 2;
	}

private:
	size_t buf_size_;
	boost::circular_buffer<test_struct> q_;
	std::mutex mtx_;
};

ThreadSafeQueue::ThreadSafeQueue() 
{
	pImpl = std::make_unique<Impl>();
}

ThreadSafeQueue::~ThreadSafeQueue() = default;

void ThreadSafeQueue::push_back(const test_struct& ts, std::thread::id id) {
	pImpl->push_back(ts,id);
}

test_struct& ThreadSafeQueue::top()
{
	return pImpl->top();
}

test_struct& ThreadSafeQueue::back()
{
	return pImpl->back();
}

size_t ThreadSafeQueue::size()
{
	return pImpl->size();
}

void ThreadSafeQueue::pop()
{
	pImpl->pop();
}

bool ThreadSafeQueue::empty()
{
	return !size();
}

long ThreadSafeQueue::queue(long lid)
{
	return pImpl->queue(lid);
}

long ThreadSafeQueue::count(long lid)
{
	return pImpl->count(lid);
}
