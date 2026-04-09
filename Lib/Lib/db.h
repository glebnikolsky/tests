#ifndef __LIB_DB_H__
#define __LIB_DB_H__
#pragma once
namespace db {
	//create db if not exists
	void CreateDb(const char* dbn);

	//Fill or update db
	void Fill(const char* dbn, const char* root);

	void CalcCrc(const char* dbn, const char* root);
}

#endif __LIB_DB_H__