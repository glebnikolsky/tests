// TestNCP.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "NonCopyable.h"


struct test : public _non_copyable_t
{
};

void funcVal( test tst)
{
}

void funcRef(test &tst)
{
}


int _tmain(int argc, _TCHAR* argv[])
{
	test t1, t2;

	t1 = t2;// генерит ошибку

//	test t3(t1); генерит ошибку

//	funcVal(t1); генерит ошибку

	funcRef(t1); // работает

	return 0;
}

