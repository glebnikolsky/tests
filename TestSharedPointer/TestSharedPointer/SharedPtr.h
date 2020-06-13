#pragma once
#ifndef __FTCUTIL_SHAREDVALUE_H_INCLUDED
#define __FTCUTIL_SHAREDVALUE_H_INCLUDED



/*!
*	\class _non_copyable_t
*   \brief ������� ����� ��� ������������ ��������
*/
class _non_copyable_t
{

    //	����������� / ����������
public:

    /*!	\brief ����������� �� ���������*/
    _non_copyable_t() { }
    /*! \brief ����������*/
    virtual ~_non_copyable_t() { }

    //	��������� 
private:
    /*! \brief �����������*/
    _non_copyable_t(const _non_copyable_t& source);
    // �������� ������������
    _non_copyable_t& operator = (const _non_copyable_t& /*source*/);/*{return *this;}*/


};


/*!
*	\class _shared_value_t
*	\brief ������ ���������� ��� ���������� �����
*/
template <class TYPE> class _shared_value_t : public _non_copyable_t
{

    //	��������
protected:


    typedef TYPE& REFTYPE; //!<	    ������ �� ���	

    mutable CRITICAL_SECTION m_section; //!<	����������� ������

    TYPE m_value; //!<	��������

    //	����������� / ����������
public:

    /*! \brief �����������*/
    _shared_value_t(const TYPE& value = TYPE()) :
        m_value(value) { ::InitializeCriticalSection(&m_section); }
    /*! \brief ����������*/
    ~_shared_value_t()
    { ::DeleteCriticalSection(&m_section); }

    //	���������
public:

    /*! \brief �������� ������������*/
    _shared_value_t<TYPE>& operator = (const TYPE& value)
    {
        ::EnterCriticalSection(&m_section);
        if(&m_value != &value) m_value = value;
        ::LeaveCriticalSection(&m_section);
        return *this;
    }

    /*! \brief �������� ��������������*/
    operator TYPE () const
    {
        ::EnterCriticalSection(&m_section);
        TYPE value = m_value;
        ::LeaveCriticalSection(&m_section);
        return value;
    }

};

#endif
