// TestC.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

void * foo(void)
{
 //   static int someshit;
    int *a;// = &someshit;
    return a;
};

int _tmain(int argc, _TCHAR* argv[])
{
    int *a = (int*)foo();
    printf("Hello world!");
	return 0;
}

