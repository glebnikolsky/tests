#include "stdafx.h"
#include <string>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include "Guid.h"

Guid::Guid() : val_(boost::uuids::nil_uuid()) {}

Guid::Guid(const Guid &g) : val_(g.val_) {}

Guid::Guid(const TCHAR *str) {
	if (!*str) val_ = boost::uuids::nil_uuid(); //пустой из пустой строки
	else {
		boost::uuids::string_generator gen;
		val_ = gen(str);
	}
}

//Guid::Guid(const CGuid &cg)
//{
//	boost::uuids::string_generator gen;
//	val_ = gen(((std::string)(CString)cg.GetString()));
//}

Guid::operator std::string() const
{
	std::string s = boost::uuids::to_string(val_);
	boost::to_upper(s);
	return s;
}

//Guid::operator CString() const
//{
//	std::string tmp = (std::string)*this;
//	return CString(tmp.c_str());
//}

//Guid::operator CGuid() const {	return CGuid((CString)*this); }

bool Guid::is_nil() const {		return val_.is_nil(); }

bool Guid::operator == (const Guid &r) const { return val_ == r.val_; }

bool Guid::operator == (const TCHAR*r) const { return *this == Guid(r); }

bool Guid::operator < (const Guid &r) const { return val_ < r.val_; }

bool Guid::operator > (const Guid &r) const { return val_ > r.val_; }

bool Guid::operator <= (const Guid &r) const { return val_ <= r.val_; }

bool Guid::operator >= (const Guid &r) const {	return val_ >= r.val_; }

const Guid& Guid::operator = (const Guid& r) 
{
	val_ = r.val_;
	return *this;
}

const Guid& Guid::operator = (const TCHAR *str) {	return *this = Guid(str); }

//const Guid& Guid::operator = (const CGuid &g) { return *this = Guid(g); }

//std::ostream& operator << (std::ostream& os, const Guid& g) 
//{
//	os << (std::string)g;
//	return os;
//}
//
