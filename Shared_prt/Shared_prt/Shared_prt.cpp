// Shared_prt.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <memory>
#include <string>

using namespace std;

struct Tst{
	Tst(const char *t):txt_(t)
		{ cout << "created "<<txt_<<'\n'; }
	~Tst(){ cout << "destroyed" << txt_ << '\n'; }
	void out( const char *txt ){ cout << txt_<<":"<<txt << "\n"; }
	void out( const int i ){ cout << txt_ << ":" << i << "\n"; }
	string txt_;
};

void foo( shared_ptr<Tst>t, const char*txt )
{
	t->out( txt );
}

void bar( shared_ptr<Tst>t, int i  )
{
	t->out( i );
}

void foobar( shared_ptr<Tst>t, const char *txt, int i ){
	foo( t, txt );
	bar( t, i );
}

void tryfuck( shared_ptr<Tst>t )
{
	shared_ptr<Tst>tmp( t );
	tmp->out( "tryfuck" );
}

int main()
{
	shared_ptr<Tst> t = make_shared<Tst>("t" );
	shared_ptr<Tst> tt = make_shared<Tst>( "tt" );
	t = tt;
	foo( t, "foo" );
	bar( t, 1 );
	foobar( t, "foobar", 2 );
	tryfuck( t );
	t.reset();
	cout << (t == nullptr);
	return 0;
}

