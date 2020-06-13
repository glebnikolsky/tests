#include "stdafx.h"
#include <string>
#include <vector>
#include <set>
#include "selector.h"



bool Selector::Open(std::string flnm)
{
	if (!flnm.empty()) filename_ = flnm;
	char modulename[1000];
	::GetModuleFileNameA(NULL, modulename, 1000);
	std::string cur_dir = modulename;
	int pos = cur_dir.rfind('\\');
	cur_dir = cur_dir.substr(0, pos + 1);
	filename_ = cur_dir + filename_;
	parse_result_= doc_.load_file(filename_.c_str());
	return (bool)parse_result_;
}

bool Selector::ResetSelection(const char *q)
{
	pugi::xpath_query query(q);
	selectors_ = query.evaluate_node_set(doc_);
	return selectors_.size() != 0;
}

std::vector<std::string> Selector::GetNames()
{
	std::vector<std::string> res;

	for (auto i : selectors_) res.push_back(i.node().attribute("name").as_string());
	return res;
}

std::string Selector::GetSql(int no)
{
	return selectors_[no].node().first_child().value();
}

std::string Selector::GetAddFunc(int no)
{
	return selectors_[no].node().attribute("addfunc").as_string();
}

std::set<std::string> Selector::GetRequired(int no)
{
	std::set<std::string> res;
	std::string s = selectors_[no].node().attribute("required").as_string();
	while (!s.empty()) {
		int pos = s.find(';');
		if (pos  > 0) {
			res.insert(s.substr(0, pos));
			s = s.substr(pos + 1);
		}
		else {
			res.insert(s.substr(0));
			s = std::string();
		}
	}
	return res;
}

std::string Selector::GetComboName(const char* q)
{
	pugi::xpath_query query(q);
	pugi::xpath_node_set res = query.evaluate_node_set(doc_);

	return res[0].node().attribute("comboheader").as_string();
}