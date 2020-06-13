// TestRef.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<iostream>

void funcref(int &i)
{
    i++;
}

void funcval(int i)
{
    i++;
}

bool funccmp( int &i, int *j)
{
    return &i == j;
}

int _tmain(int argc, _TCHAR* argv[])
{
    int k(0);
    int &l = k;
    funcref(k);
    std::cout<<k<<std::endl;
    funcref(l);
    std::cout<<k<<std::endl;
    funcval(k);
    funcval(l);
    std::cout<<funccmp(k,&k)<<std::endl;
    std::cout<<funccmp(l,&k)<<std::endl;
    std::cout<<funccmp(k,&l)<<std::endl;
    std::cout<<funccmp(l,&l)<<std::endl;

	return 0;
}

