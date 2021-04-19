//// SoketServer.cpp : Defines the entry point for the console application.
////
//
#include "stdafx.h"
//#include <cstdlib>
//#include <iostream>
//#include <memory>
//#include <utility>
//#include <boost/asio.hpp>
//#include <boost/algorithm/string/trim.hpp>
//
//
//using boost::asio::ip::tcp;
//using boost::asio::steady_timer;
//using boost::asio::deadline_timer;
//
//bool stop_ = false;
//
//
//class session
//	: public std::enable_shared_from_this<session>
//{
//public:
//	session(tcp::socket socket)
//		: socket_(std::move(socket)), command_(false)
//	{
//	}
//
//	void start(bool cmd)
//	{
//		command_ = cmd;
//		do_read();
//	}
//
//private:
//	void do_read()
//	{
//		auto self(shared_from_this());
//		std::ostream os(&buffer_);
//		os.flush();
//		boost::asio::async_read(socket_, buffer_, 
//			[this, self](boost::system::error_code ec, std::size_t length)
//		{
//			if (!ec)
//			{
//				std::istream is(&buffer_);
//				std::string readed_;
//				std::getline(is, readed_);
//				std::cout << socket_.remote_endpoint().address() << '\n';
//				boost::trim(readed_);
//				do_write(readed_);
//			}
//		});
//	}
//
//	void do_write(std::string s)
//	{
//		auto self(shared_from_this());
//		if (s != "quit") {
//			s = (command_ ? "\t>>" : "\t\t::") + s + "\r\n";
//		}
//		else {
//
//			s = "Bye-bye!\r\n";
//			stop_ = true;
//		}
//		boost::asio::async_write(socket_, boost::asio::buffer(s.c_str(), s.length()),
//			[this, self](boost::system::error_code ec, std::size_t /*length*/)
//		{
//			if (!ec)
//			{
//				if (!stop_) do_read();
//			}
//		});
//	}
//
//	tcp::socket socket_;
//	boost::asio::streambuf buffer_;
//
//	bool command_;
//};
//
//class server
//{
//public:
//	server(boost::asio::io_context& io_context, short port, bool cmd = false)
//		: acceptor_(io_context, tcp::endpoint(tcp::v4(), port)), command_(cmd), io_context_(io_context)
//	{
//		do_accept();
//	}
//private:
//	void do_accept()
//	{
//		boost::asio::deadline_timer t(io_context_, boost::posix_time::seconds(5));
//		acceptor_.async_accept(
//			[this](boost::system::error_code ec, tcp::socket socket)
//		{
//			if (!ec)
//			{
//				std::make_shared<session>(std::move(socket))->start(command_);
//			}
//			else {
//				boost::system::error_code eec = ec;
//			}
//			if ( !stop_ ) do_accept();
//			else io_context_.stop();
//		});
//	}
//	
//
//	tcp::acceptor acceptor_;
//	bool command_;
//
//	boost::asio::io_context&  io_context_;
//};
//
//int main(int argc, char* argv[])
//{
//	try
//	{
//
//		boost::asio::io_context io_context;
//
//		server s(io_context, 1010, true);
//		server s1(io_context, 1488);
//		server s2(io_context, 1489);
//
//
//
//		io_context.run();
//	}
//	catch (std::exception& e)
//	{
//		std::cerr << "Exception: " << e.what() << "\n";
//	}
//
//	return 0;
//}
//
// async_tcp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
#include "Command.h"

#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <sstream>
#include <tuple>
#include <utility>
#include <boost/asio.hpp>
#include <boost/tokenizer.hpp>
#include <vector>


namespace {
	std::mutex press_order_mutex;
	std::vector<long> v{ 1,2, 3 };
}

using boost::asio::ip::tcp;

class session
	: public std::enable_shared_from_this<session>
{
public:
	session(tcp::socket socket, boost::asio::io_context& io_context)
		: socket_(std::move(socket)), io_(io_context)
	{
	}

	void start()
	{
		do_read();
	}

private:
	void do_read()
	{
		auto self(shared_from_this());
		socket_.async_read_some(boost::asio::buffer(data_, max_length),
				[this, self](boost::system::error_code ec, std::size_t length)
		{
			if (!ec)
			{
				data_[length] = '\0';
				std::cout << ">>" << data_ << "\n";
				int delay{ 0 };
				std::string s = make_answer(data_, length, delay);
				if (delay) {
					boost::asio::steady_timer t(io_, boost::asio::chrono::seconds(delay));
					t.wait();
				}
				strncpy_s(data_, max_length, s.c_str(), s.length());
				data_[s.length()] = '\0';
				length = s.length();// +1;
				std::cout << "<<" <<data_ << "\n";
				do_write(length);
			}
		});
	}

	void do_write(std::size_t length)
	{
		auto self(shared_from_this());
		boost::asio::async_write(socket_, boost::asio::buffer(data_, length),
			[this, self](boost::system::error_code ec, std::size_t len)
		{
			if (!ec)
			{
				std::cout << socket_.remote_endpoint().address() << '\n';
			}
			else {
				std::cout << ec.message();
			}
		});
	}

	tcp::socket socket_;
	enum { max_length = 102400 };
	char data_[max_length];
	boost::asio::io_context& io_;

};

class server
{
public:
	server(boost::asio::io_context& io_context, short port)
		: acceptor_(io_context, tcp::endpoint(tcp::v4(), port)), io_(io_context)
	{
		do_accept();
	}

private:
	void do_accept()
	{
		acceptor_.async_accept(
			[this](boost::system::error_code ec, tcp::socket socket)
		{
			if (!ec)
			{
				std::make_shared<session>(std::move(socket), io_)->start();
			}

			do_accept();
		});
	}

	tcp::acceptor acceptor_;
	boost::asio::io_context& io_;
};

class DoIt {
public:
	DoIt(boost::asio::io_context& io_context, short port):io_(io_context), port_(port)
	{
		doit();
	}

private:
	void doit()
	{
		for (; ;) {
			if (press_order_mutex.try_lock()) {
				if (v.size()) {
					long l = v[0];
					press_order_mutex.unlock();
					std::lock_guard<std::mutex> lock(press_order_mutex);
					v.erase(begin(v));
				}
				else {
					press_order_mutex.unlock();
					boost::asio::steady_timer t(io_, boost::asio::chrono::seconds(50));
					t.wait();
				}
			}
			else {
				using namespace std::chrono_literals;
				std::this_thread::sleep_for(500ms);
			}
		}

	}

private:
	long port_;
	boost::asio::io_context& io_;
};


class tmr {
public:
	tmr(boost::asio::io_context& io_context) : io_(io_context)
	{
		wait_sync();
	}

private:
	void wait_sync()
	{
		
		for (;;) {
			boost::asio::steady_timer t(io_, boost::asio::chrono::seconds(5));
			t.wait();
			std::cout << "*\n";
		}
	}

	boost::asio::io_context& io_;
};







int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "ru-RU");
	try
	{
		if (argc != 2)
		{
			std::cerr << "Usage: async_tcp_echo_server <port>\n";
			return 1;
		}
		std::stringstream sqls;
		sqls << "1,2,3,4,5,6,7,8,9";
		int n;
		while (sqls>>n)
		{
			std::cout << n << "\n";
		}
		long l = 3;
		l &= ~1;
		l &= ~2;

		//TCHAR data_[1000] = "<task sender=\"4DAEBF0E-ADF4-4BB3-A0DF-0016DA3591BF\" cmd=\"by query\" debug=\"2\" delay=\"30\">\r\n\
		//	<order technology=\"6\" date=\"2021.03.15\" tom= \"2\" subject=\"1, 2, 3, 10, 31\" />\r\n\
		//	</task>";
		TCHAR data_[1000] = "<task sender=\"4DAEBF0E-ADF4-4BB3-A0DF-0016DA3591BF\" cmd=\"by query\" debug=2 delay=\"30\">\r\n\
			<order technology=\"6\" date=\"2021.03.15\" tom= \"2\" subject=\"1, 2, 3, 10, 31\" />\r\n\
			</task>";
		size_t len = strlen(data_);
		int delay;
		std::string ss = make_answer(data_, len, delay);
		std::cout << ss<<"\n"<<delay;



		boost::asio::io_context io_context;

		server s(io_context, std::atoi(argv[1]));
		server s1(io_context, 1489);
		//DoIt	d(io_context, std::atoi(argv[1]));

//		tmr t(io_context);
		io_context.run();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}

