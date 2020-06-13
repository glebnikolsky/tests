// TestSmartArray.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//#define __FTCUTIL_CLASS
//#include <vector>
//#include <Wdm.h>
//#include "NonCopyable.h"
//#include "RefCounter.h"
//#include "WeakPtr.h"
//#include "SmartPtr.h"
//#include "SmartArray.h"

#include <string>
#include <iostream>
using namespace std;

#define QQ  1
#define nnn(x) "DB" #x

struct foo{
	int i_;
	string s_;
};

//struct foArray:public _smart_array_t<foo>
//{
//	foo& getByID(unsigned long idx)
//	{
//		return m_array[idx];
//	}
//
//};

int _tmain(int argc, _TCHAR* argv[])
{
    cout<<nnn( 1 )<<endl;

	//foo tst[] = {{1,"q"},{2,"w"}};
	//foArray fa;
	//fa.Add((_smart_ptr_t<foo>)&tst[0]);
	//fa.Add((_smart_ptr_t<foo>)&tst[1]);
	//_smart_ptr_t<foo> ptr = fa.Get(1);
	//int n;
	//n = fa.Find(ptr);
	//cout<< (fa.Get(1) == ptr) <<endl;
		
	return 0;
}

