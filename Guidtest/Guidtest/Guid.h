/*!
*   \file Guid.h
*/
//	Файл:		Guid.h
//	Описание:	Вспомогательные классы и методы  
//	(с) 2011 - 2014 Федеральный центр тестирования

#pragma once
#ifndef __FTCUTIL_GUID_H_INCLUDED
#define __FTCUTIL_GUID_H_INCLUDED
#define __FTCUTIL_CLASS

// Этот класс хранит guid в компактном виде, не как строку
struct __FTCUTIL_CLASS Guid {
	Guid();
	Guid(const Guid &g);
	Guid(const TCHAR *str);
//	Guid(const CGuid &cg);

	operator std::string() const;
//	operator CString() const;
//	operator CGuid() const;

	bool is_nil() const;

	bool operator == (const Guid &r) const;
	bool operator == (const TCHAR*r) const;
	bool operator < (const Guid &r) const;
	bool operator > (const Guid &r) const;
	bool operator <= (const Guid &r) const;
	bool operator >= (const Guid &r) const;

	const Guid& operator = (const Guid& r);
	const Guid& operator = (const TCHAR *str);
//	const Guid& operator = (const CGuid &g);
	template <class Ostream>	friend Ostream& operator << (Ostream& os, const Guid& g)
	{
		os << (std::string)g;
		return os;
	}
	template <class Istream>	friend Istream& operator >> (Istream& is, Guid& g)
	{
		std::string s;
		is >> s;
		g = Guid(s.c_str());
		return is;
	}
	//friend istream& operator>>(istream& is, Guid& g);
	
private:
	boost::uuids::uuid val_;
};
#endif
