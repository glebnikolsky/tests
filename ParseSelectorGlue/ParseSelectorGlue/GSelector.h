#pragma once
#include <boost\optional.hpp>
class GSelector
{
public:

	GSelector();
	~GSelector();
	
	boost::optional<std::string> Init( std::string filename );
	std::vector<std::string> GetGSelectors();
	std::pair< bool, std::string> FillGSelectorData(int no);
	std::vector<std::string>& GetGSelectorMenu();
	std::string GetMenuItemSQL( int no );
	std::vector<std::string> GetFieldNames() const
	{
		return field_names_;
	}
	std::vector<std::string> GetFieldTypes() const
	{
		return field_types_;
	}
	std::vector<std::string> GetFieldAligns() const
	{
		return field_aligns_;
	}
	std::map<std::string, int> GetVars() const
	{
		return vars_;
	}

private:
	pugi::xml_document doc_;
	std::string name_;
	std::string server_name_;
	std::string pwd_;
	std::string sql_;
	std::vector<std::string> field_names_;
	std::vector<std::string> field_types_;
	std::vector<std::string> field_aligns_;
	std::map<std::string, int> vars_;
	std::vector < std::string> menu_items_names_;
	std::vector < std::string> menu_items_sql_;
	pugi::xpath_node_set menuitems_;
};

