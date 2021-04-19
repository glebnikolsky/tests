#pragma once


struct test_struct {
	test_struct(long a = 0, long b = 0) :a_(a), b_(b)
	{
	}

	long a_;
	long b_;
};


class ThreadSafeQueue {
public:
	ThreadSafeQueue();

	~ThreadSafeQueue();

	void push_back(const test_struct& ts, std::thread::id id);

	test_struct& top();
	
	test_struct& back();

	size_t size();

	void pop();

	bool empty();

	long queue(long lid);

	long count(long lid);

private:
	class Impl;
	std::unique_ptr<Impl> pImpl;
};
