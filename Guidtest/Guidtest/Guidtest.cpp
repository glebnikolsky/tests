// Guidtest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <string>
#include <sstream>
#include <set>
#include <vector>
#include <algorithm>
#include <boost/uuid/uuid.hpp>
#include "guid.h"

struct RecheckG {
	RecheckG() :id_(Guid()), region_(0), state_(-1), reg_(true) {}
	RecheckG(const TCHAR *id, const TCHAR *region, int state, bool r) : id_(id), region_(atol(region)), state_(state), reg_(r) {}
	RecheckG(const Guid id, const long region, int state, bool r) : id_(id), region_(region), state_(state), reg_(r) {}

	Guid id_;
	long region_;
	int state_;
	bool reg_;
	
	bool operator == (const RecheckG &r) {
		return id_ == r.id_ && region_ == r.region_ && state_ == r.state_ && reg_ == r.reg_;
	}
	
	bool operator != (const RecheckG &r) {
		return !(*this == r);
	}

	template <class Ostream>	friend Ostream& operator << (Ostream& os, const RecheckG& g)
	{
		os << g.id_ << ' ' << g.region_ << ' ' << g.state_ << ' ' << g.reg_;
		return os;
	}
	template <class Istream>	friend Istream& operator >> (Istream& is, RecheckG& g)
	{
		is >> g.id_ >> g.region_ >> g.state_ >> g.reg_;
		return is;
	}
};


int main()
{
	setlocale(LC_ALL, "ru-RU");
	using namespace std;
	vector<Guid> a, b;
	a.push_back(Guid("626A2159-57BA-4CA7-9CDB-276B796D620F"));
	a.push_back(Guid("626A2159-57BA-4CA7-9CDB-276B796D620A"));
	b.push_back(Guid("626A2159-57BA-4CA7-9CDB-276B796D620F"));
	a.push_back(Guid("726A2159-57BA-4CA7-9CDB-276B796D620A"));
	vector<Guid> res(4);
	sort(begin(a), end(a));
	sort(begin(b), end(b));

	auto e = set_symmetric_difference(begin(a), end(a), begin(b), end(b), begin(res));


	Guid g("626A2159-57BA-4CA7-9CDB-276B796D620F");
	std::cout << g << '\t' << (g == "626A2159-57BA-4CA7-9CDB-276B796D620F") << '\t';
	std::istringstream is("626A2159-57BA-4CA7-9CDB-276B796D620F");
	Guid gg;
	std::cout << gg << '\t';
	is >> gg;
	std::cout << gg << '\t' << (gg == g);
	RecheckG in("626A2159-57BA-4CA7-9CDB-276B796D620F", 99, 1, 1), in1("626A2159-57BA-4CA7-9CDB-276B796D620F", 77, 1, 1),
		in2("626A2159-57BA-4CA7-9CDB-276B796D620F", 99, 0, 1), in3("626A2159-57BA-4CA7-9CDB-276B796D620F", 99, 1, 0), o, o1, o2, o3;
	std::cout << in << '\n' << in1 << '\n' << in2 << '\n' << in3 << '\n';
	std::stringstream ss;
	ss<< in << '\n' << in1 << '\n' << in2 << '\n' << in3 << '\n';
	ss >> o >> o1 >> o2 >> o3;
	std::cout << o << '\n' << o1 << '\n' << o2 << '\n' << o3 << '\n';
	

	_ASSERT(in == o);
	_ASSERT(in1 == o1);
	_ASSERT(in2 == o2);
	_ASSERT(in3 == o3);
	ss.str("№12 от 08.04.2020\n626A2159-57BA-4CA7-9CDB-276B796D620F 111 0 1");
	std::string str;
	std::getline(ss, str);
	ss >> o;
	std::cout << o;
	//std::stringstream sss;
	ss.clear();
	ss.str("111111111111111111 1");
	std::cout << ss.str();
	if (!ss.eof())
		std::getline(ss, str);
	if ( !ss.eof())
		std::getline(ss, str);

    return 0;
}

