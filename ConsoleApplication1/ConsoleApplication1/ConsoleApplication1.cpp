// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
using namespace std;

struct A {

	void b(bool c) {
		cout << "A::b(" << c << ")\n";
	}
};
struct B {
	B(void (*ref)(bool)):fref_(ref){}
	void (*fref_)(bool);
};

int main()
{
	A a;
	B b(&A::b);
	b.fref_(true);
	b.fref_(false);
	setlocale(LC_ALL, "ru-RU");

    return 0;
}

