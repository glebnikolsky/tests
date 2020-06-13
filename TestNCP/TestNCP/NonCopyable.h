//	Файл:		NonCopyable.h
//	Описание:	Вспомогательные классы и методы  
//	(с) 2011 Федеральный центр тестирования

#pragma once
#ifndef __FTCUTIL_NONCOPYABLE_H_INCLUDED
#define __FTCUTIL_NONCOPYABLE_H_INCLUDED

//	Класс:		_non_copyable_t
//	Описание:	Базовый класс для некопируемых объектов
class _non_copyable_t
{

//	конструктор / деструктор
public:

	//	конструктор
	_non_copyable_t() { }
	//	деструктор
	virtual ~_non_copyable_t() { }

//	операторы 
private:

	//	конструктор
	_non_copyable_t(const _non_copyable_t& source);
	//	оператор присваивания
	_non_copyable_t& operator = (const _non_copyable_t& source);

};

#endif
