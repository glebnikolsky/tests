// OutOfMem.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <string>
#include <iostream>
#include <list>


int _tmain(int argc, _TCHAR* argv[])
{
    std::string bar=_T("1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890");
    std::string foo;
    std::list<std::string> l;
    long  len = bar.length();
    while( !bar.empty()){
        try{
        l.push_back(bar);
        }catch(std::exception &ex){
            std::cout<<len<<std::endl;
        }
        len += bar.length();
    }

	return 0;
}

