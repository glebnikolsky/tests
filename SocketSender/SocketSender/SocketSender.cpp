// SocketSender.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

enum { max_length = 10240 };




int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "ru-RU");
	try
	{
		if (argc != 3)
		{
			std::cerr << "Usage: blocking_tcp_echo_client <host> <port>\n";
			return 1;
		}

		boost::asio::io_context io_context;
		tcp::socket s(io_context);
		tcp::resolver resolver(io_context);
		boost::asio::connect(s, resolver.resolve(argv[1], argv[2]));


		std::cout << "Enter message: ";

		std::string request;
		for (;;) {
			std::string line;
			getline(std::cin, line);
			if (line == "###") break;
			request += line + "\n";
		}
		char data[max_length];
		strncpy_s(data, max_length, request.c_str(), request.length());
		//auto self(shared_from_this());
		boost::asio::async_write(s, boost::asio::buffer(data, request.length()),
			[&](boost::system::error_code ec, std::size_t len)
		{
			if (ec) std::cout << ec.message();
			
		});
		s.async_read_some(boost::asio::buffer(data, max_length),
			[&](boost::system::error_code ec, std::size_t length)
		{
			if (!ec)
			{
				std::cout << "Reply is: ";
				data[length] = '\0';
				std::cout << ">>" << data << "\n";
				std::cout << "<<" << data << "\n";
			}
		});
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}
