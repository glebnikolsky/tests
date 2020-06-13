// Selector.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "pugixml.hpp"

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <map>
#include <set>
#include <stack>
#include <string>

#include <boost\algorithm\string.hpp>
#include <boost\format.hpp>
#include <boost\lexical_cast.hpp>
#include <boost\noncopyable.hpp>
#include <boost\regex.hpp>
#include <boost\tokenizer.hpp>
#include <boost\tuple\tuple.hpp>


class PugSelector: public boost::noncopyable{
public:
    PugSelector(const char* flnm):filename_(flnm){}
    bool Load();
    bool Check();
    bool CheckScript(int no);
    size_t Size();
    std::string GetCode(int i);
    int GetType(int i);
    std::string GetName(int i);
    std::string GetQuery(int i);
    int GetTypeByCode(const char* code);
    std::string GetNameByCode(const char* code);
    std::string GetQueryByCode(const char* code);
    std::string last_error_;
private:
    std::string filename_;
    pugi::xml_document doc_;        ///< документ
    int size_;
};

bool PugSelector::Check()
{
    
    {
        pugi::xpath_query query("/SELECTOR/CRITERION[@type='2']");
        pugi::xpath_node_set res = query.evaluate_node_set(doc_);
        if ( res.size() == 0){
            last_error_ = _T("Отсутствует базовый запрос!");
            return false;
        }else if ( res.size() > 1){
            last_error_ = _T("Базовый запрос должен быть один!");
            return false;
        }
    }
    {
        pugi::xpath_query query("/SELECTOR/CRITERION[(@type!=2) or not (@type)]");
        pugi::xpath_node_set res = query.evaluate_node_set(doc_);
        for(auto &i:res){
            std::string code = i.node().child_value(_T("CODE"));
            if ( code.empty() ){
                last_error_ = _T("Тег CODE пропущен!");
                return false;
            }
            std::ostringstream xpathByCode;
            xpathByCode<<boost::format("/SELECTOR/CRITERION[CODE='%1%']") %code;
            pugi::xpath_query query(xpathByCode.str().c_str());
            pugi::xpath_node_set sub_res = query.evaluate_node_set(doc_);
            if ( sub_res.size() != 1){
                last_error_ = _T("Код встречается более одного раза :") + code;
                return false;
            }
        }

    }

    return true;
}

bool PugSelector::Load()
{
    if ( filename_ == std::string() ){
        last_error_ = "не указан файл";
        return false;
    }
    pugi::xml_parse_result result = doc_.load_file(filename_.c_str());
    if ( !result){
        last_error_ = result.description();
        return false;
    }
    pugi::xpath_query query("/SELECTOR");
    pugi::xpath_node_set res = query.evaluate_node_set(doc_);
    std::string tmp = res[0].node().attribute("version").as_string();
    size_ = Size();
    return tmp == "2.0";

}

size_t  PugSelector::Size()
{
    pugi::xpath_query query("/SELECTOR/CRITERION");
    pugi::xpath_node_set res = query.evaluate_node_set(doc_);
    return res.size();
}


std::string PugSelector::GetCode(int i)
{
    if ( i >= size_ )
        throw std::out_of_range("Selector. index too big");
    std::ostringstream xpathByCode;
    xpathByCode<<boost::format("/SELECTOR/CRITERION[%1%]") %(i+1);
    pugi::xpath_query query(xpathByCode.str().c_str());
    pugi::xpath_node_set res = query.evaluate_node_set(doc_);
    return res[0].node().child_value(_T("CODE"));
}

int PugSelector::GetType(int i)
{
    if ( i >= size_ )
        throw std::out_of_range("Selector. index too big");
    std::ostringstream xpathByCode;
    xpathByCode<<boost::format("/SELECTOR/CRITERION[%1%]") %(i+1);
    pugi::xpath_query query(xpathByCode.str().c_str());
    pugi::xpath_node_set res = query.evaluate_node_set(doc_);
    return res[0].node().attribute("type").as_int(0);
}

std::string PugSelector::GetName(int i)
{
    if ( i >= size_ )
        throw std::out_of_range("Selector. index too big");
    std::ostringstream xpathByCode;
    xpathByCode<<boost::format("/SELECTOR/CRITERION[%1%]") %(i+1);
    pugi::xpath_query query(xpathByCode.str().c_str());
    pugi::xpath_node_set res = query.evaluate_node_set(doc_);
    return res[0].node().child_value(_T("NAME"));
}

std::string PugSelector::GetQuery(int i)
{
    if ( i >= size_ )
        throw std::out_of_range("Selector. index too big");
    std::ostringstream xpathByCode;
    xpathByCode<<boost::format("/SELECTOR/CRITERION[%1%]") %(i+1);
    pugi::xpath_query query(xpathByCode.str().c_str());
    pugi::xpath_node_set res = query.evaluate_node_set(doc_);
    return res[0].node().child_value("QUERY");

}

std::string PugSelector::GetNameByCode(const char* code)
{
    std::ostringstream xpathByCode;
    xpathByCode<<boost::format("/SELECTOR/CRITERION[CODE='%1%']") %code;
    pugi::xpath_query query(xpathByCode.str().c_str());
    pugi::xpath_node_set res = query.evaluate_node_set(doc_);
    if( res.size() !=1 ) throw std::out_of_range("Код отсутствует или дублируется");
    return res[0].node().child_value("NAME");
}

std::string PugSelector::GetQueryByCode(const char* code)
{
    std::ostringstream xpathByCode;
    xpathByCode<<boost::format("/SELECTOR/CRITERION[CODE='%1%']") %code;
    pugi::xpath_query query(xpathByCode.str().c_str());
    pugi::xpath_node_set res = query.evaluate_node_set(doc_);
    if( res.size() !=1 ) throw std::out_of_range("Код отсутствует или дублируется");
    return res[0].node().child_value("QUERY");
}

int PugSelector::GetTypeByCode(const char* code)
{
    std::ostringstream xpathByCode;
    xpathByCode<<boost::format("/SELECTOR/CRITERION[CODE='%1%']") %code;
    pugi::xpath_query query(xpathByCode.str().c_str());
    pugi::xpath_node_set res = query.evaluate_node_set(doc_);
    if( res.size() !=1 ) throw std::out_of_range("Код отсутствует или дублируется");
    return res[0].node().attribute("type").as_int(0);
}

bool PugSelector::CheckScript(int no)
{
    std::string sql = GetQuery(no);
    struct b{
        b(std::string nm, int ln = -1):nm_(nm), ln_(ln){}
        std::string nm_;
        int ln_;
        operator std::string(){
            return nm_+"("+boost::lexical_cast<std::string>(ln_)+")";
        }
        bool operator !=(const b &r){
            return nm_ !=  r.nm_;
        }
    };
    std::set<std::string> vars;
    std::set<std::string> tags;
    std::stack<b> braces;
    boost::tokenizer<boost::char_separator<char> > sqltok(sql, boost::char_separator<char>("\n", "", boost::keep_empty_tokens));
    boost::regex open_brace(_T("\\s*--<([A-Za-z_0-9]+)\\s*")),
                 close_brace(_T("\\s*-->([A-Za-z_0-9]+)\\s*")),
                 var(_T(".*\\$([A-Zz-z_0-9]+)\\$.*"));
    int ln =1;
    for( auto lines = sqltok.begin(); lines != sqltok.end(); ++lines, ++ln){
        std::string line = *lines;
        boost::smatch what;
        if ( boost::regex_match(line, what, open_brace, boost::match_perl) ){
            braces.push(b(what[1].str(),ln));
            tags.insert(what[1].str());
            continue;
        }
        if ( boost::regex_match(line, what, close_brace, boost::match_perl) ){
            if ( braces.empty() || braces.top() != what[1].str() ){
                last_error_ = "несбалансированный закрывающий тег "+ (std::string)b(what[1].str(),ln);
                return false;
            }
            braces.pop();
            continue;
        }
        if ( boost::regex_match(line, what, var, boost::match_perl) ) vars.insert(what[1].str());
    }
    if ( braces.size() ){
        last_error_ = (braces.size() == 1 ? "несбалансированный тег: ":"несбалансированные теги: ") + (std::string)braces.top();
        braces.pop();
        for(;braces.size(); braces.pop()) last_error_ += "|" + (std::string)braces.top();
        return false;
    }
    std::set<std::string> tmp1,tmp2;
    std::set_difference(vars.begin(), vars.end(), tags.begin(), tags.end(), std::inserter(tmp1,tmp1.end()));
    std::set_difference(tags.begin(), tags.end(), vars.begin(), vars.end(), std::inserter(tmp2,tmp2.end()));
    if( tmp1.size()){
        last_error_ = "Переменные без тегов: ";
        for(const auto &i:tmp1) last_error_ += i + "|";
    }
    if( tmp2.size()){
        last_error_ += (last_error_.empty()?"":"\n") + std::string("Теги без переменных: ");
        for(const auto &i:tmp2) last_error_ += i + "|";
    }
    return true;
}



int region = 2;         //$REGION$
int area = -1;            //$AREACODE$
int school =-1;             //school
int category =-1;
int testType =-1;
int wave=-1;
int subject=-1;
int start = 100;
int stop = 200;
std::string date="";
bool deleted = false;
std::string surname = "";
std::string name = "";
std::string secondname = "";
std::string guid= "";
std::string docser = "";
std::string docno = "";
std::string selector = "";

std::string doing(std::vector<std::string> &sql, std::map<std::string, std::string> &subst, std::map<std::string, bool> &used)
{
    auto FindTag =[&sql](std::string tag, int start){ for(int i = start; i < sql.size(); ++i) if ( sql[i] == tag ) return i; return -1;};
    for(auto &i:used){
        // разберёмся с конструкцией вида --< --! --> - аналог if else endif
        int else_point = FindTag(_T("--!") + i.first, 0);
        if ( else_point >= 0 ){
            if ( i.second ){ //если тег используется, то долбанём всё между --! --> и вытрем открывающий тег - больше он не нужен
                int end_point = FindTag(_T("-->") + i.first, else_point);
                while( else_point <= end_point) sql[else_point++] = _T("");
                sql[FindTag(_T("--<") + i.first, 0)] = _T("");
            }
            else{//если тег не используется, то долбанём всё между --< --! и вытрем закрывающий тег - больше он не нужен
                int start = FindTag(_T("--<") + i.first, 0);
                while( start <= else_point ) sql[start++] = _T("");
                sql[FindTag(_T("-->") + i.first, else_point)] = _T("");
            }
        }
        if ( i.second) continue; // тег используется, значит сохраним содержимое
// иначе попробуем удалить всё внутри тега, если перед ним нет открывающегося или после  него нет закрывающегося
        for( int open = FindTag(_T("--<") + i.first, 0); open >= 0; ){
            int close = FindTag(_T("-->") + i.first, open);
            if ( close <0 ) throw std::out_of_range("Плохой шаблон селектора!");
            bool del = !(boost::starts_with(sql[open-1],_T("--<")) || boost::starts_with(sql[open+1],_T("--<"))) // непосредственно перед или за нашим тегом нет открывающегося
                     ||!(boost::starts_with(sql[close-1],_T("-->")) || boost::starts_with(sql[close+1],_T("-->")));// непосредственно перед или за нашим тегом нет закрывающегося
            if ( del ) while( open <= close) sql[open++] = "";
            open = FindTag(_T("--<") + i.first, close);
        }
    }
    std::string res;
    for(auto &i:sql) 
        if ( i == "" || boost::starts_with(i,_T("--"))) continue;
        else res += i + _T("\n");

    boost::replace_last(res,_T("WHERE\n)WHERE"),_T(")WHERE"));
    boost::replace_last(res,_T("WHERE\nAND"),_T("WHERE "));
    boost::replace_last(res,_T("WHERE AND"),_T("WHERE "));

    for(auto &i:subst) boost::replace_all(res, i.first, i.second);
    return res;
}

void prepare( std::map<std::string, std::string> &subst, std::map<std::string, bool> &used)
{
    used[_T("DELETED")] = !deleted;

    if ( selector != _T("")){
        subst[_T("$SELECTOR$")] = selector;
        used[_T("SELECTOR")] = true; //используется
    }else used[_T("SELECTOR")] = false; //используется

    if ( surname != _T("")){
        if( boost::contains(surname, _T("%")) ) subst[_T("$SURNAME$")] = _T("LIKE '") + surname +"'";
        else subst[_T("$SURNAME$")] = _T("= '") + surname + _T("'");
        used[_T("SURNAME")] = true; //используется
    }else used[_T("SURNAME")] = false; //используется

    if ( name != _T("")){
        if( boost::contains(name, _T("%")) ) subst[_T("$NAME$")] = _T("LIKE '") + name +"'";
        else subst[_T("$NAME$")] = _T("= '") + name + _T("'");
        used[_T("NAME")] = true; //используется
    }else used[_T("NAME")] = false; //используется

    if ( secondname != _T("")){
        if( boost::contains(secondname, _T("%"))  ) subst[_T("$SECONDNAME$")] = _T("LIKE '") + secondname +"'";
        else subst[_T("$SECONDNAME$")] = _T("= '") + secondname + _T("'");
        used[_T("SECONDNAME")] = true; //используется
    }else used[_T("SECONDNAME")] = false; //используется

    if ( guid != _T("")){
        subst[_T("$GUID$")] = guid;
        used[_T("GUID")] = true; //используется
    }else used[_T("GUID")] = false; //используется

    if ( docser != _T("")){
        if( boost::contains(docser, _T("%")) ) subst[_T("$DOCSER$")] = _T("LIKE '") + docser +"'";
        else subst[_T("$DOCSER$")] = _T("= '") + docser + _T("'");
        used[_T("DOCSER")] = true; //используется
    }else used[_T("DOCSER")] = false; //используется

    if ( docno != _T("")){
        if( boost::contains(docno, _T("%")) ) subst[_T("$DOCNO$")] = _T("LIKE '") + docno +"'";
        else subst[_T("$DOCNO$")] = _T("= '") + docno + _T("'");
        used[_T("DOCNO")] = true; //используется
    }else used[_T("DOCNO")] = false; //используется

    if ( region != -1 ){
        subst[_T("$REGION$")] = boost::lexical_cast<std::string>(region);
        used[_T("REGION")] = true; //используется
    }else used[_T("REGION")] = false; //не используется

    if ( area != -1 ){
        subst[_T("$AREACODE$")] = boost::lexical_cast<std::string>(area);
        used[_T("AREA")] = true; //используется
    }else used[_T("AREA")] = false; //не используется

    if ( school != -1 ){
        subst[_T("$SCHOOLCODE$")] = boost::lexical_cast<std::string>(school);
        used[_T("SCHOOL")] = true; //используется
    }else used[_T("SCHOOL")] = false; //не используется

    if ( category != -1 ){
        subst[_T("$PARTICIPANTCATEGORY$")] = boost::lexical_cast<std::string>(category);
        used[_T("PARTICIPANTCATEGORY")] = true; //используется
    }else used[_T("PARTICIPANTCATEGORY")] = false; //не используется

    if ( testType != -1 ){
        subst[_T("$TESTTYPECODE$")] = boost::lexical_cast<std::string>(testType);
        used[_T("TESTTYPECODE")] = true; //используется
    }else used[_T("TESTTYPECODE")] = false; //не используется

    if ( testType != -1 ){
        subst[_T("$TESTTYPECODE$")] = boost::lexical_cast<std::string>(testType);
        used[_T("TESTTYPECODE")] = true; //используется
    }else used[_T("TESTTYPECODE")] = false; //не используется

    if ( wave != -1 ){
        subst[_T("$WAVE$")] = boost::lexical_cast<std::string>(wave);
        used[_T("WAVE")] = true; //используется
    }else used[_T("WAVE")] = false; //не используется

    if ( subject != -1 ){
        subst[_T("$SUBJECTCODE$")] = boost::lexical_cast<std::string>(subject);
        used[_T("SUBJECT")] = true; //используется
    }else used[_T("SUBJECT")] = false; //не используется
    
    if ( date != _T("") ){
        subst[_T("$EXAMDATE$")] = date;
        used[_T("EXAMDATE")] = true; //используется
    }else used[_T("EXAMDATE")] = false; //не используется

    subst[_T("$START$")] = boost::lexical_cast<std::string>(start);
    subst[_T("$STOP$")] = boost::lexical_cast<std::string>(stop);

}

void ParseFIO(std::string fio, std::string &f, std::string &i, std::string &o, std::string &g, std::string &n)
{
    f = i = o = g = n = _T("");
    if ( fio == _T("")) return;
    if ( boost::regex_match(fio, boost::regex(_T("\\{([0-9A-Fa-f]{8})-([0-9A-Fa-f]{4})-([0-9A-Fa-f]{4})-([0-9A-Fa-f]{4})-([0-9A-Fa-f]{12})\\}|([0-9A-Fa-f]{8})-([0-9A-Fa-f]{4})-([0-9A-Fa-f]{4})-([0-9A-Fa-f]{4})-([0-9A-Fa-f]{12})"),boost::match_perl))  ){
        g = fio;
        return;
    }
    if ( boost::regex_match(fio, boost::regex(_T("(\\d{12})"),boost::match_perl))  ){
        n = fio;
        return;
    }
    boost::smatch what;
    if ( !boost::regex_match(fio, what, boost::regex(_T("\\s*([А-ЯЁ][а-яё%]*)\\s*([А-ЯЁ][а-яё%]*)?\\s*([А-ЯЁ][а-яё%]*)?\\s*")), boost::match_perl)  ) return;
    f = what[1];
    i = what[2];
    o = what[3];
}

void ParseSN(std::string sn, std::string &s, std::string &n)
{
    s = n = _T("");
    if ( sn == _T("")) return;
    boost::tokenizer<boost::char_separator<char> > tokens(sn, boost::char_separator<char>("\t "));
    auto sn_parts = tokens.begin(); 
    if ( sn_parts != tokens.end() ){
//тут можно вставить проверку на правильность серии, но я не представляю себе всё разнообразие
        s = *sn_parts++;
    }
    if ( sn_parts != tokens.end() ){
        if ( !boost::regex_match(*sn_parts, boost::regex(_T("\\d{2,8}")))  ) return;
        n = *sn_parts++;
    }
}


bool tryThis(std::vector<char >&mtx, int n, int m, int sz, int i, int j)
{
    return false;
}


bool remove(pugi::xml_document &doc, std::string id)
{
    std::vector<pugi::xml_node> tmp;
    pugi::xml_node node = doc.child("node");
    for(pugi::xml_node nn = node.first_child(); nn; nn = nn.next_sibling())
    {
        bool found = nn.attribute("id").as_string() != id;
        if (!found){
            nn.parent().remove_child(nn);
            break;
        }
    }
    return true;
}


std::string mail = _T("<Mails last_readf=\"0\" last_readr=\"0\"><Message id=\"1\" date=\"01.02.2017 12:03:33\" source=\"r\"><![CDATA[message body]]></Message></Mails>");


int _tmain(int argc, _TCHAR* argv[])
{
    using namespace std;
{    pugi::xml_document mdoc;
    pugi::xml_parse_result result = mdoc.load_string(mail.c_str());
    pugi::xml_node mroot = mdoc.child("Mails");
    int readf = mroot.attribute("last_readf").as_int();
    int readr = mroot.attribute("last_readr").as_int();

    pugi::xpath_query gquery("/Mails/Message[@id>'0' and @source='r']");
    pugi::xpath_node_set mres = gquery.evaluate_node_set(mdoc);
    for( size_t i = 0; i < mres.size(); ++i){
        std::cout<<mres[i].node().first_child().value();
    }
}


    std::string sql_template = _T(
"SELECT * FROM (SELECT rbd_Participants.*, ROW_NUMBER() OVER(ORDER BY ParticipantID) as No \n\
--<GUID\n\
        FROM rbd_Participants WHERE rbd_Participants.ParticipantID = '$GUID$'\n\
-->GUID\n\
        FROM rbd_Participants join rbd_Schools on \n\
        ( rbd_Participants.Region = rbd_Schools.REGION AND rbd_Participants.SchoolRegistration =  rbd_Schools.SchoolID ) \n\
--<AREA \n\
        join rbd_Areas on (rbd_Participants.REGION = rbd_Areas.Region AND rbd_Schools.AreaFK = rbd_Areas.AreaID) \n\
-->AREA \n\
--<WAVE \n\
--<TESTTYPECODE \n\
--<EXAMSUBJECT \n\
--<EXAMDATE \n\
        join res_HumanTests on (rbd_Participants.region = res_HumanTests.region AND res_HumanTests.ParticipantFK = rbd_Participants.ParticipantID) \n\
-->EXAMDATE  \n\
-->EXAMSUBJECT \n\
-->TESTTYPECODE \n\
        join dat_Exams on ( res_HumanTests.ExamDate = dat_Exams.ExamDate AND res_HumanTests.SubjectCode = dat_Exams.SubjectCode) \n\
-->WAVE \n\
        WHERE \n\
--<REGION \n\
        rbd_Participants.region = $REGION$ \n\
-->REGION \n\
--<DELETED \n\
        AND rbd_Schools.DeleteType = 0 \n\
--<AREA \n\
        AND rbd_Areas.IsDeleted = 0 \n\
-->AREA \n\
        AND rbd_Participants.DeleteType = 0 \n\
-->DELETED \n\
--<AREA \n\
        AND rbd_Areas.AreaCode = $AREACODE$  \n\
-->AREA \n\
--<SCHOOL \n\
        AND rbd_Schools.SchoolCode = $SCHOOLCODE$ \n\
-->SCHOOL \n\
--<PARTICIPANTCATEGORY \n\
        AND rbd_Participants.ParticipantCategoryFK = $PARTICIPANTCATEGORY$ \n\
-->PARTICIPANTCATEGORY \n\
--<SUBJECTCODE \n\
        AND res_HumanTests.SubjectCode = $SUBJECTCODE$ \n\
-->SUBJECTCODE \n\
--<TESTTYPECODE \n\
        AND res_HumanTests.TestTypeCode = $TESTTYPECODE$ \n\
-->TESTTYPECODE \n\
--<WAVE \n\
--AND dat_Exams.WaveCode = $WAVE$ \n\
-->WAVE \n\
--<EXAMDATE \n\
--AND res_HumanTests.ExamDate = '$EXAMDATE$' \n\
-->EXAMDATE \n\
--<SURNAME\n\
        AND rbd_Participants.Surname = '$SURNAME$'\n\
-->SURNAME\n\
--<NAME\n\
        AND rbd_Participants.Name = '$NAME$'\n\
-->NAME\n\
--<SECONDNAME\n\
        AND rbd_Participants.Name = '$SECONDNAME$'\n\
-->SECONDNAME\n\
--<DOCNO\n\
AND rbd_Participants.DocumentNumber = '$DOCNO$'\n\
-->DOCNO\n\
--<DOCSER\n\
AND rbd_Participants.DocumentSeries = '$DOCSER$'\n\
-->DOCSER\n\
--<SELECTOR\n\
    AND $SELECTOR$\n\
-->SELECTOR\n\
-->GUID\n\
        )WHERE No BETWEEN $START$ AND \
                          $STOP$\n\
--THATS ALL, FOLKS     DON'T REMOVE THIS LINE!!!!!!!!!!!!!!!!!!!!!!!!\r\n");
        std::vector<std::string> sql_base,sql;

        pugi::xml_document doc;
        _TCHAR *source = "<node><n id='1' /><n id='2' /><n id='3' /><n id='4' /><n id='5' /><n id='6' /><n id='7' /><n id='8' /><n id='9' /></node>";
        pugi::xml_parse_result result = doc.load_string(source);
        if ( !result){
            std::cout << "Error description: " << result.description() << "\n";
            std::cout << "Error offset: " << result.offset << " (error at [..." << (source + result.offset) << "]\n\n";
            return -1;
        }

        for(pugi::xml_node nn = doc.child("node").child("n"); nn; nn = nn.next_sibling("n") ) cout<<nn.attribute("id").as_int()<<endl;

        remove(doc, "5");

        for(pugi::xml_node nn = doc.child("node").child("n"); nn; nn = nn.next_sibling("n") ) cout<<nn.attribute("id").as_int()<<endl;

        remove(doc, "1");

        for(pugi::xml_node nn = doc.child("node").child("n"); nn; nn = nn.next_sibling("n") ) cout<<nn.attribute("id").as_int()<<endl;

        remove(doc, "9");

        for(pugi::xml_node nn = doc.child("node").child("n"); nn; nn = nn.next_sibling("n") ) cout<<nn.attribute("id").as_int()<<endl;

        boost::tokenizer<boost::char_separator<char> > tokens(sql_template, boost::char_separator<char>("\r\n"));
        for (auto i = tokens.begin(); i != tokens.end(); ++i) sql.push_back(boost::trim_copy(*i));
        sql_base = sql;
        std::string res;
        {
            std::map<std::string, std::string> subst; 
            std::map<std::string, bool> used;
            prepare(subst,used);
            res = doing(sql, subst, used);
        }
        std::ofstream fs("d:\\selectors.sql");
        fs<<"---------------------------------------------------------------------------\r\n";
        fs<<res;

        region = 2;         
        area = -1;          
        school =-1;         
        category =-1;
        testType =-1;
        wave=-1;
        subject=-1;
        date="";
        deleted = true;
        {
            sql = sql_base;
            std::map<std::string, std::string> subst; 
            std::map<std::string, bool> used;
            prepare(subst,used);
            res = doing(sql, subst, used);
        }
        fs<<"---------------------------------------------------------------------------\r\n";
        fs<<res;

        region = 2;         
        area = -1;          
        school =-1;         
        category =-1;
        testType =-1;
        wave=-1;
        subject=-1;
        date="2015.12.02";
        deleted = true;
        {
            sql = sql_base;
            std::map<std::string, std::string> subst; 
            std::map<std::string, bool> used;
            prepare(subst,used);
            res = doing(sql, subst, used);
        }
        fs<<"---------------------------------------------------------------------------\r\n";
        fs<<res;

            region = 2;         
            area = -1;          
            school =-1;         
            category =-1;
            testType =-1;
            wave=2;
            subject=-1;
            date="2015.12.02";
            deleted = true;
            {
                sql = sql_base;
                std::map<std::string, std::string> subst; 
                std::map<std::string, bool> used;
                prepare(subst,used);
                res = doing(sql, subst, used);
            }
            fs<<"---------------------------------------------------------------------------\r\n";
            fs<<res;

            region = 2;         
            area = -1;          
            school =-1;         
            category =-1;
            testType =-1;
            wave=2;
            subject=-1;
            date="2015.12.02";
            deleted = false;
            {
                sql = sql_base;
                std::map<std::string, std::string> subst; 
                std::map<std::string, bool> used;
                prepare(subst,used);
                res = doing(sql, subst, used);
            }
            fs<<"---------------------------------------------------------------------------\r\n";
            fs<<res;


            region = 2;         
            area = -1;          
            school =-1;         
            category =-1;
            testType =-1;
            wave=2;
            subject=20;
            date="2015.12.02";
            deleted = false;
            {
                sql = sql_base;
                std::map<std::string, std::string> subst; 
                std::map<std::string, bool> used;
                prepare(subst,used);
                res = doing(sql, subst, used);
            }
            fs<<"---------------------------------------------------------------------------\r\n";
            fs<<res;

            region = 2;         
            area = -1;          
            school =-1;         
            category =1;
            testType =-1;
            wave=2;
            subject=20;
            date="2015.12.02";
            deleted = true;
            {
                sql = sql_base;
                std::map<std::string, std::string> subst; 
                std::map<std::string, bool> used;
                prepare(subst,used);
                res = doing(sql, subst, used);
            }
            fs<<"---------------------------------------------------------------------------\r\n";
            fs<<res;

            region = 2;         
            area = 1;          
            school =3846;         
            category =1;
            testType =-1;
            wave=2;
            subject=20;
            date="2015.12.02";
            deleted = true;
            {
                sql = sql_base;
                std::map<std::string, std::string> subst; 
                std::map<std::string, bool> used;
                prepare(subst,used);
                res = doing(sql, subst, used);
            }
            fs<<"---------------------------------------------------------------------------\r\n";
            fs<<res;

            region = 2;         
            area = 1;          
            school =3846;         
            category =1;
            testType =-1;
            wave=2;
            subject=20;
            date="2015.12.02";
            guid = _T("{123456789})");
            deleted = true;
            {
                sql = sql_base;
                std::map<std::string, std::string> subst; 
                std::map<std::string, bool> used;
                prepare(subst,used);
                res = doing(sql, subst, used);
            }
            fs<<"---------------------------------------------------------------------------\r\n";
            fs<<res;
                
            PugSelector ps("Selector.prt");
            std::cout<<ps.Load()<<std::endl;
            std::cout<<ps.Size()<<std::endl;
            if ( !ps.Check() )
                std::cout<<ps.last_error_<<std::endl;
            else 
                std::cout<<"Check OK!\n";
            int i = 0;
            std::cout<<ps.GetCode(0)<<std::endl;
            std::cout<<ps.GetType(0)<<std::endl;
            std::cout<<ps.GetName(0)<<std::endl;
            std::cout<<ps.GetQuery(0)<<std::endl;

            i = 1;
            std::cout<<ps.GetCode(i)<<std::endl;
            std::cout<<ps.GetType(i)<<std::endl;
            std::cout<<ps.GetName(i)<<std::endl;
            std::cout<<ps.GetQuery(i)<<std::endl;

            i = 10;
            std::cout<<ps.GetCode(i)<<std::endl;
            std::cout<<ps.GetType(i)<<std::endl;
            std::cout<<ps.GetName(i)<<std::endl;
            std::cout<<ps.GetQuery(i)<<std::endl;

            i = 16;
            std::cout<<ps.GetCode(i)<<std::endl;
            std::cout<<ps.GetType(i)<<std::endl;
            std::cout<<ps.GetName(i)<<std::endl;
            std::cout<<ps.GetQuery(i)<<std::endl;

            i = 20;
            std::cout<<ps.GetTypeByCode("20")<<std::endl;
            std::cout<<ps.GetNameByCode("20")<<std::endl;
            std::cout<<ps.GetQueryByCode("20")<<std::endl;
            i = 57;
            std::cout<<ps.GetTypeByCode("57")<<std::endl;
            std::cout<<ps.GetNameByCode("57")<<std::endl;
            std::cout<<ps.GetQueryByCode("57")<<std::endl;
            std::cout<<ps.CheckScript(0)<<std::endl;
            try{
                std::cout<<ps.GetQueryByCode("300")<<std::endl;
            }catch( std::out_of_range o){
                std::cout<<o.what()<<std::endl;
            }
            std::string no;
            ParseFIO("32164EEA-9862-4049-BD45-000685C84910", surname, name, secondname, guid, no);
            std::cout<<surname<<'|'<<name<<'|'<<secondname<<'|'<<guid<<'|'<<no<<std::endl;
            ParseFIO("{32164EEA-9862-4049-BD45-000685C84910", surname, name, secondname, guid, no);
            std::cout<<surname<<'|'<<name<<'|'<<secondname<<'|'<<guid<<'|'<<no<<std::endl;
            ParseFIO("{32164EEA-9862-4049-BD45-000685C84910}", surname, name, secondname, guid, no);
            std::cout<<surname<<'|'<<name<<'|'<<secondname<<'|'<<guid<<'|'<<no<<std::endl;
            ParseFIO("32164EEФ-9862-4049-BD45-000685C84910", surname, name, secondname, guid, no);
            std::cout<<surname<<'|'<<name<<'|'<<secondname<<'|'<<guid<<'|'<<no<<std::endl;
            ParseFIO("123456789012", surname, name, secondname, guid, no);
            std::cout<<surname<<'|'<<name<<'|'<<secondname<<'|'<<guid<<'|'<<no<<std::endl;
            ParseFIO("1234567890123", surname, name, secondname, guid, no);
            std::cout<<surname<<'|'<<name<<'|'<<secondname<<'|'<<guid<<'|'<<no<<std::endl;
            ParseFIO("12345678901", surname, name, secondname, guid, no);
            std::cout<<surname<<'|'<<name<<'|'<<secondname<<'|'<<guid<<'|'<<no<<std::endl;
            ParseFIO("Иванов А Петрович", surname, name, secondname, guid, no);
            std::cout<<surname<<'|'<<name<<'|'<<secondname<<'|'<<guid<<'|'<<no<<std::endl;
            ParseFIO("    \tИванов Александр ", surname, name, secondname, guid, no);
            std::cout<<surname<<'|'<<name<<'|'<<secondname<<'|'<<guid<<'|'<<no<<std::endl;
            ParseFIO("    \tИванов ", surname, name, secondname, guid, no);
            std::cout<<surname<<'|'<<name<<'|'<<secondname<<'|'<<guid<<'|'<<no<<std::endl;
            ParseFIO("    \tИванов А П", surname, name, secondname, guid, no);
            std::cout<<surname<<'|'<<name<<'|'<<secondname<<'|'<<guid<<'|'<<no<<std::endl;
            ParseFIO("    \tИванов Александр П", surname, name, secondname, guid, no);
            std::cout<<surname<<'|'<<name<<'|'<<secondname<<'|'<<guid<<'|'<<no<<std::endl;
            ParseFIO("    \tИванов     Александр    Петрович   ", surname, name, secondname, guid, no);
            std::cout<<surname<<'|'<<name<<'|'<<secondname<<'|'<<guid<<'|'<<no<<std::endl;
            ParseFIO("", surname, name, secondname, guid,no);
            std::cout<<surname<<'|'<<name<<'|'<<secondname<<'|'<<guid<<'|'<<no<<std::endl;
            ParseSN("", docser, docno);
            std::cout<<docser<<'|'<<docno<<'|'<<std::endl;
            ParseSN(" sdfsf 12345  ", docser, docno);
            std::cout<<docser<<'|'<<docno<<'|'<<std::endl;
            ParseSN(" sdfsf   a12345  ", docser, docno);
            std::cout<<docser<<'|'<<docno<<'|'<<std::endl;
            ParseSN("   ", docser, docno);
            std::cout<<docser<<'|'<<docno<<'|'<<std::endl;
            ParseSN("  as ", docser, docno);
            std::cout<<docser<<'|'<<docno<<'|'<<std::endl;


    return 0;
}





/*
#include <cmath>
#include <cstdio>
#include <vector>
using namespace std;

bool try_set(vector<long int> &v, long int k)
{
for(long int i = 1; i < v.size(); ++i)
for(long int j = 0; j < v.size(); ++j)
if( v[i] && v[j] && (i+j == k) )
return false;
return true
}

int min_elem(vector<long int> &v, long int n)
{
long int mn = 0, mni = 0;
for(int i = 1; i < v.size(); ++i)
if ( v[i] && v[i] < n){
mn = v[i];
mni = i;
}
return mni;
}

int main() {

long int n,k;
cin>>n>>k;
vector<long int> v(k,0);
for(int i =0; i < n; ++i){
    long int tmp;
    cin>>tmp;
    ++v[tmp%k];
}
// 0 1 2 3 4 5 6 7 ... n отбросим 0 и пропустим все нулевые значения начнём сверху
long int m = 0, mx = 0;
for(int i = 1; i < n; ++i) m += v[i] ? 1:0;
vector<long int> cur(v);
for(int i = 1; i < m-1; ++i){
    if ( try_set(cur,k)) break;

}



return 0;
}
*/