// TestSharedPointer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "bar.h"
#include "foo.h"
#include <iostream>
#include <vector>

#include <boost/timer/timer.hpp>

int _tmain(int argc, _TCHAR* argv[])
{
    boost::timer::auto_cpu_timer t;
    std::vector<fooBar::foo> f(1000);
    std::vector<fooBar::bar> b(1000);
    for(int i = 1000000; i ; --i){
        for(auto &j:f) j.Do();
        for(auto &j:b) j.Do();
    }
    std::cout<<"foo="<<f[999].myval_<<std::endl;
    std::cout<<"bar="<<b[999].myval_<<std::endl;
	return 0;
}

