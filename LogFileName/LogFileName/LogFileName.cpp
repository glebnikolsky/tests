// LogFileName.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <boost/log/trivial.hpp>
#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/filesystem.hpp>
#include <string>
#include <iostream>
#include <sstream>


//enum severity_level
//{
//	trace,
//	debug,
//	info,
//	warning,
//	error,
//	fatal
//};

typedef boost::log::sinks::synchronous_sink< boost::log::sinks::text_file_backend > file_sink;
//boost::shared_ptr< file_sink > store_sink;
void Log()
{

	//boost::log::sources::severity_logger<boost::log::trivial::severity_level> lg;
	//BOOST_LOG_SEV(lg, boost::log::trivial::severity_level::trace) << "Some log record 1";
	//BOOST_LOG_SEV(lg, boost::log::trivial::severity_level::debug) << "Some log record 2";
	//BOOST_LOG_SEV(lg, boost::log::trivial::severity_level::info) << "Some log record 3";

	//std::string flnm = (char*)store_sink->locked_backend()->get_current_file_name().c_str();
	//std::cout << flnm << std::endl;
}


int main()
{
	
	boost::shared_ptr< file_sink > sink(new file_sink(
		boost::log::keywords::file_name = "QQ%Y%m%d_%H%M%S.log", //logfilename.c_str(), //boost::filesystem::path("QQ%Y%m%d_%H%M%S.log"), //ѕри каждом запуске новый файл с датой и временем в имени
		boost::log::keywords::rotation_size = 16 * 1024 * 1024,                   // максимальный размер
		boost::log::keywords::format = "[%TimeStamp%]: %Message%"
	));
	//store_sink = sink;
	sink->locked_backend()->set_file_collector(boost::log::sinks::file::make_collector(
		boost::log::keywords::target = ".\\",
		boost::log::keywords::max_size = 64 * 1024 * 1024,                          // максимальный суммарный объЄм
		boost::log::keywords::min_free_space = 100 * 1024 * 1024,                    // минимальное свободное место на диске - не провер€л - что и как он сообщит
		boost::log::keywords::max_files = 100
	));

	sink->locked_backend()->scan_for_files();                                       // посмотрит - не стереть ли чего
	sink->locked_backend()->auto_flush(true);
	boost::log::core::get()->set_filter
	(
		boost::log::trivial::severity >= 2
	);
	boost::log::core::get()->add_sink(sink);
	Log();
	boost::log::sources::severity_logger<boost::log::trivial::severity_level> lg;
	//BOOST_LOG_SEV(lg, boost::log::trivial::severity_level::trace) << "Some log record 1";
	//BOOST_LOG_SEV(lg, boost::log::trivial::severity_level::debug) << "Some log record 2";
	//BOOST_LOG_SEV(lg, boost::log::trivial::severity_level::info) << "Some log record 3";
	//BOOST_LOG_SEV(lg, boost::log::trivial::severity_level::error) << "Some log record 3";
	BOOST_LOG_SEV(lg, boost::log::trivial::severity_level::trace) << "Some log record 1";
	BOOST_LOG_SEV(lg, boost::log::trivial::severity_level::debug) << "Some log record 2";
	BOOST_LOG_SEV(lg, boost::log::trivial::severity_level::info) << "Some log record 3";
	BOOST_LOG_SEV(lg, boost::log::trivial::severity_level::error) << "Some log record 3";
	boost::filesystem::path p = sink->locked_backend()->get_current_file_name();
	std::wstring flnm = sink->locked_backend()->get_current_file_name().c_str();
	std::cout << sink->locked_backend()->get_current_file_name() << std::endl;
	std::wcout << flnm << std::endl;
	std::stringstream st;
	st << sink->locked_backend()->get_current_file_name();
	std::string str = st.str();
	std::cout << ":" << str << ":\n";

    return 0;
}

