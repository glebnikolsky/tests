// vs2019.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <filesystem>
#include <map>
#include <string>
#include "pugixml.hpp"
using namespace std;
namespace fs = std::filesystem;


const char* node_types[] =
{
    "null", "document", "element", "pcdata", "cdata", "comment", "pi", "declaration"
};

// tag::impl[]
struct simple_walker : pugi::xml_tree_walker
{
    simple_walker():path_(""), path_printed_(false),level_(-1){}

    void print_path(pugi::xml_node& node)
    {
        string tmp = node.child_value();
        if (!tmp.empty()) {
            cout << path_ << "\n" << tmp << '\n';
            path_printed_ = true;
        }
    }

    virtual bool for_each(pugi::xml_node& node)
    {
        if (node.type() == pugi::node_element) {
            if (depth() > level_ ) {
                path_ += "/";
                path_ += node.name();
                level_ = depth();
                print_path(node);
            }
            else if (depth() == level_) {
                path_ = path_.substr(0, path_.rfind('/')+1) + node.name();
                print_path(node);
            }
            else if (depth() < level_) {
                for (; level_ >= depth(); --level_) path_ = path_.substr(0, path_.rfind('/'));
                path_ += "/";
                path_ += node.name();
                level_ = depth();
                print_path(node);
            }
            if (node.attributes_begin() != node.attributes_end()) {
                if ( !path_printed_) cout << path_ << '\n';
                for (auto i : node.attributes()) cout << '\t' << i.name() << '=' << i.value() << '\n';
            }
            path_printed_ = false;
        }/*else if (node.type() == pugi::node_pcdata) 
            cout << path_ << "\nValue(" << node.value() << ")\n";*/

        return true; // continue traversal
    }
    string path_;
    bool path_printed_;
    int level_;
    map<string, int> tree_;

};



int main(int argc, char *argv[])
{
 /*   fs::path p("D:\\tests");
    try {
        for (fs::recursive_directory_iterator i(p); i != fs::recursive_directory_iterator(); ++i) {
            if (i->is_regular_file()) {
                    cout << i->path().string() << '\n';
            }
        }
    }
    catch (fs::filesystem_error ex) {
        cout << ex.what();
    }
 */
    if (argc != 3) {
        cout << "Usage file1 file2\n";
    }
    pugi::xml_document doc1, doc2;
    pugi::xml_parse_result result1 = doc1.load_file(argv[1]), 
        result2 = doc2.load_file(argv[2]);
    if (!result1) {
        cout << argv[1] << '\n' << result1.description();
        return 1;
    }
    if (!result2) {
        cout << argv[2] << '\n' << result2.description();
        return 1;
    }

    simple_walker walker;
    doc1.traverse(walker);
    pugi::xpath_node_set tools = doc1.select_nodes("/*[@include]");
    for (auto i : tools) {
        cout << i.node().path() << '\n';
    }


    //pugi::xpath_query query("ClCompile");
    //pugi::xpath_node_set res = query.evaluate_node_set(doc);
    //cout << res.size() << '\n';

    return 0;
}

