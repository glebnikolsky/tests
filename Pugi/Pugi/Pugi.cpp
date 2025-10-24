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
    //if ( !result ) return 1;
    //regex r{ "(.*)(\\\\\\\\((10\\.0\\.18\\.1)|(.*)" };
    //string s = "\\\\DEV-SRV\\lib\ResizableLib\\include;\\\\DEV-SRV\\lib\\Ultimate Grid\Include;\\\\DEV-SRV\\lib\\boost\\include\\boost-1_87;$(IncludePath)";
    //string s{ "q;boost_1_88;" };
    //smatch sm;
    //regex_match(s, sm, r);



    try {
        pugi::xpath_query query("//ProjectConfiguration");
        pugi::xpath_node_set res = query.evaluate_node_set( doc );
        for (auto& i : res) {
            auto nd = i.node();
            if (!nd.child("NewestChild")) {
                auto val = nd.append_child("NewestChild");
                val.append_child(pugi::node_pcdata).set_value("AllDebug");
            }
            nd = i.node();
            if (nd.child("NewTag")) nd.child("NewTag").append_child(pugi::node_pcdata).set_value("AllDebug");
            else nd.append_child("NewTag").append_child(pugi::node_pcdata).set_value("AllDebug");
            doc.print(std::cout);
            cout << endl;
        }
        res = query.evaluate_node_set(doc);
        for (auto& i : res) {
            doc.print(std::cout);
            cout << endl;
            auto nd = i.node();
            if (nd.child("NewTag")) 
                nd.child("NewTag").append_child(pugi::node_pcdata).set_value("Debug");
            doc.print(std::cout);
            cout << endl;
        }
        {
            pugi::xpath_query query("//Platform");
            pugi::xpath_node_set res = query.evaluate_node_set(doc);
            for (auto& i : res) {
                auto nd = i.node().first_child().set_value("x64");
            }
            doc.print(std::cout);

        }
    }
    catch (const pugi::xpath_exception& e) {
        cout << e.what();
        return 1;
    }
    return 0;
}

