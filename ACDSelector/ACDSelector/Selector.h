#pragma once


struct Selector {
	Selector(const char *flnm = "\0"): filename_(flnm){}
	~Selector() {}
	bool Open(std::string f = std::string());

	std::vector<std::string> GetNames();
	std::string GetSql(int no);
	std::set<std::string> GetRequired(int no);
	std::string GetAddFunc(int no);
	bool ResetSelection(const char *query);
	std::string GetComboName(const char* query);
	bool IsOpen() { return (bool)parse_result_; }
	const char* GetError() { return parse_result_.description(); }
	std::string GetFileName() { return filename_; }

protected:
	std::string filename_;
	pugi::xml_document doc_;
	pugi::xml_parse_result parse_result_;
	pugi::xpath_node_set selectors_;
};
