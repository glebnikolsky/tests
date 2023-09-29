// SingleInstance.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SingleInstanceApp.h"
#include <iostream>

int main()
{
	SingleApp singleton(_T("TestSingleton"));
	if ( singleton.IsAlredyRunningThisMachine() ){
		std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
	}
	singleton.RemoveAnyway();
    return 0;
}

