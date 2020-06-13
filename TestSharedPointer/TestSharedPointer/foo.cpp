#include "stdafx.h"
#include"SharedPtr.h"
#include "foo.h"


namespace fooBar{

//static _shared_value_t<long> _LastID = 0;

static long _LastID;

foo::foo(void): myval_(0)
{

}

foo::~foo(void)
{
}

long foo::NextID()
{ 
    return (_LastID = _LastID + 1); 
}

void foo::Do()
{ 
    myval_ = NextID();
}

}