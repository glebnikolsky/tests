#include "stdafx.h"
#include "Command.h"
#include "pugixml.hpp"
#include <map>



namespace {
	std::map<std::string, Command > commands{
		{ "by query",  Command::by_query},
		{ "stop", Command::stop},
		{ "db_info", Command::db_info},
		{ "db_list", Command::db_list},
		{ "exec info", Command::exec_info } };

	std::map<Result, std::string> answers{
		{Result::OK, "<answer>\n\t<result  state=\"OK\" loadid=\"123\" count=\"123\" queue=\"150\"/>\n\t<explain>исполняется</explain>\n</answer>" },
		{ Result::queue,"<answer>\n\t<result  state=\"OK\" loadid=\"123\" count=\"123\" queue=\"150\"/>\n\t<explain>поставлена в очередь</explain>\n</answer>"},
		{ Result::info, "<answer>\n\t<result  state=\"OK\" loadid=\"123\" count=\"123\" queue=\"150\"/>\n\t<explain/>\n</answer>" },
		{ Result::query_empty,"<answer>\n\t<result  state=\"error\" />\n\t<explain>нет не исполненных заявок</explain>\n</answer>"},
		{ Result::bad_pdf,"<answer><result  state=\"error\" />\n\t<explain>для заказа отсутствует pdf файл</explain>\n</answer>"},
		{ Result::invalid_client,"<answer>\n\t<result  state=\"error\" />\n\t<explain>плохой источник заказа</explain>\n</answer>" },
		{ Result::bad_args,"<answer>\n\t<result  state=\"error\" />\n\t<explain>не все атрибуты указаны</explain>\n</answer>" },
		{ Result::unknown_command,"<answer>\n\t<result  state=\"error\" />\n\t<explain>не все атрибуты указаны</explain>\n</answer>" },
		{ Result::bad_command,"<answer>\n\t<result  state=\"error\" />\n\t<explain>неправильная команда</explain>\n</answer>" },
		{ Result::bad_xml,"<answer>\n\t<result  state=\"error\" />\n\t<explain>плохой xml</explain>\n</answer>" },
		{ Result::no_range, "<answer>\n\t<result  state = \"error\" />\n\t<explain>исчерпан диапазон штрихкодов</explain>\n</answer>" },
		{ Result::has_dubles, "<answer>\n\t<result  state = \"error\" />\n\t<explain>дублируются штрихкоды</explain>\n</answer>" },
		{ Result::lost_db_connection, "<answer>\n\t<result  state = \"error\" />\n\t<explain>утеряно соединение с базой данных</explain>\n</answer>" },
		{ Result::press_orders_blocked, "<answer>\n\t<result  state = \"error\" />\n\t<explain>таблица PressOrders блокирована более 10-ти минут</explain>\n</answer>" }
		};

}

std::string get_answer(Result res)
{
	return answers[res];
}

std::string make_answer(TCHAR* data_, size_t &length, int &delay)
{
	data_[length] = '\0';
	std::string s;
	std::tuple<Command, Result, std::string, std::string, int, int >res = decode_cmd(data_);
	if (std::get<1>(res) != Result::need_check) {
		s = get_answer(std::get<1>(res));
	
	}
	else {
		switch (std::get<4>(res)) {
		case 0:	s = get_answer(Result::OK); break;
		case 1: s = get_answer(Result::queue); break;
		case 2:	s = get_answer(Result::query_empty); break;
		case 3: s = get_answer(Result::bad_pdf); break;
		case 4:	s = get_answer(Result::invalid_client); break;
		}
	}
	delay = std::get<5>(res);
	return s;
}

std::tuple<Command, Result, std::string, std::string, int , int > decode_cmd(TCHAR* cmd)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result;
	result = doc.load_string(cmd);
	if (!result) 
		return std::make_tuple(Command::unknown, Result::bad_xml, "", result.description(),0,0);
	pugi::xpath_query taskq("/task");
	pugi::xpath_node_set task = taskq.evaluate_node_set(doc);
	if ( !task.size() ) 
		return std::make_tuple(Command::unknown, Result::bad_command, "", "не указан тег task",0,0);
	if (task[0].node().attribute("cmd").as_string() == "")
		return std::make_tuple(Command::unknown, Result::bad_command, "", "не указан атрибут cmd",0,0);
	if (task[0].node().attribute("sender").as_string() == "" )
		return std::make_tuple(Command::unknown, Result::bad_command, "", "не указан атрибут sender",0,0);
	if (commands[task[0].node().attribute("cmd").as_string()] == Command::exec_info ) return std::make_tuple(commands[task[0].node().attribute("cmd").as_string()], Result::info, task[0].node().attribute("sender").as_string(), "", 0, 0);
	else return std::make_tuple(commands[task[0].node().attribute("cmd").as_string()], Result::need_check, task[0].node().attribute("sender").as_string(), "", task[0].node().attribute("debug").as_int(), task[0].node().attribute("delay").as_int());
}



