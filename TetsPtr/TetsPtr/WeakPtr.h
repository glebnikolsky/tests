/*! 
*   \file WeakPtr.h
*/
//	����:		WeakPtr.h
//	��������:	��������������� ������ � ������  
//	(�) 2011 - 2014 ����������� ����� ������������

#pragma once
#ifndef __FTCUTIL_WEAKPTR_H_INCLUDED
#define __FTCUTIL_WEAKPTR_H_INCLUDED

/*!
*	\class _weak_ptr_t
*	\brief ������ ������� ���������
*/
template <class TYPE> class _weak_ptr_t
{

//	��������
protected:

	/*! \brif ������������� ������*/
    template<class TYPE> friend class _smart_ptr_t;

	
	typedef TYPE* PTRTYPE; //!< ��������� �� ���
	typedef TYPE& REFTYPE; //!< ������ �� ���

	
	PTRTYPE m_object; //!<   ��������� �� ������
	
	_ref_counter_t<TYPE>* m_counter;    //!<	��������� �� ������ �������� ������ 

//	����������� / ����������
public:

	/*! \brief	����������� �� ���������*/
	_weak_ptr_t() : m_object(NULL), m_counter(NULL) { }

	/*! \brief �����������*/
	_weak_ptr_t(const _smart_ptr_t<TYPE>& source) :	m_object(NULL), m_counter(source.m_counter)
	{
		if(m_counter)
		{
			m_counter->_WeakReference();
			m_object = source.m_object;
		}
	}		

	/*! \brief �����������*/
	_weak_ptr_t(const _weak_ptr_t<TYPE>& source) : m_object(NULL), m_counter(source.m_counter)
	{
		if(m_counter)
		{
			m_counter->_WeakReference();
			m_object = source.Lock().m_object;
		}
	}

	/*! \brief ����������*/
	~_weak_ptr_t() { if(m_counter) m_counter->_WeakUnreference(); }

//	���������
public:

	/*! \brief	�������� ������������ */
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

	/*! \brief	�������� ������������ */
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

//	������
public:

	/*! \brief �������� ������������� �������
    * \returns TRUE - ���� ����� ������ ����� 0
    */
	inline BOOL Expired() const { return (m_counter ? (m_counter->_UseCount() == 0) : FALSE); }

	/*! \brief ���������� �� �������� �������*/
	inline _smart_ptr_t<TYPE> Lock() const
		{ return (Expired()? _smart_ptr_t<TYPE>(): _smart_ptr_t<TYPE>(*this)); }
	
	/*! \brief ������� - ���������*/
	inline VOID Reset() { *this = _weak_ptr_t<TYPE>(); }

};

#endif
