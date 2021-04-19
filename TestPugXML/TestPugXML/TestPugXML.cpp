Script::// TestPugXML.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TestPugXML.h"
#include "pugixml.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <sstream>
#include <string>
#include <set>
#include  <vector>

//#include <boost/crc.hpp>  
//#include <boost/format.hpp>  

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//struct DBVersion{
//    DBVersion(const char *ver){
//        const char *begin = ver;
//        const char *end = ver;
//        while( *begin ){
//            while( *end  && *end != '.' )++end;
//            ver_.push_back(atol(begin));
//            begin = *end ? ++end: end;
//        }
//    }
//    DBVersion(){}
//
//    void operator()(const char *ver){
//        DBVersion tmp(ver);
//        ver_.swap(tmp.ver_);
//    }
//
//    operator std::string(){
//        std::string tmp;
//        char buf[10];
//        for(auto i:ver_){
//            _ltoa_s(i, buf, 10, 10);
//            tmp += (tmp.empty() ? "" : ".");
//            tmp += buf;
//        }
//        return tmp;
//    }
//
//    const DBVersion& operator =(const DBVersion &rhs)
//    {
//        ver_ = rhs.ver_;
//        return *this;
//    }
//
//    bool empty() const \
//    {
//        return !ver_.size(); 
//    }
//
//    bool operator <(const DBVersion &rhs){
//        for(size_t i = 0; i < min(ver_.size(),rhs.ver_.size()); ++i )
//            if ( ver_[i] == rhs.ver_[i] ) continue;
//            else if ( ver_[i] < rhs.ver_[i] ) return true;
//            else return false;
//        if ( ver_.size() < rhs.ver_.size()) return true;
//        else return false;
//    }
//
//    std::vector<long> ver_;
//};
//
//
//class Script{
//
//
//public:
//    //������������/�����������
//    Script():changed_(false)
//            ,filename_(std::string())
//    {
//        pugi::xml_parse_result result = doc_.load_string(empty_);
//    }
//
//    Script(const char* filename):changed_(false)
//                                ,filename_(filename)
//    {}
//    
//    //���������� �� ��������� ���� ������� ������
//
//
//   	/*!
//	* \brief
//	* ����� ��� �� ��� ������� ������������ ���� ������
//    *
//    * \returns
//	* ��� �� ��� ������� ������������ ���� ������
//	* 
//	*/
//    std::string GetType() { 
//        pugi::xml_node scripts;
//        scripts = doc_.child("Scripts");
//        return scripts.attribute("dbtype").as_string();
//    }
//
//   	/*!
//	* \brief
//	* ������ ��������� � ��������� ������
//    *
//    * \returns
//	* ��������� � ��������� ������
//	* 
//	*/
//    std::string GetLastError(){ return last_error_;}
//
//    /*!
//	* \brief
//	* ��������� ��� �� ��� ������� ������������ ���� ������
//	* 
//	* \param const char* type
//	* ��� ��
//	* 
//	*/
//    void SetType(const char* type){ 
//        pugi::xml_node scripts = doc_.child("Scripts");
//        pugi::xml_attribute attr;
//        if ( (attr = scripts.attribute("dbtype")) ) attr.set_value(type);
//        else scripts.append_attribute("dbtype") = type;
//        changed_ = true; 
//    }
//
//
//   	/*!
//	* \brief
//	* ��������� ��� ����� ��� �������� � ����������
//	* 
//	* \param const char* type
//	* ��� ��
//	* 
//	*/
//    void SetFilename(const char* filename){ filename_ = filename; }
//
//    /*!
//	* \brief
//	* �������� ��������� �� �����, �������������� ���������� 
//    *
//    * \returns
//	* true - �����, false - �����, ����������� � last_error_
//	* 
//	*/
//    bool LoadXML();
//
//   	/*!
//	* \brief
//	* �������� ��������� - ������������������ id, ������, ������������ crc
//    *
//    * \returns
//	* true - �����, false - �����, ����������� � last_error_
//	* 
//	*/
//    bool Check();
//
//    /*!
//	* \brief
//	* ������������ �������� - - ������������������ id, ������������ crc, 
//    * ���� version ������������ �� ������� ������� ������
//    *
//    * \returns
//	* true - �����, false - �����, ����������� � last_error_
//	* 
//	*/
//    bool Rebuild();
//
//    /*!
//	* \brief
//	* ��������� �������� � filename_
//    *
//    * \returns
//	* true - �����, false - �����, ����������� � last_error_
//	* 
//	*/
//    bool Save();
//
//
//    /*!
//	* \brief
//	* ��������� �������� � filename
//    *
//    * \param const char *filename
//    * ��� ����� ��� ����������
//    *
//    * \returns
//	* true - �����, false - �����, ����������� � last_error_
//	* 
//	*/
//    bool SaveAs(const char *filename);
//
//
//    /*!
//	* \brief
//	* ���� �� ����� ������ ��� ���������� � ��
//    * ���������� � �����  while(HasVer(...)){ ��������� ����;}
//    *
//    * \param const char* lastVerInDB
//    * ������ ��
//    *
//    * \param const char* guid
//    * ������ ��
//    *
//    * \param std::string &newVer
//    * ���� ����� ������ ����, �� ������ �������� �.
//    *
//    * \returns
//	* true - �������, false - �� �������, ��������������, ��� ������-���� ���������
//	* 
//	*/
//    bool HasVer(const char* dbVer, const char* guid, std::string &newVer);
//
//    /*!
//	* \brief
//	* �������� ������ � ��������, id � crc ����������� �������������
//    *
//    * \param const char *new_ver
//    * ����� ����� ������
//    *
//    * \param const char* sql
//    * ������
//    *
//    * \param const char *comment = NULL
//    * �����������
//    *
//    * \param const char* guid = NULL
//    * guid ��� out of order ��������
//    *
//    * \returns
//	* true - �����, false - �����, ����������� � last_error_
//	* 
//	*/
//    bool AddScript(const char *new_ver, const char* sql, const char *comment=NULL, const char* guid = NULL);
//
//
//    /*!
//	* \brief
//	* �������� ������ �� �����, �������� � ��������, id � crc ����������� �������������
//    *
//    * \param const char *new_ver
//    * ����� ����� ������
//    *
//    * \param const char* sql_file
//    * ���� �� ��������
//    *
//    * \param const char *comment = NULL
//    * �����������
//    *
//    * \param const char* guid = NULL
//    * guid ��� out of order ��������
//    *
//    * \returns
//	* true - �����, false - �����, ����������� � last_error_
//	* 
//	*/
//    bool AddScriptFile(const char *new_ver, const char* sql_file, const char *comment=NULL, const char* guid = NULL);
//
//    /*!
//	* \brief
//	* ���������� ��������
//    *
//    * \returns
//	* ���������� ������� ��� -1 � ������ ������
//	* 
//	*/
//    int NoOfRecs();
//
//   /*!
//	* \brief
//	* �������� ������ #recno, ������� ����
//    *
//    * \param int recno
//    * ����� ������
//    *
//    * \param CString& dbtype
//    * ��� ��
//    *
//    * \param CString& ver
//    * ������
//    *
//    * \param CString& date
//    * ���� ����������
//    *
//    * \param CString& guid
//    * ����
//    *
//    * \param CString& comment
//    * �����������
//    *
//    * \param CString& sql
//    * ������
//    *
//    * \returns
//	* true - �����, false - �����, ����������� � last_error_
//	* 
//	*/
//    bool GetRecord(int recno, CString &dbtype, CString &ver, CString &date,  CString &guid, CString &comment, CString &sql);
//
//    
//
//protected:
//    const static char* empty_;      ///< ������ � ������ ���������� 
//    pugi::xml_document doc_;        ///< ��������
//    //pugi::xml_node scripts_;        ///< ��� �������� ������ Scripts (��� ��������� ���������)
//    bool changed_;                  ///< ��� �� ������ ��������?
//    std::string filename_;          ///< ���� ��� �������� � ����������
//    std::string last_error_;        ///< ��������� � ��������� ������
//};
//
//
//
//
//const char* Script::empty_ = "<?xml version=\"1.0\" encoding=\"WINDOWS-1251\"?><Scripts dbtype='none'/>";
//
//bool Script::LoadXML()
//{
//    if ( filename_ == std::string() ){
//        last_error_ = "�� ������ ����";
//        return false;
//    }
//    pugi::xml_parse_result result = doc_.load_file(filename_.c_str());
//    if ( !result){
//        last_error_ = result.description();
//        return false;
//    }
//    return true;
//}
//
//
//bool Script::Check()
//{
//    int id=0;
//    std::string ver;
//    boost::crc_32_type  computer;
//    computer.reset();
//    for(pugi::xml_node script = doc_.first_element_by_path("/Scripts/Script"); script; script = script.next_sibling())
//    {
//
//        pugi::xml_attribute attr = script.attribute("id");
//        if ( attr ){
//            int newId = attr.as_int();
//            if ( newId != (id+1) ){
//                last_error_ = "������� ������� ���������� ������ �� id";
//                return false;
//            }else id = newId;
//        }else{
//            last_error_ = "����������� ������� id";
//            return false;
//        }
//        pugi::xml_attribute guid = script.attribute("guid");
//        bool outOfOrder = (bool)guid;
//        if ( (attr = script.attribute("version"))){
//            std::string newVer = attr.as_string();
//            if ( !ver.empty() && ver >= newVer ){
//                last_error_ = "������� ������� ���������� ������ �� ������ ������";
//                return false;
//            }else if ( !outOfOrder)
//                ver = newVer;
//        }else{
//            last_error_ = "����������� ������� version";
//            return false;
//        }
//        if ( (attr = script.attribute("crc"))){
//            const char *ch = script.first_child().value();
//            computer.process_bytes(ch, strlen(ch));
//            if ( computer.checksum() != attr.as_uint()){
//                last_error_ = "�� ��������� ����������� �����";
//                return false;
//            }
//        }else{
//            last_error_ = "����������� ������� crc";
//            return false;
//        }
//    }
//
//    return true;
//}
//
//
//bool Script::Rebuild()
//{
//    int id=1;
//    boost::crc_32_type  computer;
//    computer.reset();
//    for(pugi::xml_node script = doc_.first_element_by_path("/Scripts/Script"); script; script = script.next_sibling()){
//        pugi::xml_attribute attr = script.attribute("id");
//        if ( attr )
//            attr.set_value(id);
//        else script.append_attribute("id")=id;
//        ++id;
//        const char *ch = script.first_child().value();
//        computer.process_bytes(ch, strlen(ch));
//        if ( attr = script.attribute("crc") ) 
//            attr.set_value(computer.checksum());
//        else script.append_attribute("crc") = computer.checksum();
//    }
//    return true;
//}
//
//
//bool Script::SaveAs(const char *filename)
//{
//    filename_ = filename;
//    return Save();
//
//}
//
//bool Script::Save()
//{
//    return doc_.save_file(filename_.c_str(), "\t", pugi::format_indent, pugi::xml_encoding::encoding_auto );
//}
//
//
//bool Script::HasVer(const char* dbVer, const char* guid, std::string &newVer)
//{
//    std::ostringstream xpathByGuid;
//    DBVersion dbver(dbVer), gver, rver;
//    pugi::xpath_node_set res;
//
//    if ( guid ){
//        xpathByGuid<<boost::format("/Scripts/Script[@guid='%1%']") %guid;
//        pugi::xpath_query gquery(xpathByGuid.str().c_str());
//        res = gquery.evaluate_node_set(doc_);
//        for( size_t i = 0; i < res.size(); ++i){
//            DBVersion tmp(res[i].node().attribute("version").as_string());
//            if ( dbver < tmp ){
//                gver = tmp;
//                break;
//            }
//        }
//    }
//    pugi::xpath_query rquery("/Scripts/Script[not (@guid)]");
//    res = rquery.evaluate_node_set(doc_);
//    for( size_t i = 0; i < res.size(); ++i){
//        DBVersion tmp(res[i].node().attribute("version").as_string());
//        if ( dbver < tmp ){
//            rver = tmp;
//            break;
//        }
//    }
//    bool ret = false;
//    if ( !rver.empty() && !gver.empty()){
//        newVer = min(rver,gver);
//        ret = true;
//    }else if ( !rver.empty() ){
//        newVer = rver;
//        ret = true;
//    }else if ( !gver.empty() ){
//        newVer = gver;
//        ret = true;
//    }
//    return ret;
//}
//
//
//bool Script::AddScript(const char *new_ver, const char* sql, const char *comment, const char* guid)
//{
//    pugi::xpath_query query("/Scripts/Script[last()]");
//    pugi::xpath_node_set res = query.evaluate_node_set(doc_);
//    DBVersion newVer(new_ver);
//    if ( res.size() ) {
//        if ( !(DBVersion(res[0].node().attribute("version").as_string()) < newVer) ) {
//            last_error_ = "������������ ����� ������";
//            return false;
//        }
//    }
//    pugi::xml_node node = doc_.child("Scripts");
//    node = node.append_child("Script");
//    boost::crc_32_type  computer;
//    computer.reset();
//    computer.process_bytes(sql, strlen(sql));
//    node.append_attribute("crc") = computer.checksum();
//    node.append_attribute("version") = new_ver;
//    if ( comment ) node.append_attribute("comment") = comment;
//    if ( guid ) node.append_attribute("guid") = guid;
//    node.append_attribute("date") = CTime::GetCurrentTime().Format(_T("%Y.%m.%d %H:%M:%S")).GetString();
//    node.append_child(pugi::node_cdata).set_value(sql);
//    changed_ = true; 
//    return true;
//}
//
//
//int Script::NoOfRecs()
//{
//    pugi::xpath_query query("/Scripts/Script");
//    pugi::xpath_node_set res = query.evaluate_node_set(doc_);
//    return res.size();
//}
//
//bool Script::GetRecord(int recno, CString &dbtype, CString &ver, CString &date,  CString &guid, CString &comment, CString &sql)
//{
//    pugi::xpath_query query("/Scripts/Script");
//    pugi::xpath_node_set res = query.evaluate_node_set(doc_);
//    if(!res.size() || recno >= res.size()){
//        last_error_ = "������������ ����� ������";
//        return false;
//    }
//    dbtype = GetType().c_str();
//    ver = res[recno].node().attribute("version").as_string();
//    date = res[recno].node().attribute("date").as_string();
//    guid = res[recno].node().attribute("guid").as_string();
//    comment = res[recno].node().attribute("comment").as_string();
//    sql = res[recno].node().first_child().value();
//    return true;
//}


// The one and only application object

CWinApp theApp;

using namespace std;

struct Selector {
	Selector(const TCHAR *q = "\0", const TCHAR *flnm = "\0"): filename_(flnm), qtext_(q){}
    void SetQuery(const TCHAR *q) { qtext_ = q; }
	bool Open(std::string f = std::string());
	std::vector<std::string> GetNames();
	std::string GetSql(int no);
	std::set<std::string> GetRequired(int no);
	bool IsOpen();
	std::string ErrorText();
protected:
	pugi::xpath_node_set Query();

protected:
    std::string filename_;
    std::string qtext_;
	pugi::xml_document doc_;
	pugi::xml_parse_result resp_;
};

bool Selector::Open(std::string flnm)
{
	if (!flnm.empty()) filename_ = flnm;
	resp_ = doc_.load_file(filename_.c_str());
	return (bool)resp_;
}

bool Selector::IsOpen()
{
	return (bool)resp_;
}

pugi::xpath_node_set Selector::Query()
{
	pugi::xpath_query query(qtext_.c_str());
	return query.evaluate_node_set(doc_);
}


std::vector<std::string> Selector::GetNames()
{
	std::vector<std::string> res;
	pugi::xpath_node_set resq = Query();
	for (auto i : resq) res.push_back(i.node().attribute("name").as_string());
	return res;
}

std::string Selector::GetSql(int no)
{
	pugi::xpath_node_set resq = Query();

	return resq[no].node().first_child().value();
}

std::string Selector::ErrorText()
{
	return resp_.description();
}

std::set<std::string> Selector::GetRequired(int no)
{
	 std::set<std::string> res;
	 pugi::xpath_node_set resq = Query();
	 resq[no].node().print(std::cout, "", pugi::format_raw);
	 string s = resq[no].node().attribute("requr").as_string();
	 while(!s.empty()){
		int pos = s.find(';');
		if ( pos  > 0 ){
			res.insert(s.substr(0,pos));
			s = s.substr(pos+1);
		}else{
			res.insert(s.substr(0));
			s = string();
		}
	 }
	 return res;
}


int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// initialize MFC and print and error on failure
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: change error code to suit your needs
			_tprintf(_T("Fatal Error: MFC initialization failed\n"));
			nRetCode = 1;
		}
		else
		{
			//Selector apd_;
			//apd_.SetQuery("/Selector/Criterion[@type='rro']");
		 //   apd_.Open(_T("Selector.apd"));

			//if (!apd_.IsOpen()) {
			//	std::string err(_T("�� ����������� ���� Selector.apd.\n "));
			//	err += apd_.ErrorText();
			//	AfxMessageBox(err.c_str());
			//}
			//std::string sql = apd_.GetSql(2);
			//std::set<std::string> t = apd_.GetRequired(2);
			pugi::xml_document doc;
			pugi::xml_parse_result result;
			result = doc.load_string("<test atr1=\"1\" atr2=\"2\"/>");
			if ( !result) 
				std::cout<<result.description()<<'\n';
			pugi::xml_node test = doc.first_child();
			std::cout<<test.attribute("atr1").as_string()<<"\t"<<test.attribute("atr2").as_string()<<"\t"<<test.attribute("atr3").as_string()<<"\n"<<
				test.attribute("atr1").as_int()<<"\t"<<test.attribute("atr2").as_int()<<"\t"<<test.attribute("atr3").as_int()<<"\n"
				<<(test.attribute("atr3").as_string() == "");


		}
	}
	else
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: GetModuleHandle failed\n"));
		nRetCode = 1;
	}

	return nRetCode;
}

