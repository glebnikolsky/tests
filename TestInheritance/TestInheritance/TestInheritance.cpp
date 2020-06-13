// TestInheritance.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <string.h>
#include <string>
#include <comdef.h>
#include <io.h>
#include <sys/types.h>
#include <SYS\Stat.h>
#include <fcntl.h>
#include <boost/locale.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <vector>

using namespace std;

struct Dog{
	
	Dog():i_(rand()){}
	int i_;
	virtual void Bark(int *)
	{
		cout<<"bark-bark"<<endl;
	}

};


struct Bulldog:public Dog{
	void Bark(int *)
	{
		cout<<i_<<" bark-bark-bark-bark"<<endl;
	}
	void Bark(int )
	{
		cout<<i_<<" bark-bark-bark-bark!"<<endl;
	}
	void Bark(int,int )
	{
		cout<<i_<<" wow-wow!"<<endl;
	}

};

typedef std::vector<Bulldog> vDog;


int _tmain(int argc, _TCHAR* argv[])
{
	Bulldog buks;
	int i=0;

	buks.Bark(1);
	buks.Bark(&i);
	buks.Bark(1,2);

	//vector<int> v;
	//for(int i =0; i < 10; ++i)
	//	v.push_back(i);
	//for(auto iv:v) cout<<iv<<' ';
	//cout<<endl;
	//map<int, double> m;
	//for(int i = 10; i > 0; --i )
	//	m[i] = i-10.0;
	//for(auto mv:m) cout<<mv.second<<' ';
	//cout<<endl;
	setlocale(LC_ALL, "ru-RU");
	std::locale loc;
	//_bstr_t q123;
	//q123 = L"wedfk gawkfj גפףûצארןנפûא";
	//string tmpstr;
	//tmpstr = q123;
	//cout<<tmpstr<<endl;
	//BSTR Bstr = ::SysAllocString(L"123Çהנאגסעגףי, לטנ"); 
	//unsigned char **tmp = (unsigned char**)&Bstr;
	//for(int i=0; i < 50; ++i)
	//	printf(" %02x %c", (*tmp)[i], (*tmp)[i]);
	//cout<<endl;
	//char *ch = "Çהנאגסעגףי, לטנ";
	//_bstr_t bstr_t(Bstr), bstr_a(ch);
	//cout<<(bstr_t == bstr_a)<<endl;
	//char * p = (char*)bstr_t;
	//string str(bstr_t), str1(p);
	//cout<<strlen(p)<<'|'<<str<<'|'<<str1<<endl;
	//::SysFreeString(Bstr);
	//
	//cout<<endl<<str<<endl;

	//long testLong = 2147483647;

	//cout<<testLong<<endl;
	//printf("%d %ld\n",testLong, testLong);
	//CString tempstr;
	//tempstr.Format("%d %ld \n",testLong, testLong);

	int fl;
	int ret = _sopen_s(&fl,	_T("D:\\work\\sqlquery12.sql"), _O_BINARY|_O_RDONLY, _SH_DENYRD, _S_IREAD);
	if ( fl != -1){
		long flen = _lseek( fl, 0, SEEK_END);
		char *buf = new char[flen+2];
		short int unicodeSign[] = {0xfffe, 0xfeff};
		_lseek( fl, 0, SEEK_SET);
		_read(fl, (void*)buf, flen);
		buf[flen] = buf[flen+1] = 0;
		if ( *((short *)buf) == unicodeSign[0] || *((short *)buf) == unicodeSign[1] ){
			//_bstr_t bttmp((const wchar_t*)buf);
			//strcpy_s(buf, flen, (char*)bttmp);
			{
				int fo;
				ret = _sopen_s(&fo,	_T("D:\\work\\sqlquery12.sql.asc"), _O_BINARY|_O_CREAT|_O_WRONLY|_O_TRUNC, _SH_DENYRW, _S_IWRITE);
				_write(fo,buf+1, strlen(buf)-1);
				_close(fo);
			}
		}
		delete[] buf;
		_close(fl);
		CFile cfl(_T("D:\\work\\sqlquery12.sql"), CFile::modeRead);
		long ln = cfl.GetLength();
		char *buffer = new char[ln+2];
		cfl.Read(buffer, ln);
		buffer[ln] = buffer[ln+1] = '\0';
		wchar_t *w = (wchar_t *)buffer;
		++w;
		_bstr_t tmp(w);
		strcpy_s(buffer, ln, (char*)tmp);
		CString qq((char*)tmp);
		{
			int fo;
			ret = _sopen_s(&fo,	_T("D:\\work\\sqlquery12.sql.basc"), _O_BINARY|_O_CREAT|_O_WRONLY|_O_TRUNC, _SH_DENYRW, _S_IWRITE);
			_write(fo,buffer, strlen(buffer));
			_close(fo);
		}


		vDog v(10);
		int nAmnt=0;
		cout<<endl<<endl;
		for(auto &i: v)
		{
			i.Bark(1);
			nAmnt += i.i_;
		}
		int nCountO(0), nCountE(0);
		cout<<time(NULL)<<endl;
		srand(time(NULL));

		cout<<count_if(v.begin(), v.end(), [&nCountO,&nCountE](const Bulldog& t){
					nCountO += t.i_%2;
					nCountE += t.i_%2 ? 0 : 1;
					return !(t.i_%2);
				})<<endl;
		cout<<nCountE<<' '<<nCountO;


	}



	return 0;
}

