#include "stdafx.h"
#include "epubinfo.h"
#include "zip.h"
#include "unzip.h"
#include "pugixml.hpp"

#include <boost/algorithm/string.hpp>

using namespace std;

namespace {
	pugi::xml_document doc;
	bool xml_loaded{ false };

	bool OpenDoc(const char* path) {
		if (loaded) return true;
		HZIP hz = OpenZip(path, nullptr);
		if (!hz) {
			cerr << "OOps!\n";
			return false;
		}
		ZIPENTRY ze;
		GetZipItem(hz, -1, &ze);
		int numitems = ze.index;
		for (int i = 0; i < numitems; ++i) {
			GetZipItem(hz, i, &ze);
			boost::to_lower(ze.name);
			if (!boost::ends_with(ze.name, "content.opf")) continue;
			std::cout << std::string(ze.name) << '\t' << ze.unc_size << '\n';
			std::vector<TCHAR> buf(ze.unc_size + 1, '\0');
			UnzipItem(hz, i, &buf[0], ze.unc_size);
			pugi::xml_parse_result result = doc.load_buffer(&buf[0], buf.size());
			if (result) {
				loaded = true;
				return true;
			}
		}
		return false;
	}

	std::vector<std::string>& GetAuthors(const char* path)
	{
		if (!loaded) OpenDoc(path);

		std::vector<std::string> res;
		return res;
	}

	std::vector<std::string>& GetGenres(const char* path) 
	{
		if (!loaded) OpenDoc(path);

		std::vector<std::string> res;
		return res;

	}

	std::vector<std::string>& GetTitles(const char* path)
	{
		if (!loaded) OpenDoc(path);

		std::vector<std::string> res;
		return res;
	}
}

namespace EpubInfo {
	void EpubInfo(sql3::database& dbs, int file_id, string p) {

	}



}