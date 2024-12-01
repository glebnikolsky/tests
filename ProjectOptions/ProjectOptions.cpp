// ProjectOptions.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

#include <boost/program_options.hpp>
#include <boost/signals2/signal.hpp>
#include <boost/tokenizer.hpp>
#include "pugixml.hpp"

using namespace std;
using namespace boost;
namespace fs = std::filesystem;
namespace po = boost::program_options;

namespace {
    string delimiter{ '#' };
    bool show{ false };
    bool ack{ false };
    bool changed{ false };
    pugi::xml_document doc;
    string txt;
}

using Remove_tag = vector<pair<string, string>>;
using Add_value = vector<tuple<string, string, string>>;
using Replace_reg = vector<tuple<regex, string, int>>;
using Replace_attrib = vector<tuple<string, string, string, string>>;
using Replace_attrib_reg = vector<tuple<string, string, regex, string, int>>;
using Replace_value = vector<tuple<string, string, string>>;
using Replace_value_reg = vector<tuple<string, regex, string, int>>;

// Additional command line parser which interprets '@something' as a
// option "config-file" with the value "something"
pair<string, string> at_option_parser(string const& s)
{
    if ('@' == s[0])
        return std::make_pair(string("response-file"), s.substr(1));
    else
        return pair<string, string>();
}

vector<vector<string>> Parse(const vector<string> &v)
{
    boost::char_separator<char> field_sep(delimiter.c_str(), "", boost::keep_empty_tokens);
    vector<vector<string>> res;
    for (auto& i : v) {
        boost::tokenizer<boost::char_separator<char> > tokens(i, field_sep);
        vector<string> tmp;
        for (auto& tok : tokens) tmp.push_back(tok);
        res.push_back(tmp);
    }
    return res;
}

struct RemoveTag {
    void operator()() {
        for (auto& [parnt, chld] : val_) {
            pugi::xpath_query query(parnt.c_str());
            try {
                pugi::xpath_node_set res = query.evaluate_node_set(doc);
                for (auto& i : res) {
                    i.node().remove_child(chld.c_str());
                    changed = true;
                }
            }
            catch (const pugi::xpath_exception& e)
            {
                std::cout << "Ошибка: " << e.what() << '\n';
                return;
            }
        }
    }
    bool AddParam(vector<string> param) {
        vector<vector<string>> params = Parse(param);
        const size_t vals{ 2 };
        int p{ 0 };
        for(auto &i:params) {
            if (i.size() != vals) {
                cerr << "Ошибка в параметре -g '" << param[p] << "'\n";
                return false;
            }
            val_.emplace_back(i[0], i[1]);
        }
        return true;
    }
    Remove_tag val_;
};

struct ReplaceReg {
    void operator()() {
        boost::char_separator<char> field_sep("\r\n", "", boost::keep_empty_tokens);
        boost::tokenizer<boost::char_separator<char> > tokens(txt, field_sep);
        string newtxt;
        for (auto& i : tokens) {
            for (auto& [re, replace_with, no] : val_) {
                smatch sm;
                if (regex_match(ln, sm, re)) {
                    string tmp;
                    for (size_t i = 1; i < sm.size(); ++i) tmp += (i == no ? replace_with : sm[i]);
                    ln = tmp;
                }
            }
        }
    }
    bool AddParam(vector<string> param) {
        vector<vector<string>> params = Parse(param);
        const size_t vals{ 3 };
        int p{ 0 };
        for (auto& i : params) {
            if (i.size() != vals) {
                cerr << "Ошибка в параметре -s '" << param[p] << "'\n";
                return false;
            }
            val_.emplace_back(i[0], i[1],  stoi(i[2]));
        }
        return true;
    }
    Replace_reg val_;
};


//bool ParseReplace(const vector<string>& v)
//{
//    const size_t vals{ 3 };
//    for (auto& i : v) {
//        vector<string> tmp = Parse(i);
//        if (tmp.size() != vals) {
//            cerr << "В параметре -s '" << i << "' " << tmp.size() << " подпараметров, ожидалось " << vals;
//            return false;
//        }
//        try {
//            replace_reg.emplace_back(tmp[0], tmp[1], stoi(tmp[2]));
//        }
//        catch (const std::exception& e) {
//            cout << "Bad reg: " << tmp[0] << " " << e.what();
//            return false;
//        }
//    }
//    return true;
//}

po::options_description set_options()
{
    po::options_description opt(" Параметры командной строки");
    opt.add_options()
        ("help,h", "Обязательные параметры:\nroot-dir filename-pattern b undo или хотя бы один из \n\
replace, replace-attrib, replace-attrib-reg, replace-value, replace-value-reg, add-value, remove-tag, \n\
Параметров search-and-replace, add-value и remove-tag может быть сколько угодно.")
        ("backup-off,b", "Не создавать bak файл.\nПо умолчанию создаётся.")
        ("undo,u", "Откатить последнюю операцию, создавшую bak файлы.")
        ("confirm,c","Подтверждать операцию.")
        ("delimiter,d", po::value<string>(), "Разделитель. По умолчанию '#'")
        ("root-dir,r", po::value<string>(), "Директория поиска файлов. Поиск рекурсивный.")
        ("filename,f", po::value<string>(), "Шаблон имени файла. Регулярное выражение.\n\
Пример: '.*_vs2015.vcxproj'")
        ("show,w", "Отображать операции")
        ("response-file,p", po::value<string>(), "Файл с параметрами. Так же можно просто @имяФайла\n\
Пример: '-p toboost1_80.txt' или '@toboost1_80.txt'\nМожет содержать любое количество команд\n\
replace, add-value, replace-attrib, replace-attrib-reg, replace-value, replace-value-reg, remove-tag")
        ("replace,x", po::value<vector<string>>(), "Регулярное выражение, замена, номер группы.\nПример: '(.*boost-1_)(\\d\\d)(.*)#86#2'")
        ("replace-attrib,i", po::value< vector<string> >(), "Родитель или Xpath, атрибут, Что найти, на что заменить\nПример: '/Project/ItemGroup/ProjectConfiguration#Include#Debug#DebugAll'")
        ("replace-attrib-reg,a", po::value< vector<string> >(), "Родитель или Xpath, атрибут, Что найти (регулярное выражение), на что заменить, какую группу\nПример: '/Project/PropertyGroup#Condition#(.*)(Debug)(|.*)#DebugAll#2'")
        ("replace-value,l", po::value< vector<string> >(), "Родитель или Xpath, Что найти, на что заменить\nПример: 'Debug#AllDebug'")
        ("replace-value-reg,g", po::value< vector<string> >(), "Родитель или Xpath, что найти (регулярное выражение), на что заменить, какую группу\n\
Пример: '//Configuration#(Debug)#AllDebug#1'")
        ("add-value,v", po::value< vector<string> >(), "Родитель или Xpath, дочерний тэг, значение. Будет добавлен дочерний тэг и значение, при отсутствии или заменено значение, если тэг был.\n\
Пример: '/Project/ItemDefinitionGroup/ClCompile#LanguageStandard#stdcpp17'")
        ("remove-tag,m", po::value< vector<string> >(), "Полный путь или Xpath. Будет удалён тэг нижнего уровня.\n\
Пример: '/Project/ItemDefinitionGroup/ClCompile#LanguageStandard'")
    ;
    return opt;
}

bool parse_options(int argc, char* argv[], po::options_description &opt, po::variables_map &vm)
{
    try{
        po::store(po::command_line_parser(argc, argv).options(opt).extra_parser(at_option_parser).run(), vm);
        if (vm.count("response-file")) {
            // Load the file and tokenize it
            ifstream ifs(vm["response-file"].as<string>().c_str());
            if (!ifs) {
                cerr << "Не открывается файл с опциями\n";
                return true;
            }
            stringstream ss;
            ss << ifs.rdbuf();
            boost::char_separator<char> sep(" \n\r");
            string sstr = ss.str();
            boost::tokenizer<boost::char_separator<char> > tok(sstr, sep);
            vector<string> args;
            copy(tok.begin(), tok.end(), back_inserter(args));
            store(po::command_line_parser(args).options(opt).run(), vm);
        }
    }
    catch (std::exception& e) {
        cerr << e.what();
        return true;
    }
    return false;
}


int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "ru-RU");
    po::options_description opt = set_options();
    po::variables_map vm;
    if (parse_options(argc, argv, opt, vm)) return 1;

    string root_dir;
    string filename_pattern;
    bool backup{ true };
    bool undo{ false };
    if (vm.count("show")) show = true;
    if (vm.count("backup-off")) backup = false;
    if (vm.count("undo")) undo = true;
    if (vm.count("confirm")) ack = true;
    
    if (vm.count("delimiter")) delimiter = vm["delimiter"].as<char>();
    if (vm.count("root-dir")) root_dir = vm["root-dir"].as<string>();
    if (vm.count("filename")) filename_pattern = vm["filename"].as<string>();
    if (vm.count("help")) {
        cout << opt;
        return 0;
    } 
    if (!vm.count("root-dir") || !vm.count("filename")) {
        cout << opt;
        return 0;
    }
    if (!vm.count("undo") && !(vm.count("replace") || vm.count("replace-attrib") || vm.count("replace-attrib-reg") 
        || vm.count("replace-value") || vm.count("replace-value-reg") || vm.count("add-value remove-tag"))){
        cout << opt;
        return 0;
    }
    boost::signals2::signal<void()> sig;
    RemoveTag remove_tag;
    if (vm.count("remove-tag") && remove_tag.AddParam(vm["remove-tag"].as<vector<string>>())) sig.connect(remove_tag);
//    if (vm.count("replace") && !ParseReplace(vm["replace"].as<vector<string>>())) return -1;

    fs::path search_in(root_dir);
    if (undo) filename_pattern += ".bak";
    cout << "show=" << show << " backup-off=" << backup << " undo=" << undo << " delimiter='" << delimiter << "'\n"<<
        "root_dir = "<< root_dir << " filename_pattern ="<< filename_pattern<<"\n";
    try{
        regex re_flnm(filename_pattern);
        for (fs::recursive_directory_iterator i(search_in); i != fs::recursive_directory_iterator(); ++i) {
            if (!i->is_regular_file()) continue;
            if (regex_match(i->path().string(), re_flnm)) {
                cout << i->path() << "\n";
            }
        }
    }
    catch (fs::filesystem_error ex) {
        cout << ex.what();
    }
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
