#include "stdafx.h"


#include "GSelector.h"

#include <boost/tokenizer.hpp>


GSelector::GSelector()
{
}


GSelector::~GSelector()
{
}

boost::optional<std::string> GSelector::Init( std::string filename)
{
	pugi::xml_parse_result result;
	result = doc_.load_file( filename.c_str() );
	if ( result.status ) return result.description();
	else return boost::none;
}

std::vector<std::string> GSelector::GetGSelectors()
{
	pugi::xpath_query query( "/Glue/GSelector" );
	pugi::xpath_node_set res = query.evaluate_node_set( doc_ );
	std::vector<std::string> tmp;
	for ( auto i : res ){
		tmp.push_back( i.node().attribute( "name" ).as_string() );
	}
	return tmp;
}

std::pair< bool, std::string> GSelector::FillGSelectorData( int no )
{
	pugi::xpath_query query( "/Glue/Selector" );
	auto res = query.evaluate_node_set( doc_ );
	sql_ = res[no].node().child_value("sql");
	std::string tag_name = "FieldsNames";
	auto node = res[no].node().child( tag_name.c_str() );
	if ( !node || !node.attribute( "names" ) ) return std::make_pair( false, "ќтсутствует об€зательный тег или атрибут " + tag_name );
	std::string str = node.attribute( "names" ).as_string();
	boost::char_separator<char> field_sep( _T( ";" ), _T( "" ), boost::keep_empty_tokens );
	{
		boost::tokenizer<boost::char_separator<char> > field_tokens( str, field_sep );
		field_names_.clear();
		for ( auto &i : field_tokens ) field_names_.push_back( i );
	}
	tag_name = "Server";
	node = res[no].node().child( tag_name.c_str() );
	if ( node ){
		server_name_ = node.attribute( "name" ).as_string();
		pwd_ = node.attribute( "pwd" ).as_string();
	}

	tag_name = "FieldsTypes";
	node = res[no].node().child( tag_name.c_str() );
	if ( !node || !node.attribute( "names" ) ) return std::make_pair( false, "ќтсутствует об€зательный тег или атрибут " + tag_name );
	str = node.attribute( "names" ).as_string();
	{
		boost::tokenizer<boost::char_separator<char> > field_tokens( str, field_sep );
		field_types_.clear();
		for ( auto &i : field_tokens ) field_types_.push_back( i );
		if ( field_names_.size() != field_types_.size() ) return std::make_pair( false, " оличество полей и типов не совпадает" );
	}
	tag_name = "FieldsAligns";
	node = res[no].node().child( tag_name.c_str() );
	if ( !node || !node.attribute( "names" ) ) return std::make_pair( false, "ќтсутствует об€зательный тег или атрибут " + tag_name );
	str = node.attribute( "names" ).as_string();
	{
		boost::tokenizer<boost::char_separator<char> > field_tokens( str, field_sep );
		field_aligns_.clear();
		for ( auto &i : field_tokens ) field_aligns_.push_back( i );
		if ( field_names_.size() != field_aligns_.size() ) return std::make_pair( false, " оличество полей и выравниваний не совпадает" );
	}
	tag_name = "Vars";
	node = res[no].node().child( tag_name.c_str() );
	if ( !node || !node.attribute( "vars" ) ) return std::make_pair( false, "ќтсутствует об€зательный тег или атрибут " + tag_name );
	str = node.attribute( "vars" ).as_string();
	{
		boost::tokenizer<boost::char_separator<char> > field_tokens( str, field_sep );
		int no{0};
		vars_.clear();
		for ( auto i = begin( field_tokens ); i != end( field_tokens );++i, ++no ) vars_[ *i] = no;
		if ( !vars_.size() ) return std::make_pair( false, "Ќе указана ни одна переменна€" );
	}
	pugi::xpath_query query1( "Menu/MenuItem" );
	res = query1.evaluate_node_set( res[no].node() );
	if ( !res.size() ) return std::make_pair( false, "отсутствуют MenuItem" );
	for ( auto nd : res ){
		menu_items_names_.push_back( nd.node().attribute( "name" ).as_string() );
		menu_items_sql_.push_back(nd.node().child_value( "sql" ) );
	}

	return std::make_pair( true,"");

}

std::vector<std::string>& GSelector::GetGSelectorMenu()
{
	return menu_items_names_;
}

std::string GSelector::GetMenuItemSQL( int no )
{
	return menu_items_sql_[no];
}
