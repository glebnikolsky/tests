#include "stdafx.h"
#include "bar.h"
#include"SharedPtr.h"

namespace fooBar{

//static _shared_value_t<long> _LastID = 0;

static long _LastID;


bar::bar(void): myval_(0)
{

}

bar::~bar(void)
{
}

long bar::NextID()
{ 
    return (_LastID = _LastID + 1); 
}

void bar::Do()
{ 
    myval_ = NextID();
}

}