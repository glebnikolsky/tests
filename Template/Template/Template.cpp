// Template.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

using namespace std;

template<typename T>class Test{
public:
	Test(T v):val_(v){}
	T val_;
	T Get(){ return val_; }
	void Out();
};

template<typename T>
void Test<T>::Out(){
	cout << val_;
}

template<>
void Test<double>::Out(){
	cout << '*' << val_;
}

int main()
{
	Test<int> a( 1 );
	Test<double> b( 1.0 );
	a.Out();
	b.Out();
    return 0;
}

