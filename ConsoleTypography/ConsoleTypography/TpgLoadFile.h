/*!
 * \brief
 * Интерфейс загрузки типографских файлов
 * 
 * Прячем все вспомогательные функции и boost библиотеки в реализацию
 * 
 */

#ifndef TPGLOADFILE_H
#define TPGLOADFILE_H
#include <string>



bool ProcFile(CDatabase *db, const std::string &file_name, bool check_file = false);

bool SelectFiles(CDatabase *db, const std::string &dir_name);



#endif