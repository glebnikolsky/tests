// Guidtest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <string>
#include <sstream>
#include <boost/uuid/uuid.hpp>
#include "guid.h"




int main()
{
	setlocale(LC_ALL, "ru-RU");
	Guid g("626A2159-57BA-4CA7-9CDB-276B796D620F");
	std::cout << g << '\t' << (g == "626A2159-57BA-4CA7-9CDB-276B796D620F") << '\t';
	std::istringstream is("626A2159-57BA-4CA7-9CDB-276B796D620F");
	Guid gg;
	std::cout << gg << '\t';
	is >> gg;
	std::cout << gg << '\t' << (gg == g);


    return 0;
}

