// TestNCP.cpp: ���������� ����� ����� ��� ����������� ����������.
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

	t1 = t2;// ������� ������

//	test t3(t1); ������� ������

//	funcVal(t1); ������� ������

	funcRef(t1); // ��������

	return 0;
}

