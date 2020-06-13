// GLS.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

#include <fstream>
#include <iostream>
#include <map>
#include <string>

#include "pugixml.hpp"

int current_project_=0;

/*!
* \brief
* Откроем документ. Если не указан - создадим новый

* \param doc
* документ
* 
* \param filename
* имя файла с документом, если NULL то будет создан пустой документ
* 
* \returns
* успех или нет
*/

bool OpenDoc(pugi::xml_document &doc, char *filename = NULL)
{
    if ( filename ){
        pugi::xml_parse_result result = doc.load_file(filename);
        if ( !(bool)result )
            std::cout<<result.description(); //AfxMessageBox(result.description());
            current_project_ = (bool)result? 1: 0;
        return (bool)result;
    }else{
        pugi::xml_node parent = doc.append_child("GenLockString");
        current_project_ = 0;
    }
    return true;
}

int CountProjects(pugi::xml_document &doc)
{
    pugi::xml_node parent = doc.first_child();
    int no = 0;
    for (pugi::xml_node_iterator it = parent.begin(); it != parent.end(); ++it){
        std::cout<<it->name()<<"  "<<it->attribute("no").as_int()<<std::endl;
        ++no;
    }
    return no;
}

void  AddResourceH(std::string const &inc_file, std::map<std::string,int> &tbl)
{
    std::ifstream infile(inc_file);
    std::string line;
    boost::regex e("^\\#define\\s+([A-Za-z_0-9]+)\\s+(\\d+)\\s*");
    while(std::getline(infile,line)){
        boost::smatch what;
        if ( boost::regex_match(line, what, e, boost::match_perl) )
            tbl[what[1].str()] = atoi( what[2].str().c_str());
    }
}


pugi::xml_node FindOrCreate(pugi::xml_node &node, _TCHAR* name, _TCHAR *attrib_name, const _TCHAR *attrib_val)
{
    pugi::xml_node tmp = node.first_child();
    for( pugi::xml_node_iterator it = tmp.begin(); it != tmp.end(); ++it){
        if ( !strcmp(it->name(), name) 
             && !strcmp(it->attribute(attrib_name).value(), attrib_val) )
            return *it;                                                                     //Нашли
    }
// Не найден. Создаём
    tmp = node.append_child(name);
    tmp.append_attribute(attrib_name) = attrib_val;
    return tmp;
}



void AddMenu(pugi::xml_node &cproj, std::string const &res_file)
{
    std::ifstream infile(res_file);
    std::string line;
    boost::regex menu("([A-Za-z_0-9]+)\\s+MENU");
    boost::regex menuitem("MENUITEM\\s+\"(.*)\",\\s+([A-Za-z_0-9]+)");
    boost::regex popup("POPUP\\s+\"(.*)\"");

    int begin_end = 0;
    bool in_menu = false;
    pugi::xml_node menu_node;
    std::string pfx;
    while(std::getline(infile, line)){
        boost::smatch what;
        boost::trim( line);
        if (!in_menu && boost::regex_match(line, what, menu, boost::match_perl) ){
            in_menu = true;
            menu_node = FindOrCreate(cproj, "Menu", "id", what[1].str().c_str());
            continue;
        }
        if(!in_menu ) continue;
        if(line == "BEGIN" || line == "END"){
            begin_end += line == "BEGIN" ? 1 : -1;
            in_menu = begin_end != 0;
            continue;
        }
        if( boost::regex_match(line, what, popup, boost::match_perl)){
            pfx = what[1].str() + "\\";
            continue;
        }
        if( boost::regex_match(line, what, menuitem, boost::match_perl)){
            std::string text = what[1].str(), id = what[2].str();
            text = pfx + text;
            pugi::xml_node tmp =FindOrCreate(menu_node, "MenuItem", "id", id.c_str());
            tmp.append_attribute("text") = text.c_str();
            continue;
        }
    }

}

void AddDialog(pugi::xml_node &cproj, std::string const &res_file)
{
    std::ifstream infile(res_file);
    std::string line;
    boost::regex dialog("([A-Za-z_0-9]+)\\s+(DIALOG|DIALOGEX)\\s+\\d+,\\s+\\d+,\\s+\\d+,\\s+\\d+");
    boost::regex edit("EDITTEXT\\s+([A-Za-z][A-Za-z_0-9]+),.*");
    boost::regex btn("(PUSHBUTTON|DEFPUSHBUTTON)\\s+\"(.*)\",([A-Za-z_0-9]+),.*");
    boost::regex combo("(COMBOBOX|LISTBOX)\\s+([A-Za-z_0-9]+),.*");
    boost::regex control("CONTROL\\s+\"(.*)\",([A-Za-z][A-Za-z_0-9]+),.*");
    boost::regex caption("CAPTION\\s+\"(.*)\"");

    bool in_dialog = false;
    pugi::xml_node dialog_node;
    while(std::getline(infile, line)){
        boost::smatch what;
        boost::trim( line);
        if (! in_dialog && boost::regex_match(line, what, dialog, boost::match_perl) ){
            in_dialog = true;
            dialog_node = FindOrCreate(cproj, "Dialog", "id", what[1].str().c_str());
            continue;
        }
        if(!in_dialog ) continue;
        if(line == "END"){
            in_dialog = false;            
            continue;
        }
        if( boost::regex_match(line, what, caption, boost::match_perl)){
            dialog_node.append_attribute("caption") = what[1].str().c_str();
            continue;
        }
        if( boost::regex_match(line, what, btn, boost::match_perl)){
            pugi::xml_node tmp =FindOrCreate(dialog_node, "Control", "id", what[3].str().c_str());
            tmp.append_attribute("type") = "button";
            tmp.append_attribute("caption") = what[2].str().c_str();
            continue;
        }
        if( boost::regex_match(line, what, edit, boost::match_perl) ){
            pugi::xml_node tmp =FindOrCreate(dialog_node, "Control", "id", what[1].str().c_str());
            tmp.append_attribute("type") = "edit";
            continue;
        }
        if( boost::regex_match(line, what, combo, boost::match_perl) ){
            pugi::xml_node tmp =FindOrCreate(dialog_node, "Control", "id", what[2].str().c_str());
            tmp.append_attribute("type") = "combo|list";
            continue;
        }
        if( boost::regex_match(line, what, control, boost::match_perl) ){
            pugi::xml_node tmp =FindOrCreate(dialog_node, "Control", "id", what[2].str().c_str());
            tmp.append_attribute("type") = "control";
            std::string capt = what[1].str();
            if ( !capt.empty())
                tmp.append_attribute("caption") = capt.c_str();

            continue;
        }

    }

}

void DropChilds(pugi::xml_node &cproj,_TCHAR* name)
{
    int has_child = true;
    while( has_child)
        has_child = cproj.remove_child(name);
}


int CreateProject(pugi::xml_document &doc, const char *path)
{

    std::string res_file;
    std::string inc_file;
    std::string proj;
    int found = 0;
    for ( boost::filesystem::recursive_directory_iterator itr(path); itr != boost::filesystem::recursive_directory_iterator(); ){
        if ( is_regular_file(itr->status()) ){
            std::string file_name = itr->path().filename().string();
            boost::to_lower(file_name);
            if (boost::algorithm::ends_with(file_name, _T(".rc"))) {
                res_file = itr->path().string();
                proj = itr->path().filename().string();
                proj = proj.substr(0,proj.length()-3);
                found++;
            }
            if ( file_name == "resource.h" ){
                inc_file = itr->path().string();
                ++found;
            }
            if ( found == 2)
                break;
        }
        try{
            ++itr;
        }catch (const std::exception & ex ){// Просто пропустим системные и т.д. файлы, на которые у нас нет прав
            (void) ex;
            itr.no_push();
            ++itr;
        }
    }
    if ( found != 2 ) {
        //AfxMessageBox("Не найден Resource.h или <ProjectName>.rc");
        return 0;
    }
    pugi::xml_node cproj = doc.first_child().append_child("LockStringProject");
    int no = CountProjects(doc);
    cproj.append_attribute("no") = no;
    current_project_ = no;
    pugi::xml_node body = cproj.append_child("VCProject");
    body.append_attribute("path") = path;
    body.append_attribute("include") = inc_file.c_str();
    body.append_attribute("res") = res_file.c_str();
    body.append_attribute("proj") = proj.c_str();
    body = cproj.append_child("ProjecInfo");
    body.append_attribute("prefix") = "";
    body.append_attribute("lock_string") = "";
    std::map<std::string,int> tbl;
    DropChilds(cproj,"Menu");
    AddResourceH(inc_file, tbl);
    AddMenu(cproj, res_file);
    DropChilds(cproj,"Dialog");
    AddDialog(cproj, res_file);

    return no;
}


int _tmain(int argc, _TCHAR* argv[])
{
    pugi::xml_document doc;
    bool b = OpenDoc(doc,"save_file_output.xml");
    if ( !b )
        b = OpenDoc(doc);
    int no = CountProjects(doc);
    no = CreateProject(doc,"D:\\use_2014_1\\Projects\\DBManager" );
    no = CreateProject(doc,"D:\\use_2014_1\\Projects\\CreateDatabase" );

    doc.save_file("save_file_output.xml");


    /*
    pugi::xml_node parent = doc.first_child();
    if ( strcmp(parent.name(),"GenLocString") )
        parent = doc.append_child("GenLocString");
    int no = 0;
    for (pugi::xml_node_iterator it = parent.begin(); it != parent.end(); ++it){

        std::cout<<it->name()<<"  "<<it->attribute("no").as_int()<<endl;
        ++no;
    }
    pugi::xml_node cproj = parent.append_child("LockStringProject");
    cproj.append_attribute("no") = no+1;
    pugi::xml_node node = cproj.append_child("VSproject");
    node.append_attribute("file") = "dfsadfsadfsadgfdsfgsd\\dfg sdfgsdfgsdfgsdfg\\dsfgsdfgsdfgsdfg";
    node.append_attribute("name") = "dsfgsdfgsdfgsdfg";
    node.append_attribute("prefix") = "FTC.";
    pugi::xml_node menu = cproj.append_child("menu");
    menu.append_attribute("IDR") = "sadkfjaslkdjflksadjfklsad'";
    node = menu.append_child("menuItem");
    node.append_attribute("name") = "adadasdadad";
    node.append_attribute("id") = 12;
    node.append_attribute("caption") = "sdfsadgf sdfgsdgsdfgsdfgsdfg dsfg dsfg";
    node = menu.append_child("menuItem");
    node.append_attribute("name") = "qqqqqqqqqqqqqq";
    node.append_attribute("id") = 13;
    node.append_attribute("caption") = "dsglsd; sdg dfg ";
    pugi::xml_node dialog = cproj.append_child("dialog");
    dialog.append_attribute("name") = "sfsa fsadfsa dgfsadgfsadg";
    dialog.append_attribute("IDD") = 30;
    node = dialog.append_child("control");
    node.append_attribute("ID") = 12;
    node.append_attribute("name") = "sad asFD SAFASDF ASF";
    node.append_attribute("Caption") = "sd afsfgsdgf sdgsdg";
    pugi::xml_document doc;
    doc.load_file("save_file_output.xml");

    pugi::xml_node parent = doc.first_child();
    if ( strcmp(parent.name(),"GenLocString") )
        parent = doc.append_child("GenLocString");
    int no = 0;
    for (pugi::xml_node_iterator it = parent.begin(); it != parent.end(); ++it){

        std::cout<<it->name()<<"  "<<it->attribute("no").as_int()<<endl;
        ++no;
    }
    pugi::xml_node cproj = parent.append_child("LockStringProject");
    cproj.append_attribute("no") = no+1;
    pugi::xml_node node = cproj.append_child("VSproject");
    node.append_attribute("file") = "dfsadfsadfsadgfdsfgsd\\dfg sdfgsdfgsdfgsdfg\\dsfgsdfgsdfgsdfg";
    node.append_attribute("name") = "dsfgsdfgsdfgsdfg";
    node.append_attribute("prefix") = "FTC.";
    pugi::xml_node menu = cproj.append_child("menu");
    menu.append_attribute("IDR") = "sadkfjaslkdjflksadjfklsad'";
    node = menu.append_child("menuItem");
    node.append_attribute("name") = "adadasdadad";
    node.append_attribute("id") = 12;
    node.append_attribute("caption") = "sdfsadgf sdfgsdgsdfgsdfgsdfg dsfg dsfg";
    node = menu.append_child("menuItem");
    node.append_attribute("name") = "qqqqqqqqqqqqqq";
    node.append_attribute("id") = 13;
    node.append_attribute("caption") = "dsglsd; sdg dfg ";
    pugi::xml_node dialog = cproj.append_child("dialog");
    dialog.append_attribute("name") = "sfsa fsadfsa dgfsadgfsadg";
    dialog.append_attribute("IDD") = 30;
    node = dialog.append_child("control");
    node.append_attribute("ID") = 12;
    node.append_attribute("name") = "sad asFD SAFASDF ASF";
    node.append_attribute("Caption") = "sd afsfgsdgf sdgsdg";*/

	return 0;
}

