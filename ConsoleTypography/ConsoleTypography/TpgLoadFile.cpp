#include "stdafx.h"
#include "ConsoleTypography.h"

#include <sstream>
#include <vector>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>
#include <boost/log/trivial.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/crc.hpp>

using namespace boost::log::trivial;

#include "TpgLoadFile.h"


namespace{

    /// \enum PackageType тип пакета (набора из трёх) файлов
    enum PackageType{ptKIM_, ptTOM_, ptZOU_, ptOVZ_  };

    /// \enum FileType тип файла.
    enum FileType{ftPDF_, ftBOX_, ftPACK_, ftKIM_ };


    
    std::string parse_file_name_rexp;
    
    /// check_file загружает файл, помещает содержимое в строку, вычисляет SHA1
	/**
		\param filename - полный путь
		\param fbody    - выходной, содержимое файла
		\param hash     - выходной SHA1 
		\return         - false, если файл не открылся. Обработки ошибок нет, поскольку это почти невероятный случай.
	*/
    bool CheckFile(std::string &fulle_name, std::string &file_name, std::string &base_name, std::string &disciple, std::string &date, std::string &zone, PackageType &pt, FileType &ft, int &version);

	/// GetPackageType определяет тип пакета файлов
	/**
		\param ptype - символьное имя пакета. Всегда правильное, поскольку выбирается регулярным выражением.
		\return тип пакета файлов типа PackageType
	*/
	PackageType GetPackageType(const std::string &ptype);


	/// GetFileType определяет тип файла
	/**
		\param ptype - символьный тип из имени файла. Всегда правильный, поскольку выбирается регулярным выражением. Всегда в нижнем регистре.
		\param ext - расширение файла. Всегда в нижнем регистре.
		\return тип файла типа FileType
	*/
	FileType GetFileType(const std::string &ftype, const std::string &ext);




    int AddLoadFileAndCalcCRC(std::string filename, std::vector< std::string> &file_body , unsigned long &crc32, int &lines);


    
//------------------------------------------------Реализация ------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    
std::ostream& operator<< (std::ostream& strm, PackageType pt)
{
    static const char* strings[] =
    {
        "ptKIM_",
        "ptTOM_",
        "ptZOU_",
        "ptOVZ_"
    };

    if (static_cast< std::size_t >(pt) < sizeof(strings) / sizeof(*strings))
        strm << strings[pt];
    else
        strm << static_cast< int >(pt);
    return strm;
}

std::ostream& operator<< (std::ostream& strm, FileType ft)
{
    static const char* strings[] =
    {
        "ftPDF_",
        "ftBOX_",
        "ftPACK_",
        "ftKIM_"
    };

    if (static_cast< std::size_t >(ft) < sizeof(strings) / sizeof(*strings))
        strm << strings[ft];
    else
        strm << static_cast< int >(ft);
    return strm;
}

PackageType GetPackageType(const std::string &ptype)
{
    if ( ptype.empty() )
        return ptKIM_;
    std::string types("tomzouovz");
    int pos = types.find(ptype);
    switch( pos ){
    case 0:
        return ptTOM_;
        break;
    case 3:
        return ptZOU_;
        break;
    case 6:
        return ptOVZ_;
        break;
    }
    return ptKIM_; // Для предотвращения предупреждения
}


FileType GetFileType(const std::string &ftype, const std::string &ext)
{
    if(ext == _T("txt") )
        return ftPDF_;
    std::string types(_T("kimboxpack"));
    int pos = types.find(ftype);
    switch( pos ){
    case 0:
        return ftKIM_;
        break;
    case 3:
        return ftBOX_;
        break;
    case 6:
        return ftPACK_;
        break;
    }
    return ftKIM_; // Для предотвращения предупреждения
}

bool CheckFile(std::string &full_name, std::string &file_name, std::string &base_name, std::string &disciple, std::string &date, std::string &zone, PackageType &pt, FileType &ft, int &version)
{
    boost::to_lower(full_name);
    if ( parse_file_name_rexp.empty() )
        parse_file_name_rexp = _T("(?i).*\\\\(\\d{2})_(\\d{6})_(\\d+)_?(kim|pack|box)?_?(tom|zou|ovz)?_?(\\d+)?\\.(csv|txt)");
    //      1 предмет, 2 дата, 3 зона, 4 тип файла, 5 тип пакета, 6 версия, 7 расширение
    boost::regex expr(parse_file_name_rexp);
    boost::smatch parsed;
    if ( ! boost::regex_match(full_name, parsed, expr, boost::match_perl) )
        return false;
    file_name = parsed[1] + '_' + parsed[2] + '_' + parsed[3];
    if ( parsed[4] != std::string() )
        file_name += '_' + parsed[4];
    if ( parsed[5] != std::string() )
        file_name += '_' + parsed[5];
    base_name = file_name;
    if ( parsed[6] != std::string() ){
        file_name += '_' + parsed[6];
        version = boost::lexical_cast<int>(parsed[6]);
    }else 
        version = 0;

    file_name += '.' + parsed[7];
    disciple = parsed[1];
    date = parsed[2];
    zone = parsed[3];
    pt = GetPackageType(parsed[5]);
    ft = GetFileType(parsed[4], parsed[7] );

    return true;
}

int AddLoadFileAndCalcCRC(std::string filename, std::vector< std::string> &file_body , unsigned long &crc32, int &lines)
{
    std::ifstream infile(filename);
    if (infile.bad()){
        //afxMessage()
        return -1;
    }
    std::string line;
    boost::crc_32_type  res32;
    while(getline(infile, line)){
        res32.process_bytes(line.c_str(), line.length());
        file_body.push_back(line);
        ++lines;
    }
    crc32 = res32.checksum();
    return 0;
}

}// Конец анонимного пространства имён



bool ProcFile(CDatabase *db, const std::string &full_name, bool check_file)
{
    boost::log::sources::severity_logger< boost::log::trivial::severity_level > lg;
    BOOST_LOG_SEV(lg, info) << "In ProcFile "+full_name;
 
    std::string name(full_name), file_name, base_name, disciple, date, zone;
    PackageType pt;
    FileType ft;
    int version;

    CheckFile(name, file_name, base_name, disciple, date, zone, pt, ft, version);
    std::ostringstream log_rec;
    log_rec<<boost::format(_T("After Check file %s %s %s %s %d")) % base_name.c_str() % disciple.c_str() % date.c_str() % zone.c_str() % version;
    log_rec<<pt<<' '<<ft;
    BOOST_LOG_SEV(lg, trace) << log_rec.str();
    std::string file_body;
    
    db->BeginTrans();
    std::vector< std::string> body;
    int lines(0);
    unsigned long crc32;
    AddLoadFileAndCalcCRC(name, body, crc32, lines);
    switch (ft)
    {
    case ftPDF_:
        break;

    case ftBOX_:
        break;
    case ftPACK_:
        break;
    case ftKIM_:
        break;
    default:
        break;
    }
    db->CommitTrans();

    return true;
}

bool SelectFiles(CDatabase *db, const std::string &dir_name)
{
    for ( boost::filesystem::recursive_directory_iterator itr( dir_name); itr != boost::filesystem::recursive_directory_iterator(); ){

        if ( is_regular_file(itr->status()) )
            ProcFile(db, itr->path().string(), true);

        try{
            ++itr;
        }catch (const std::exception & ex ){// Просто пропустим системные и т.д. файлы, на которые у нас нет прав
            (void) ex;
            itr.no_push();
            ++itr;
        }
    }
    return true;
}

