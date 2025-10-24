// Pugi.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <chrono>
#include <filesystem>
#include <regex>
#include "pugixml.hpp"


using namespace std;
namespace fs = std::filesystem;


int main()
{
    fs::path p("E:/tests/Pugi/Pugi/Pugi.xml");
    if (fs::exists(p)) cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file( p.string().c_str() );
    std::cout << "Load result: " << result.description()  << std::endl;
    if ( !result ) return 1;
    //try {
    //    pugi::xpath_query query("//*[starts-with(name(),'ClComp')]");
    //    pugi::xpath_node_set res = query.evaluate_node_set( doc );
    //    cout << res.size()<<'\n';
    //}
    //catch (const pugi::xpath_exception& e) {
    //    cout << e.what();
    //    return 1;
    //}
    regex r{ "(.*)(1_\\d\\d)(.*)" };
    string s{ "q;boost_1_88;" };
    smatch sm;
    regex_match(s, sm, r);



    try {
        pugi::xpath_query query("//ProjectConfiguration/NewTag");
        pugi::xpath_node_set res = query.evaluate_node_set( doc );
        for (auto& i : res) {
            auto nd = i.node().append_child(pugi::node_pcdata).set_value("Debug");
        }
        doc.print(std::cout);

    }
    catch (const pugi::xpath_exception& e) {
        cout << e.what();
        return 1;
    }
    return 0;
}

