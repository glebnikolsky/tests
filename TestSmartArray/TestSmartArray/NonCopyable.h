/*!
*   \file NonCopyable.h
*/
//	Файл:		NonCopyable.h
//	Описание:	Вспомогательные классы и методы  
//	(с) 2011 - 2014 Федеральный центр тестирования

#pragma once
#ifndef __FTCUTIL_NONCOPYABLE_H_INCLUDED
#define __FTCUTIL_NONCOPYABLE_H_INCLUDED

/*!
*	\class _non_copyable_t
*   \brief Базовый класс для некопируемых объектов
*/
class __FTCUTIL_CLASS _non_copyable_t
{

//	конструктор / деструктор
public:

	/*!	\brief конструктор по умолчанию*/
	_non_copyable_t() { }
	/*! \brief деструктор*/
	virtual ~_non_copyable_t() { }

//	операторы 
private:
    /*! \brief конструктор*/
    _non_copyable_t(const _non_copyable_t& source);
	// оператор присваивания
    _non_copyable_t& operator = (const _non_copyable_t& source){return *this;}


};

#endif
