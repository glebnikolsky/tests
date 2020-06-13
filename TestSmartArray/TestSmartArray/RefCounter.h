/*!
*   \file RefCounter.h
*/
//	‘айл:		RefCounter.h
//	ќписание:	¬спомогательные классы и методы  
//	(с) 2011 - 2014 ‘едеральный центр тестировани€

#pragma once
#ifndef __FTCUTIL_REFCOUNTER_H_INCLUDED
#define __FTCUTIL_REFCOUNTER_H_INCLUDED
/*!
*   \class _ref_counter_t
*   \brief Ўаблон дл€ подсчета ссылок
*/
template<class TYPE> class _ref_counter_t : public _non_copyable_t
{

//	свойства
protected:

	//	дружественные классы
	template<class TYPE> friend class _smart_ptr_t;
	template<class TYPE> friend class _weak_ptr_t;

	typedef TYPE* PTRTYPE;  //!< указатель на тип

	
    long m_use_count;   //!<	число ссылок дл€ _smart_ptr_t
	
    long m_all_count;   //!< 	число ссылок дл€ _weak_ptr_t + ( m_use_count == 0 ? 0 : 1)
	
	PTRTYPE m_object;   //!< 	указатель на объект

//	конструктор
protected:

	/*! \brief конструктор*/
    explicit _ref_counter_t(PTRTYPE object) :
		m_use_count(1), m_all_count(1), m_object(object) { }

	/*! \brief деструктор*/
	~_ref_counter_t() { }

//	методы
protected:

	/*! \brief добавление ссылки*/
	inline void _UseReference() { ::InterlockedIncrement(&m_use_count);	}
	/*! \brief добавление ссылки*/
    inline bool _LockReference()
    {
		while(true)
		{
			long count = static_cast<const volatile long&>(m_use_count);
			if(count == 0) return false;
			if(::InterlockedCompareExchange(&m_use_count, count + 1, count) == count) return true;
        }
    }
	/*! \brief удаление ссылки*/
    inline void _UseUnreference()
    {
		if(::InterlockedDecrement(&m_use_count) == 0)
			_DisposeObject();
    }

	/*! \brief добавление слабой ссылки*/
	inline void _WeakReference() { ::InterlockedIncrement(&m_all_count); }
	/*! \brief удаление слабой ссылки*/
    inline void _WeakUnreference()
	{
		if(::InterlockedDecrement(&m_all_count) == 0)
			_DisposeThis();
	}

	/*! \brief удаление объекта*/
	inline void _DisposeObject() { delete m_object; _WeakUnreference(); }
	/*!	\brief удаление себ€*/
	inline void _DisposeThis() { delete this; };

	/*! \brief число ссылок*/
	inline long _UseCount() const { return static_cast<const volatile long&>(m_use_count); }

};

#endif
