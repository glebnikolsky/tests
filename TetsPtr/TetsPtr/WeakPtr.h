/*! 
*   \file WeakPtr.h
*/
//	Файл:		WeakPtr.h
//	Описание:	Вспомогательные классы и методы  
//	(с) 2011 - 2014 Федеральный центр тестирования

#pragma once
#ifndef __FTCUTIL_WEAKPTR_H_INCLUDED
#define __FTCUTIL_WEAKPTR_H_INCLUDED

/*!
*	\class _weak_ptr_t
*	\brief Шаблон слабого указателя
*/
template <class TYPE> class _weak_ptr_t
{

//	свойства
protected:

	/*! \brif дружественные классы*/
    template<class TYPE> friend class _smart_ptr_t;

	
	typedef TYPE* PTRTYPE; //!< указатель на тип
	typedef TYPE& REFTYPE; //!< ссылка на тип

	
	PTRTYPE m_object; //!<   указатель на объект
	
	_ref_counter_t<TYPE>* m_counter;    //!<	указатель на объект подсчета ссылок 

//	конструктор / деструктор
public:

	/*! \brief	конструктор по умолчанию*/
	_weak_ptr_t() : m_object(NULL), m_counter(NULL) { }

	/*! \brief конструктор*/
	_weak_ptr_t(const _smart_ptr_t<TYPE>& source) :	m_object(NULL), m_counter(source.m_counter)
	{
		if(m_counter)
		{
			m_counter->_WeakReference();
			m_object = source.m_object;
		}
	}		

	/*! \brief конструктор*/
	_weak_ptr_t(const _weak_ptr_t<TYPE>& source) : m_object(NULL), m_counter(source.m_counter)
	{
		if(m_counter)
		{
			m_counter->_WeakReference();
			m_object = source.Lock().m_object;
		}
	}

	/*! \brief деструктор*/
	~_weak_ptr_t() { if(m_counter) m_counter->_WeakUnreference(); }

//	операторы
public:

	/*! \brief	оператор присваивания */
	_weak_ptr_t<TYPE>& operator = (const _smart_ptr_t<TYPE>& source)
	{
		if(m_object != source.m_object)
		{
			if(source.m_counter) source.m_counter->_WeakReference();
			if(m_counter) m_counter->_WeakUnreference();

			m_counter = source.m_counter;
			m_object  = source.m_object;
		}
		return *this;
	}

	/*! \brief	оператор присваивания */
	_weak_ptr_t<TYPE>& operator = (const _weak_ptr_t<TYPE>& source)
	{
		if(m_object != source.m_object)
		{
			if(source.m_counter) source.m_counter->_WeakReference();
			if(m_counter) m_counter->_WeakUnreference();

			m_counter = source.m_counter;
			m_object  = source.Lock().m_object;
		}
		return *this;
	}

//	методы
public:

	/*! \brief проверка существования объекта
    * \returns TRUE - если число ссылок равно 0
    */
	inline BOOL Expired() const { return (m_counter ? (m_counter->_UseCount() == 0) : FALSE); }

	/*! \brief блокировка от удаления объекта*/
	inline _smart_ptr_t<TYPE> Lock() const
		{ return (Expired()? _smart_ptr_t<TYPE>(): _smart_ptr_t<TYPE>(*this)); }
	
	/*! \brief очистка - зануление*/
	inline VOID Reset() { *this = _weak_ptr_t<TYPE>(); }

};

#endif
