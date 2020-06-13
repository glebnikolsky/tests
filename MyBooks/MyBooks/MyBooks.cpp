// MyBooks.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <map>
#include <string>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>


namespace fs = boost::filesystem;
using fs::path;


int _tmain(int argc, _TCHAR* argv[])
{
    for ( fs::recursive_directory_iterator i( "D:\\BackUp\\Annfb2" ); i != fs::recursive_directory_iterator(); ){
        if ( is_regular_file(i->status()) ){
            std::string fl = i->path().string();
            boost::regex e5("(.*)( - royallib\\.ru)(\\..*)");
            boost::regex e4("(.*)( - royallib\\.com)(\\..*)");
            boost::regex e3("(.*)( \\(\\d+\\))(\\..*)");
            boost::regex e2("(.*)(\\.fb2)(\\..*)");
            boost::regex e1("(.*)(\\.\\d+)(\\..*)");
            boost::regex e0("(.*)(\\.\\d+\\.fb2)(\\..*)");
            boost::smatch what;
            if (   boost::regex_match(fl, what, e0, boost::match_perl) 
                || boost::regex_match(fl, what, e1, boost::match_perl)
                || boost::regex_match(fl, what, e2, boost::match_perl)
                || boost::regex_match(fl, what, e3, boost::match_perl) 
                || boost::regex_match(fl, what, e4, boost::match_perl) 
                || boost::regex_match(fl, what, e5, boost::match_perl) ){
                fl = what[1] + what[3];
                if ( fs::exists(fl) ) fs::remove(i->path());
                else fs::rename(i->path(), fl);
            }
        }
        try{
            ++i;
        }catch (const std::exception & ex ){// Просто пропустим системные и т.д. файлы, на которые у нас нет прав
            (void) ex;
            ++i;
        }
    }
    //for ( fs::recursive_directory_iterator i( "D:\\BackUp\\Annfb2" ); i != fs::recursive_directory_iterator(); ){
    //    std::string spath = i->path().string();
    //    if ( is_regular_file(i->status()) ){
    //        boost::regex e("^([A-Za-z '-]+)_.*epub$");
    //        boost::regex e1("^([А-Яа-яёЁ]+)_.*epub$");
    //        boost::regex e2("^([А-Яа-яёЁ ]+)\\..*epub$");
    //        boost::regex e3("^([A-Za-z ']+) - .*epub$");
    //        boost::regex e4("^([A-Za-z ']+),.*epub$");
    //        boost::regex e5("^([A-Za-z ']+)_-.*epub$");
    //        boost::regex e6("^([A-Za-z '-]+)\\. -.*epub$");
    //        boost::regex e7("^([A-Za-z. '-]+) -.*epub$");
    //        boost::smatch what;
    //        std::string sfile = i->path().filename().string();
    //        if ( boost::regex_match(sfile, what, e, boost::match_perl) 
    //            ||boost::regex_match(sfile, what, e1, boost::match_perl)
    //            ||boost::regex_match(sfile, what, e2, boost::match_perl) 
    //            ||boost::regex_match(sfile, what, e3, boost::match_perl) 
    //            ||boost::regex_match(sfile, what, e4, boost::match_perl) 
    //            ||boost::regex_match(sfile, what, e5, boost::match_perl) 
    //            ||boost::regex_match(sfile, what, e6, boost::match_perl) 
    //            ||boost::regex_match(sfile, what, e7, boost::match_perl) ){
    //            path p(i->path().parent_path().string() + "\\" + what[1]);
    //            if ( !fs::exists(p)) fs::create_directory(p);
    //            p = path(i->path().parent_path().string() + "\\" + what[1] + "\\" + i->path().filename().string());
    //            if ( !fs::exists(p)) fs::rename(i->path(),p);
    //            else fs::remove(i->path());
    //        }
    //        std::string ofile = i->path().filename().string(),nfile;
    //        nfile = ofile;


    //    }
    //    try{
    //        ++i;
    //    }catch (const std::exception & ex ){// Просто пропустим системные и т.д. файлы, на которые у нас нет прав
    //        (void) ex;
    //        ++i;
    //    }
    //}
    std::map<_TCHAR, std::string> encode;
    encode['А'] = "A";encode['а'] = "a";
    encode['Б'] = "B";encode['б'] = "b";
    encode['В'] = "V";encode['в'] = "v";
    encode['Г'] = "G";encode['г'] = "g";
    encode['Д'] = "D";encode['д'] = "d";
    encode['Е'] = "E";encode['е'] = "e";
    encode['Ё'] = "Jo";encode['ё'] = "jo";
    encode['Ж'] = "Zh";encode['ж'] = "zh";
    encode['З'] = "Z";encode['з'] = "z";
    encode['И'] = "I";encode['и'] = "i";
    encode['Й'] = "Yy";encode['й'] = "yy";
    encode['К'] = "K";encode['к'] = "k";
    encode['Л'] = "L";encode['л'] = "l";
    encode['М'] = "M";encode['м'] = "m";
    encode['Н'] = "N";encode['н'] = "n";
    encode['О'] = "O";encode['о'] = "o";
    encode['П'] = "P";encode['п'] = "p";
    encode['Р'] = "R";encode['р'] = "r";
    encode['С'] = "S";encode['с'] = "s";
    encode['Т'] = "T";encode['т'] = "t";
    encode['У'] = "U";encode['у'] = "u";
    encode['Ф'] = "F";encode['ф'] = "f";
    encode['Х'] = "X";encode['х'] = "x";
    encode['Ц'] = "C";encode['ц'] = "c";
    encode['Ч'] = "Ch";encode['ч'] = "ch";
    encode['Ш'] = "Sh";encode['ш'] = "sh";
    encode['Щ'] = "Sch";encode['щ'] = "sch";
    encode['Ъ'] = "'";encode['ъ'] = "'";
    encode['Ы'] = "I";encode['ы'] = "i";
    encode['Ь'] = "'";encode['ь'] = "'";
    encode['Э'] = "E";encode['э'] = "e";
    encode['Ю'] = "U";encode['ю'] = "u";
    encode['Я'] = "Ya";encode['я'] = "ya";





    _TCHAR rus[] = _T("АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЪЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя");
    for ( fs::recursive_directory_iterator i( "D:\\BackUp\\Annfb2" ); i != fs::recursive_directory_iterator(); ){
        if ( is_regular_file(i->status()) ){
            if ( i->path().extension() == ".epub"){
                std::string name = i->path().filename().string(), oname;
                oname = name = name.substr(0,name.length()-5);
                boost::replace_all(name,"_"," ");
                boost::replace_all(name,"-"," ");
                boost::replace_all(name,"."," ");
                boost::replace_all(name,"  "," ");
                boost::replace_all(name,"  "," ");
                boost::replace_all(name,"  "," ");
                std::string tmpname;
                for(auto &ch:name){
                    if ( strchr(rus, (int)ch))
                        tmpname += encode[ch];
                    else 
                        tmpname += ch;
                }
                name = tmpname;
                if ( name != oname ){
                    std::string opath = i->path().string();
                    boost::replace_last(opath,oname,name);
                    std::cout<<i->path().filename()<<" -> "<<name<<std::endl;
                    fs::rename(i->path(), opath);
                }
            }
        }
        try{
            ++i;
        }catch (const std::exception & ex ){// Просто пропустим системные и т.д. файлы, на которые у нас нет прав
            (void) ex;
            ++i;
        }
    }

    return 0;
}

