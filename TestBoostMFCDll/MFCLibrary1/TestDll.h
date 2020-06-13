#pragma once    

#ifdef _AFXDLL
#define TestDllExpImp _declspec(dllexport)
#else
#define TestDllExpImp _declspec(dllimport)
#endif

void TestDllExpImp test1();

void TestDllExpImp test2();
