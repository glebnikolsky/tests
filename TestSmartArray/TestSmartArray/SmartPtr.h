/*!
*   \file SmartPtr.h
*/
//	Файл:		SmartPtr.h
//	Описание:	Вспомогательные классы и методы  
//	(с) 2011 - 2014 Федеральный центр тестирования

#pragma once
#ifndef __FTCUTIL_SMARTPTR_H_INCLUDED
#define __FTCUTIL_SMARTPTR_H_INCLUDED

/*!
*	\class _smart_ptr_t
*   \brief Шаблон умного указателя
*/
template <class TYPE> class _smart_ptr_t
{

//	свойства
protected:
	
	/*! \brief дружественные классы*/
	template<class TYPE> friend class _weak_ptr_t;

	
	typedef TYPE* PTRTYPE; //!< указатель на тип
	typedef TYPE& REFTYPE; //!< ссылка на тип

	
	PTRTYPE m_object;   //!<	указатель на объект
	
	_ref_counter_t<TYPE>* m_counter;    //!<	указатель на объект подсчета ссылок 

//	конструктор / деструктор
public:

	/*! \brief конструктор по умолчанию*/
	_smart_ptr_t() : m_object(NULL), m_counter(NULL) { }

	/*!	\brief конструктор*/
	explicit _smart_ptr_t(PTRTYPE object) : m_object(object), m_counter(NULL)
	{ 
		m_counter = new _ref_counter_t<TYPE>(object);
	}

	/*!	\brief конструктор*/
	explicit _smart_ptr_t(const _weak_ptr_t<TYPE>& source) : m_object(NULL), m_counter(source.m_counter)
	{
		if(m_counter && m_counter->_LockReference())
			m_object = source.m_object;
		else
			m_counter = NULL;
	}		

	/*!	\brief конструктор*/
	_smart_ptr_t(const _smart_ptr_t<TYPE>& source) : m_object(NULL), m_counter(source.m_counter)
	{
		if(m_counter)
		{
			m_counter->_UseReference();
			m_object = source.m_object;
		}
	}		

	/*! \brief деструктор*/
	~_smart_ptr_t() { if(m_counter) m_counter->_UseUnreference(); }

//	операторы
public:

	/*! \brief оператор присваивания */
	_smart_ptr_t<TYPE>& operator = (const _smart_ptr_t<TYPE>& source)
	{
		if(m_object != source.m_object)
		{
			if(source.m_counter) source.m_counter->_UseReference();
			if(m_counter) m_counter->_UseUnreference();

			m_counter = source.m_counter;
			m_object  = source.m_object;
		}
		return *this;
	}

	/*! \brief оператор * */
	inline REFTYPE operator * () { return *m_object; }
	/*! \brief оператор * */
	inline const TYPE& operator * () const { return *m_object; }
	/*! \brief оператор -> */
	inline PTRTYPE operator -> () { return  m_object; }
	/*!	оператор const -> */
	inline const PTRTYPE operator -> () const { return  m_object; }

	/*! \brief оператор преобразования */
	inline operator REFTYPE () { return *m_object; }
	/*! \brief const оператор преобразования */
	inline operator const TYPE& () const { return *m_object; }
	/*! \brief	оператор преобразования */
	inline operator PTRTYPE () { return  m_object; }
	/*! \brief const оператор преобразования */
	inline operator const PTRTYPE () const { return  m_object; }

//	методы
public:
	
	/*! \brief возвращает число ссылок*/
	inline long References() const { return (m_counter ? m_counter->_UseCount() : 0); }

	/*! \brief проверка на наличие одной единственной ссылки
    * \returns true - хранится только 1 ссылка
    */
	inline bool Unique() const { return (References() == 1); }

	/*! \brief очистка - зануление*/
	inline void Reset() { *this = _smart_ptr_t<TYPE>(); }

//	операторы
public:

	/*! \brief оператор >> */	
	template <class STREAM> friend inline
	STREAM& operator >> (STREAM& input, _smart_ptr_t<TYPE>& target)
	{
		ASSERT(target.m_object != NULL);
		input >> *target.m_object;
		return input;
	}
	
	/*! \brief оператор << */
	template <class STREAM> friend inline
	STREAM& operator << (STREAM& output, const _smart_ptr_t<TYPE>& source)
	{
		ASSERT(source.m_object != NULL);
		output << *source.m_object;
		return output;
	}

	/*! \brief оператор == */
	friend inline bool operator == (const _smart_ptr_t<TYPE>& left, const _smart_ptr_t<TYPE>& right)
		{ return (left.m_object == right.m_object);	}

	/*! \brief оператор != */
	friend inline bool operator != (const _smart_ptr_t<TYPE>& left, const _smart_ptr_t<TYPE>& right)
		{ return (left.m_object != right.m_object);	}

};

#endif
