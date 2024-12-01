// Pugtest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "pugixml.hpp"


int main()
{
	pugi::xml_document doc;
	auto node = doc.append_child( "root" );
	node = node.append_child( "child" );
	node.append_attribute( "some_attrib" )="some shit";
	node.append_attribute( "some_attrib1" ) = "some shit";
	node.append_attribute( "some_attrib2" ) = "some shit";
	node.append_child( pugi::node_pcdata ).set_value( "node with data and attrib" );
	doc.save( std::cout );

    return 0;
}

