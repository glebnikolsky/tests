#pragma once
#ifndef __FTCUTIL_SHAREDVALUE_H_INCLUDED
#define __FTCUTIL_SHAREDVALUE_H_INCLUDED



/*!
*	\class _non_copyable_t
*   \brief Базовый класс для некопируемых объектов
*/
class _non_copyable_t
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
    _non_copyable_t& operator = (const _non_copyable_t& /*source*/);/*{return *this;}*/


};


/*!
*	\class _shared_value_t
*	\brief Шаблон переменной для нескольких нитей
*/
template <class TYPE> class _shared_value_t : public _non_copyable_t
{

    //	свойства
protected:


    typedef TYPE& REFTYPE; //!<	    ссылка на тип	

    mutable CRITICAL_SECTION m_section; //!<	критическая секция

    TYPE m_value; //!<	значение

    //	конструктор / деструктор
public:

    /*! \brief конструктор*/
    _shared_value_t(const TYPE& value = TYPE()) :
        m_value(value) { ::InitializeCriticalSection(&m_section); }
    /*! \brief деструктор*/
    ~_shared_value_t()
    { ::DeleteCriticalSection(&m_section); }

    //	операторы
public:

    /*! \brief оператор присваивания*/
    _shared_value_t<TYPE>& operator = (const TYPE& value)
    {
        ::EnterCriticalSection(&m_section);
        if(&m_value != &value) m_value = value;
        ::LeaveCriticalSection(&m_section);
        return *this;
    }

    /*! \brief оператор преобразования*/
    operator TYPE () const
    {
        ::EnterCriticalSection(&m_section);
        TYPE value = m_value;
        ::LeaveCriticalSection(&m_section);
        return value;
    }

};

#endif
