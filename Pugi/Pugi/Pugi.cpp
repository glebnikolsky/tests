// Pugi.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <chrono>
#include <filesystem>
#include "pugixml.hpp"


using namespace std;
namespace fs = std::filesystem;


int main()
{
    fs::path p("D:\\Projects\\ACDStation\\ACDStation.vcxproj");
    if (fs::exists(p)) cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file( "D:\\Projects\\ACDStation\\ACDStation.vcxproj " );
    std::cout << "Load result: " << result.description()  << std::endl;
    if ( !result ) return 1;
    try {
        pugi::xpath_query query("//*[starts-with(name(),'ClComp')]");
        pugi::xpath_node_set res = query.evaluate_node_set( doc );
        cout << res.size()<<'\n';
    }
    catch (const pugi::xpath_exception& e) {
        cout << e.what();
        return 1;
    }
    return 0;
}

