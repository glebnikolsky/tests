// Property_tree.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <set>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>



struct queries{

    bool load(char *filename, std::string &err)
    {
        boost::property_tree::read_xml(filename, tree_);
        boost::property_tree::ptree stree = tree_.get_child("queries");
        auto it = stree.begin(); 
        if ( it == stree.end() ){
            err = "Неправильный формат";
            return false;
        }
        std::set<std::string> tmp;
        for(; it != stree.end(); ++it){
            auto fnd = tmp.find(it->second.get<std::string>("aliase"));
            if ( fnd != tmp.end()){
                err = "Дублируется aliase";
                return false;
            }
            tmp.insert(it->second.get<std::string>("aliase"));
        }
        return true;
    }

    size_t size()
    { 
        return tree_.get_child("queries").size();
    }

    bool getName(int n, std::string &name)
    {
        boost::property_tree::ptree stree = tree_.get_child("queries");
        auto it = stree.begin(); 
        int i = 0;
        for( ; i != n && it != stree.end(); ++it, ++i);
        if ( i != n) return false;
        name = it->second.get<std::string>("name");
        return true;
    }

    bool getData( const char *name, std::string &aliase, std::string &body, int &cols, std::string &hdr)
    {
        boost::property_tree::ptree stree = tree_.get_child("queries");
        try{
            for( auto it = stree.begin(); it != stree.end(); ++it){
                if ( it->second.get<std::string>("name") == name){
                    aliase = it->second.get<std::string>("aliase");
                    body = it->second.get<std::string>("body");
                    cols = it->second.get<int>("columns");
                    hdr = it->second.get<std::string>("headers");
                    return true;
                }
            }
        }catch(std::exception &e){
            std::cout<<e.what()<<std::endl;
//            AfxMessageBox(e.what());
        }
        return false;
    }

private:
    boost::property_tree::ptree tree_;
};

using namespace boost::property_tree;


int _tmain(int argc, _TCHAR* argv[])
{
    queries tst;
    std::string err;
    if ( !tst.load("D:\\Work\\tests\\Property_tree\\Property_tree\\Queries.xml",err))
    {
        std::cout<<err<<std::endl;
        return 1;
    }
    int n = tst.size();
    int c;
    std::string nm,al,bd, hd;
    for( int i =0; i < n; ++i){
        tst.getName(i,nm);
        tst.getData(nm.c_str(), al, bd, c, hd);
        std::cout<<al<<'\t'<<nm<<std::endl<<bd<<'\t'<<c<<'\t'<<hd<<std::endl;
    }

	return 0;
}

