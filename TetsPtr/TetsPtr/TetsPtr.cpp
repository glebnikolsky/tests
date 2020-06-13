// TetsPtr.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <windows.h>
#include "NonCopyable.h"
#include "RefCounter.h"
#include "WeakPtr.h"
#include "SmartPtr.h"
#include "SmartArray.h"

#include <boost/timer/timer.hpp>
#include <boost/shared_ptr.hpp>

struct foo{
    foo():a(0), b(0){ c[0] = L'\0';}

    int a;
    long b;
    _TCHAR c[1];
};




const size_t Size = 1000;
const long   Cnt = 1000;


int _tmain(int argc, _TCHAR* argv[])
{
    {
        boost::timer::auto_cpu_timer t;
        for(int i = 0; i < Cnt; i++){
            std::vector<_smart_ptr_t<foo> > v(Size),v1(Size);
            for(int j=0; j < Size; ++j){
                v[j] = _smart_ptr_t<foo>(new foo());
                v1[j] = v[j];
            }

        }

    }
    {
        boost::timer::auto_cpu_timer t;
        for(int i = 0; i < Cnt; i++){
            std::vector<boost::shared_ptr<foo> > v(Size),v1(Size);
            for(int j=0; j < Size; ++j){
                v[j] = boost::shared_ptr<foo>(new foo());
                v1[j] = v[j];
            }

        }

    }


	return 0;
}

