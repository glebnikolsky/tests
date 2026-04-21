#include "stdafx.h"
#include "db.h"
#include "sqlite3pp.h"

#include <chrono>
#include <fstream>
#include <iostream>
#include <numeric>
#include <set>
#include <sstream>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/hash2/md5.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

namespace fs = boost::filesystem;
namespace sql3 = sqlite3pp;
using namespace std;

namespace {
	const char* kDBStruct = "\
PRAGMA foreign_keys = ON;\n\
PRAGMA temp_store = 2;\n\
PRAGMA page_size = 8192;\n\
PRAGMA journal_mode=off;\n\
PRAGMA synchronous = off;\n\
\n\
CREATE TABLE IF NOT EXISTS Types(\n\
	type_id INTEGER NOT NULL,\n\
	type TEXT PRIMARY KEY \n\
);\n\
\
\INSERT INTO Types(type_id,type) VALUES(5,'.pdf'), (3,'.epub'), (2,'.fb2'), (4,'.djvu'), (1,'.zip');\n\
\
CREATE TABLE IF NOT EXISTS Folders(\n\
	folder_id	INTEGER PRIMARY KEY AUTOINCREMENT,\n\
	folder TEXT NOT NULL\n\
);\n\
\
CREATE UNIQUE INDEX IF NOT EXISTS folders_idx ON Folders(folder);\n\
\
CREATE TABLE IF NOT EXISTS Files(\n\
	file_id	INTEGER PRIMARY KEY AUTOINCREMENT,\n\
	file TEXT NOT NULL,\n\
	type TEXT NOT NULL,\n\
	size INTEGER NOT NULL,\n\
	crc TEXT NULL,\n\
	folder_id	INTEGER REFERENCES Folders(folder_id)\n\
);\n\
\
CREATE TABLE IF NOT EXISTS Genres(\n\
	genre_id INTEGER PRIMARY KEY AUTOINCREMENT,\n\
	genre TEXT NOT NULL\n\
);\n\
\
CREATE TABLE IF NOT EXISTS Authors(\n\
	autor_id INTEGER PRIMARY KEY AUTOINCREMENT,\n\
	last_name TEXT NOT NULL,\n\
	first_name TEXT,\n\
	middle_name TEXT\n\
);\n\
\
CREATE TABLE IF NOT EXISTS BooksGenres(\n\
	file_id	INTEGER REFERENCES Files(file_id),\n\
	genre_id INTEGER REFERENCES Genres(genre_id)\n\
);\n\
\
CREATE TABLE IF NOT EXISTS BooksAuthors(\n\
	file_id	INTEGER REFERENCES Files(file_id),\n\
	autor_id INTEGER REFERENCES Author(autor_id)\n\
);\n\
\
CREATE TABLE IF NOT EXISTS BooksTitles(\n\
	file_id	INTEGER REFERENCES Files(file_id),\n\
	title TEXT NOT NULL\n\
);\n\
\
CREATE INDEX IF NOT EXISTS FilesIdxType ON Files( type );\n\
\
CREATE INDEX IF NOT EXISTS FilesIdxFile ON Files( file );";

	template<typename T>bool GetFieldVal(sql3::query::iterator& rec, int field_no, T& val)
	{
		switch ((*rec).column_type(field_no)) {
		case SQLITE_INTEGER:
			val = boost::lexical_cast<T>((*rec).get<long long>(field_no));
			break;

		case SQLITE_TEXT:
			val = boost::lexical_cast<T>((*rec).get<const char*>(field_no));
			break;

		case SQLITE_FLOAT:
			val = boost::lexical_cast<T>((*rec).get<double>(field_no));
			break;

		case SQLITE_BLOB:
			val = boost::lexical_cast<T>((*rec).get<const char*>(field_no));
			break;

		case SQLITE_NULL:
			return false;
		}
		return true;
	}

	string DupApos(const string& s) {
		string res(s);
		boost::replace_all(res, "'", "''");
		return res;
	}
	struct insert_muli {
		insert_muli(sql3::database &db,const char* hdr, long max_count=3000):db_(db),hdr_(hdr), max_count_(max_count),count_(0),value_(string()){}
		
		void Add(const string& ins) {
			if (!count_) value_ += hdr_ + ins + "\n";
			else value_ += "," + ins + "\n";
			if (++count_ == max_count_) Flush();
		}
		void Flush() {
			if (!count_) return;
			value_ += ";";
			int ret = db_.execute(value_.c_str());
			if (ret) {
				cout << db_.error_msg() << ends;
				exit(1);
			}
			value_.clear();
			count_ = 0;
		}
		sql3::database& db_;
		string hdr_;
		string value_;
		long max_count_;
		long count_;

	};

	struct folder {
		string path_;
		long id_;

		long CheckDir(sql3::database &dbs, const fs::path& p) {
			if (path_ == p.string() && id_) return id_;
			path_ = p.string();
			ostringstream sql;
			sql << boost::format("SELECT folder_id FROM Folders WHERE folder = '%1%'") % DupApos(path_);
			sql3::query q(dbs, sql.str().c_str());
			sql3::query::iterator i = q.begin();
			if (i != q.end()) {
				id_ = (*i).get<long long>(0);
				return id_;
			}
			int ret = dbs.executef("INSERT INTO Folders(folder) VALUES('%s');", DupApos(path_).c_str());
			id_ = static_cast<long>(dbs.last_insert_rowid());
			cout << path_ << endl;
			return id_;
		}
	};

	template<typename T>
	string PrettyNum(T val, const char* delim = "'") {
		string num = to_string(val);
		for (int pos = num.length() - 3; pos > 0; pos -= 3) num.insert(pos, delim);
		return num;
	}

	string Calc(const string& p, size_t len) {
		ifstream ifs(p.c_str(), ios::binary);
		constexpr size_t kBsize = 1'204 * 1'024;
		ifstream fl(p.c_str(), ios::in | ios::binary);
		boost::hash2::md5_128 hash;
		try {
			while (len) {
				vector<char> v(min(kBsize, len));
				ifs.read(&v[0], v.size());
				hash.update(&v[0], v.size());
				if (len >= kBsize) len -= kBsize;
				else len = 0;
 			}
		}
		catch (exception e) {
			cout << e.what();
		}
		return to_string(hash.result());
	}
}

namespace db{
	void CreateDb(const char* dbn)
	{
		sql3::database dbs(dbn);
		int ret = dbs.execute(kDBStruct);

		if (ret) {
			cout << dbs.error_msg() << ends;
			exit(1);
		}
	}

	void Fill(const char *dbn, const char* root)
	{
		cout << "Load folders" << endl;
		sql3::database dbs(dbn);
		folder last_visited{ "", 0 };
		insert_muli im(dbs, "INSERT INTO Files(folder_id,file,type,size) VALUES", 10000);
		for (fs::recursive_directory_iterator i(root); i != fs::recursive_directory_iterator(); ++i)
		{
			if (fs::is_directory(i->path())) {
				last_visited.CheckDir(dbs, i->path());
			}
			else {
				ostringstream sql;
				sql << boost::format("SELECT file_id FROM Files WHERE file = '%1%' AND folder_id=%2%") % DupApos((*i).path().filename().string())%last_visited.id_;
				sql3::query q(dbs, sql.str().c_str());
				if (q.begin() != q.end()) continue;
				sql.str("");
				sql << boost::format("(%1%,'%2%','%3%',%4%)") %last_visited.id_ % DupApos(i->path().filename().string())%DupApos(i->path().extension().string())%
				file_size(i->path());
				im.Add(sql.str());
			}
		}
		im.Flush();
	}

	void CalcCrc(const char* dbn, const char* root)
	{
		sql3::database dbs(dbn);
		cout << "Calculate md5" << endl;
		long long total, curnt{ 1 };
		{
			sql3::query cnt(dbs, 
"SELECT count(*) FROM Folders fd \n\
JOIN Files fl ON (fd.folder_id = fl.folder_id) \n\
JOIN Types ON(type=fl.type) WHERE crc IS NULL");
			sql3::query::iterator rec = cnt.begin();
			GetFieldVal(rec, 0, total);
		}
		if (!total) return;
		auto start = chrono::high_resolution_clock::now();
		sql3::query q(dbs, 
"SELECT folder||'\\'||file, size, file_id FROM Folders fd \n\
JOIN Files fl ON (fd.folder_id = fl.folder_id) \n\
JOIN Types ON(types=fl.type) WHERE crc IS NULL");
		cout << "Total: "<<PrettyNum(total) << endl;
		for (sql3::query::iterator i = q.begin(); i != q.end(); ++i,++curnt) {
			string p;
			size_t len;
			long id;
			GetFieldVal(i, 0, p);
			GetFieldVal(i, 1, len);
			GetFieldVal(i, 2, id);
			ostringstream sql;
			sql << boost::format("UPDATE Files SET crc='%1%' WHERE file_id=%2%") % Calc(p,len) % id;
			dbs.execute(sql.str().c_str());
			std::chrono::duration<double, std::milli> fp_ms = chrono::high_resolution_clock::now() - start;
			cout<<'\r'<<string(50,' ')<<"\r" << PrettyNum(curnt) << '\t' << fp_ms.count() / 1000.<<'\t'<< (fp_ms.count()/1000/curnt)*(total-curnt);
		}
	}
	void FindDoubles(const char* dbn)
	{
		sql3::database dbs(dbn);
		cout << "Find doubles" << endl;
		sql3::query q(dbs, "SELECT crc, COUNT(*) FROM files WHERE crc IS NOT NULL GROUP BY folder_id, size, crc HAVING COUNT(*) >1");
		for (sql3::query::iterator i = q.begin(); i != q.end(); ++i) {
			string crc;
			GetFieldVal(i, 0, crc);
			ostringstream sql;
			sql << boost::format("SELECT folder||'\\'||file, file_id FROM Folders fd JOIN Files fl ON (fd.folder_id = fl.folder_id) WHERE crc = '%1%'")
				% crc;
			sql3::query f(dbs, sql.str().c_str());
			for (sql3::query::iterator j = f.begin(); j != f.end(); ++j) {
				string p;
				GetFieldVal(j, 0, p);
				int file_id;
				GetFieldVal(j, 1, file_id);
				cout << p << endl;
			}
			cout << endl;
		}
	}

}