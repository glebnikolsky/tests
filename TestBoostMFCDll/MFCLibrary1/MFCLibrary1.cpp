// MFCLibrary1.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <boost\thread\win32\mfc_thread_init.hpp>

#include "TestDll.h"
#include <boost/log/trivial.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


void TestDllExpImp test1()
{
    boost::log::sources::severity_logger< boost::log::trivial::severity_level > lg;
    BOOST_LOG_SEV(lg, boost::log::trivial::trace) << __FILE__<<"test1()  before"<<__FUNCTION__<<" "<<__LINE__;
    for(int i = 0; i < 200; ++i)
        BOOST_LOG_SEV(lg, boost::log::trivial::trace) << __FILE__<<"test1()  into "<<i;
    BOOST_LOG_SEV(lg, boost::log::trivial::trace) << __FILE__<<"test1()  after "<<__FUNCTION__<<" "<<__LINE__;
}

void TestDllExpImp test2()
{
    boost::log::sources::severity_logger< boost::log::trivial::severity_level > lg;
    BOOST_LOG_SEV(lg, boost::log::trivial::trace) << __FILE__<<"test2()  before"<<__FUNCTION__<<" "<<__LINE__;
    for(int i = 0; i < 100; ++i)
        BOOST_LOG_SEV(lg, boost::log::trivial::trace) << __FILE__<<"test2()  into "<<i;
    BOOST_LOG_SEV(lg, boost::log::trivial::trace) << __FILE__<<"test2()  after "<<__FUNCTION__<<" "<<__LINE__;
}
