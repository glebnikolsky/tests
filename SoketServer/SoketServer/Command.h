#pragma once
#include <tuple>
#include <string>

enum class Command{
	by_query = 1,  
	exec_info,
	stop = 10,
	db_info = 11,
	db_list = 12,
	unknown = 100
};

enum class Result {
	need_check = 0,

	OK = 1,
	queue,
	info,

	query_empty = 10,
	bad_pdf = 11,
	bad_args,


	invalid_client = 100,
	lost_db_connection,
	no_range,
	has_dubles,
	press_orders_blocked,
	unknown_command,
	bad_command,
	bad_xml
};

std::tuple<Command, Result,  std::string, std::string, int , int > decode_cmd(TCHAR* cmd);
std::string get_answer(Result res);
std::string make_answer(TCHAR* data_, size_t &length, int &delay);
