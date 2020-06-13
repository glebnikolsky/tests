// IndianCode.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <iostream>
#include <vector>
#include <boost/thread/thread_only.hpp>
#include <boost/thread/xtime.hpp>

volatile long I;
volatile long Cnt;

const long ThreadCycles = 1000;
const long Threads = 5;

struct Add_
{
    void operator()()
    {
        InterlockedIncrement(&Cnt);
        for(int i = 0; i < ThreadCycles; ++i){
            InterlockedIncrement(&I);
            Sleep(50);
        }
        InterlockedDecrement(&Cnt);
    }

};

struct Sub_
{
    void operator()()
    {
        InterlockedIncrement(&Cnt);
        for(int i = 0; i < ThreadCycles; ++i){
            InterlockedDecrement(&I);
            Sleep(50);
        }
        InterlockedDecrement(&Cnt);
    }

};



int _tmain(int argc, _TCHAR* argv[])
{
    long if_ = 0, elif_ = 0, else_ = 0;
    std::vector<Add_> a(Threads);
    std::vector<Sub_> b(Threads);

    for( int i = 0; i < Threads; ++i){
        boost::thread tb(b[i]);
        boost::thread ta(a[i]);
    }

    while( Cnt ){
        if ( I != 0) ++if_ ;
        else if ( I == 0 ) ++elif_;
        else ++else_;
    }
    
    std::cout<<if_<<'\t'<<elif_<<'\t'<<else_<<std::endl;


	return 0;
}

