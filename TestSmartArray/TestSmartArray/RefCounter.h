/*!
*   \file RefCounter.h
*/
//	����:		RefCounter.h
//	��������:	��������������� ������ � ������  
//	(�) 2011 - 2014 ����������� ����� ������������

#pragma once
#ifndef __FTCUTIL_REFCOUNTER_H_INCLUDED
#define __FTCUTIL_REFCOUNTER_H_INCLUDED
/*!
*   \class _ref_counter_t
*   \brief ������ ��� �������� ������
*/
template<class TYPE> class _ref_counter_t : public _non_copyable_t
{

//	��������
protected:

	//	������������� ������
	template<class TYPE> friend class _smart_ptr_t;
	template<class TYPE> friend class _weak_ptr_t;

	typedef TYPE* PTRTYPE;  //!< ��������� �� ���

	
    long m_use_count;   //!<	����� ������ ��� _smart_ptr_t
	
    long m_all_count;   //!< 	����� ������ ��� _weak_ptr_t + ( m_use_count == 0 ? 0 : 1)
	
	PTRTYPE m_object;   //!< 	��������� �� ������

//	�����������
protected:

	/*! \brief �����������*/
    explicit _ref_counter_t(PTRTYPE object) :
		m_use_count(1), m_all_count(1), m_object(object) { }

	/*! \brief ����������*/
	~_ref_counter_t() { }

//	������
protected:

	/*! \brief ���������� ������*/
	inline void _UseReference() { ::InterlockedIncrement(&m_use_count);	}
	/*! \brief ���������� ������*/
    inline bool _LockReference()
    {
		while(true)
		{
			long count = static_cast<const volatile long&>(m_use_count);
			if(count == 0) return false;
			if(::InterlockedCompareExchange(&m_use_count, count + 1, count) == count) return true;
        }
    }
	/*! \brief �������� ������*/
    inline void _UseUnreference()
    {
		if(::InterlockedDecrement(&m_use_count) == 0)
			_DisposeObject();
    }

	/*! \brief ���������� ������ ������*/
	inline void _WeakReference() { ::InterlockedIncrement(&m_all_count); }
	/*! \brief �������� ������ ������*/
    inline void _WeakUnreference()
	{
		if(::InterlockedDecrement(&m_all_count) == 0)
			_DisposeThis();
	}

	/*! \brief �������� �������*/
	inline void _DisposeObject() { delete m_object; _WeakUnreference(); }
	/*!	\brief �������� ����*/
	inline void _DisposeThis() { delete this; };

	/*! \brief ����� ������*/
	inline long _UseCount() const { return static_cast<const volatile long&>(m_use_count); }

};

#endif
