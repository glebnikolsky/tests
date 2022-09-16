// Signals.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <csignal>
#include <iostream>


void signal_handler( int signal )
{
	switch ( signal ){
	case SIGTERM: std::cout << "SIGTERM"; break;
	case SIGSEGV: std::cout << "SIGSEGV"; break;
	case SIGINT: std::cout << "SIGINT"; break;
	case SIGILL: std::cout << "SIGILL"; break;
	case SIGABRT: std::cout << "SIGABRT"; break;
	case SIGFPE: std::cout << "SIGFPE"; break;
	}
//	std::abort();
}

int main()
{
	std::signal( SIGINT, signal_handler );
	//std::signal( SIGABRT, signal_handler );
	//std::signal( SIGTERM, signal_handler );
	//std::raise( SIGINT );
	int n;
	std::cin >> n;

    return 0;
}

