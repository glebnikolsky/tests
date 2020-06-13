// ConsoleTypography.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ConsoleTypography.h"
#include <iostream>
#include <sstream>
#include <boost/any.hpp>
#include <boost/lexical_cast.hpp>

//#include "TpgLoadFile.h"

//#include <boost/log/trivial.hpp>
//#include <boost/log/common.hpp>
//#include <boost/log/expressions.hpp>
//#include <boost/log/attributes.hpp>
//#include <boost/log/sources/logger.hpp>
//#include <boost/log/sinks/sync_frontend.hpp>
//#include <boost/log/sinks/text_file_backend.hpp>
//#include <boost/filesystem.hpp>
//#include <boost/system/system_error.hpp>
//#include "pugixml.hpp"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//TCHAR GetControlNoEAN13(TCHAR const *sBarcode12, bool is8)
//{
//    int code(0);
//    int veight[13] = {1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1};
//    for(int i =0; sBarcode12[i] && i < (is8 ? 7 : 12); ++i ){
//        code += veight[i]*(sBarcode12[i]-L'0');
//    }
//    return L'0' + ((code%10) ? 10 - (code%10): 0) ;
//}
//
//bool CheckEAN13(TCHAR const * ean, bool is8)
//{
//    int veight[13] = {1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1};
//
//    int code(0);
//    for(int i =0; ean[i] && i < (is8 ? 8 : 13); ++i ){
//        code += veight[i]*(ean[i]-L'0');
//    }
//    return !(code%10);
//}


// The one and only application object

CWinApp theApp;

using namespace std;


string last_opened_dir = _T("\\.");

//void InitLog()
//{
//    
//    typedef boost::log::sinks::synchronous_sink< boost::log::sinks::text_file_backend > file_sink;
//    boost::shared_ptr< file_sink > sink(new file_sink(
//        boost::log::keywords::file_name = _T("Typography%Y%m%d_%H%M%S.log"), //При каждом запуске новый файл с датой и временем в имени
//        boost::log::keywords::rotation_size = 16*1024*1024                   // максимальный размер
//    ));
//
//    sink->locked_backend()->set_file_collector(boost::log::sinks::file::make_collector(
//        boost::log::keywords::target = "logs",                                      // директория от cwd
//        boost::log::keywords::max_size = 64 * 1024 * 1024,                          // максимальный суммарный объём
//        boost::log::keywords::min_free_space = 100 * 1024 * 1024                    // минимальное свободное место на диске - не проверял - что и как он сообщит
//                                                                                    // проверю позже, скорее всего бросит исключение
//     ));
//
//    sink->locked_backend()->scan_for_files();                                       // посмотрит - не стереть ли чего
//
//    boost::log::core::get()->set_filter
//        (
//        boost::log::trivial::severity >= 1//logging::trivial::info
//        /*
//        namespace trivial {
//
//        //! Trivial severity levels
//        enum severity_level
//        {
//        trace,
//        debug,
//        info,
//        warning,
//        error,
//        fatal
//        };
//*/
//            
//        );
//    boost::log::core::get()->add_sink(sink);
//}

//CString SetLetterNoDate(CString txt, CString no, CString date)
//{
//	pugi::xml_document doc;
//	pugi::xml_parse_result result = doc.load_string(txt);
//	pugi::xml_node root = doc.first_element_by_path("/Root/Letter");
//	if ( !root) {
//		pugi::xml_node node;
//		root = doc.first_element_by_path("/Root");
//		if ( !root ) root = doc.append_child("Root");
//		node = root.append_child("Letter");
//		node.append_child("No");
//		node.append_child("Date");
//	}
//	pugi::xml_node dat = doc.first_element_by_path("/Root/Letter/No");
//	if ( !dat ) dat = doc.first_element_by_path("/Root/Letter").append_child("No");
//	if ( ! dat.first_child() ) dat.append_child(pugi::node_pcdata).set_value(no.GetString());
//	else dat.first_child().set_value(no.GetString());
//	dat = doc.first_element_by_path("/Root/Letter/Date");
//	if ( !dat ) dat = doc.first_element_by_path("/Root/Letter").append_child("Date");
//	if ( ! dat.first_child() ) dat.append_child(pugi::node_pcdata).set_value(date.GetString());
//	else dat.first_child().set_value(date.GetString());
//	std::ostringstream  res;
//	doc.save(res);
//	return res.str().c_str();
//}
//
//
//void GetLetterNoDate(CString txt, CString &no, CString &date)
//{
//	pugi::xml_document doc;
//	pugi::xml_parse_result result = doc.load_string(txt);
//	pugi::xml_node node = doc.first_element_by_path("/Root/Letter/No");
//	if ( node && node.first_child() ) no = node.first_child().value();
//	node = doc.first_element_by_path("/Root/Letter/Date");
//	if ( node && node.first_child() ) date = node.first_child().value();
//}
//

struct CellData{

	SetString(std::string s){
		data_ = s;
	}
	std::string GetString()
	{
		if ( data_.type() == typeid(std::string()) ) return boost::any_cast<std::string>(data_);
		if ( data_.empty() ) return std::string();
		if ( data_.type() == typeid(bool) ) //
	}

bool GetBool()
{

}
	boost::any data_;

}



int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;
    setlocale(LC_ALL, "ru-RU");
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
            {



			//CString txt = _T("<Root><Mails last_readf=\"0\" last_readr=\"0\"></Mails></Root>");
			//CString res = SetLetterNoDate(txt,"1","2");
			//CString no, date;
			//GetLetterNoDate(txt, no, date);
			//std::cout<<"@@@"<<no.GetString()<<'\t'<<date.GetString()<<std::endl;
			//GetLetterNoDate(res, no, date);
			//std::cout<<"@@@"<<no.GetString()<<'\t'<<date.GetString()<<std::endl;
			//std::cout<<res.GetString()<<std::endl;
			//res = SetLetterNoDate(res,"4","5");
			//std::cout<<res.GetString()<<std::endl;
			//GetLetterNoDate(res, no, date);
			//std::cout<<"@@@"<<no.GetString()<<'\t'<<date.GetString()<<std::endl;
			res = SetLetterNoDate("","4","5");
			std::cout<<res.GetString()<<std::endl;



            //pugi::xml_document doc;
            //doc.load_file("save_file_output.xml");
            //
            //pugi::xml_node parent = doc.first_child();
            //if ( strcmp(parent.name(),"GenLocString") )
            //    parent = doc.append_child("GenLocString");
            //int no = 0;
            //for (pugi::xml_node_iterator it = parent.begin(); it != parent.end(); ++it){

            //    std::cout<<it->name()<<"  "<<it->attribute("no").as_int()<<endl;
            //    ++no;
            //}
            //pugi::xml_node cproj = parent.append_child("LockStringProject");
            //cproj.append_attribute("no") = no+1;
            //pugi::xml_node node = cproj.append_child("VSproject");
            //node.append_attribute("file") = "dfsadfsadfsadgfdsfgsd\\dfg sdfgsdfgsdfgsdfg\\dsfgsdfgsdfgsdfg";
            //node.append_attribute("name") = "dsfgsdfgsdfgsdfg";
            //node.append_attribute("prefix") = "FTC.";
            //pugi::xml_node menu = cproj.append_child("menu");
            //menu.append_attribute("IDR") = "sadkfjaslkdjflksadjfklsad'";
            //    node = menu.append_child("menuItem");
            //    node.append_attribute("name") = "adadasdadad";
            //    node.append_attribute("id") = 12;
            //    node.append_attribute("caption") = "sdfsadgf sdfgsdgsdfgsdfgsdfg dsfg dsfg";
            //    node = menu.append_child("menuItem");
            //    node.append_attribute("name") = "qqqqqqqqqqqqqq";
            //    node.append_attribute("id") = 13;
            //    node.append_attribute("caption") = "dsglsd; sdg dfg ";
            //pugi::xml_node dialog = cproj.append_child("dialog");
            //dialog.append_attribute("name") = "sfsa fsadfsa dgfsadgfsadg";
            //dialog.append_attribute("IDD") = 30;
            //    node = dialog.append_child("control");
            //    node.append_attribute("ID") = 12;
            //    node.append_attribute("name") = "sad asFD SAFASDF ASF";
            //    node.append_attribute("Caption") = "sd afsfgsdgf sdgsdg";



            //    no = 0;
            //    for (pugi::xml_node_iterator it = parent.begin(); it != parent.end(); ++it)++no;
            //    cproj = parent.append_child("LockStringProject");
            //    cproj.append_attribute("no") = no+1;
            //    node = cproj.append_child("VSproject");
            //    node.append_attribute("file") = "dfsadfsadfsadgfdsfgsd\\dfg sdfgsdfgsdfgsdfg\\dsfgsdfgsdfgsdfg";
            //    node.append_attribute("name") = "dsfgsdfgsdfgsdfg";
            //    node.append_attribute("prefix") = "FTC.";
            //    menu = cproj.append_child("menu");
            //    menu.append_attribute("IDR") = "sadkfjaslkdjflksadjfklsad'";
            //        node = menu.append_child("menuItem");
            //    node.append_attribute("name") = "adadasdadad";
            //    node.append_attribute("id") = 12;
            //    node.append_attribute("caption") = "sdfsadgf sdfgsdgsdfgsdfgsdfg dsfg dsfg";
            //    node = menu.append_child("menuItem");
            //    node.append_attribute("name") = "qqqqqqqqqqqqqq";
            //    node.append_attribute("id") = 13;
            //    node.append_attribute("caption") = "dsglsd; sdg dfg ";
            //    dialog = cproj.append_child("dialog");
            //    dialog.append_attribute("name") = "sfsa fsadfsa dgfsadgfsadg";
            //    dialog.append_attribute("IDD") = 30;
            //    node = dialog.append_child("control");
            //    node.append_attribute("ID") = 12;
            //    node.append_attribute("name") = "sad asFD SAFASDF ASF";
            //    node.append_attribute("Caption") = "sd afsfgsdgf sdgsdg";
            //    std::cout << "Saving result: " << doc.save_file("save_file_output.xml") << std::endl;



            }


         //   CString qqqq = _T("HFJFDGHHVHV");
         //   qqqq.MakeLower();

         //   int i = atoi(_T("ffff"));

         //   double big = 1., little = 1e-16, summ = big+little;
         //   if ( big != summ )
         //       cout<<"WTF!!!!";
         //   
         //   double third = 2/3.;
         //   for( double numerator = 4., denominator = 6.; denominator > 0; numerator += 2, denominator += 3)
         //       if ( third != numerator/denominator ) 
         //           cout<<"Gotcha! "<<numerator<<'/'<<denominator<<endl;



         //   CString s = _T("460154602129");
         //   TCHAR cha = GetControlNoEAN13(s.GetString(), false);
         //   TCHAR chb;
         //   s += cha;
         //   s = _T("4601546021290");
         //   bool boolb = CheckEAN13(s.GetString(), false);
         //   for ( cha = L'0'; cha <= L'9'; cha+=1)
         //   {
         //       s = s.Right(12);
         //       s = cha + s;
         //       chb = GetControlNoEAN13(s.GetString(), false);
         //       s = s.Left(12) + chb;
         //       if ( !CheckEAN13(s.GetString(), false))
         //           cout<<"!!!!!!!!!!!!!!!!!!!!!!!!";
         //   }



         //   


         //   CDatabase db;
         //   CString spath(_T("d:\\work\\testfilesystem"));
         //   boost::filesystem::path p(spath.GetString());
         //   std::cout<<exists(p)<<std::endl;
         //   boost::system::error_code ec;
         //   create_directory(p, ec);
         //   std::cout<<exists(p)<<std::endl;
         //   spath = _T("d:\\work\\a\\b\\c");
         //   p = spath.GetString();
         //   std::cout<<exists(p, ec)<<std::endl;
         //   std::cout << "  ec:\n"
         //       << "    value() is " << ec.value() << '\n'
         //       << "    category().name() is " << ec.category().name() << '\n'
         //       << "    message() is " <<  ec.message() << '\n'
         //       ;
         //   create_directory(p, ec);
         //   std::cout<<exists(p)<<std::endl;
         //   std::cout << "  ec:\n"
         //       << "    value() is " << ec.value() << '\n'
         //       << "    category().name() is " << ec.category().name() << '\n'
         //       << "    message() is " <<  ec.message() << '\n'
         //       ;

         //   BOOL b = db.OpenEx(_T("DRIVER=SQL Server;SERVER={10.0.18.3};DATABASE={ERBD_EGE_MAIN_15};UID={ra};PWD={Njkmrjcdjb};ConnectionTimeout=10000"), CDatabase::useCursorLib);
         //   InitLog();
         //   //boost::log::add_common_attributes();

         //   using namespace boost::log::trivial;
         //   boost::log::sources::severity_logger< severity_level > lg;

        	//CFolderPickerDialog  fFolder;
         //   fFolder.m_ofn.lpstrTitle = _T("Выберите директорию с типографскими файлами");
         //   fFolder.m_ofn.lpstrInitialDir = last_opened_dir.c_str();
         //   if ( fFolder.DoModal() == IDOK ){
         //       last_opened_dir = fFolder.GetFolderPath().GetString();
         //       wcout << last_opened_dir.c_str() << endl;
         //       BOOST_LOG_SEV(lg, info) << "SelectFiles "+last_opened_dir;
         //       SelectFiles(&db, last_opened_dir);
         //   }
         //   CFileDialog fileDlg (TRUE, NULL, NULL, OFN_FILEMUSTEXIST|OFN_ALLOWMULTISELECT, _T("csv files (*.csv)|*.csv|txt files(*.txt)|*.txt||"), NULL);
         //   fileDlg.m_ofn.lpstrTitle = _T("Выберите один или более файлов для загрузки");
         //   fileDlg.m_ofn.lpstrInitialDir = last_opened_dir.c_str();
         //   if ( fileDlg.DoModal() == IDOK ){
         //       POSITION pos = fileDlg.GetStartPosition();
         //       while( pos ){
         //           string tmp = fileDlg.GetNextPathName(pos).GetString();
         //           BOOST_LOG_SEV(lg, info) << "ProcFile "+tmp;
         //           ProcFile(&db, tmp );
         //           wcout<<tmp.c_str()<<endl;
         //       }
         //   }



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
