/*!
*   \file SmartPtr.h
*/
//	����:		SmartPtr.h
//	��������:	��������������� ������ � ������  
//	(�) 2011 - 2014 ����������� ����� ������������

#pragma once
#ifndef __FTCUTIL_SMARTPTR_H_INCLUDED
#define __FTCUTIL_SMARTPTR_H_INCLUDED

/*!
*	\class _smart_ptr_t
*   \brief ������ ������ ���������
*/
template <class TYPE> class _smart_ptr_t
{

//	��������
protected:
	
	/*! \brief ������������� ������*/
	template<class TYPE> friend class _weak_ptr_t;

	
	typedef TYPE* PTRTYPE; //!< ��������� �� ���
	typedef TYPE& REFTYPE; //!< ������ �� ���

	
	PTRTYPE m_object;   //!<	��������� �� ������
	
	_ref_counter_t<TYPE>* m_counter;    //!<	��������� �� ������ �������� ������ 

//	����������� / ����������
public:

	/*! \brief ����������� �� ���������*/
	_smart_ptr_t() : m_object(NULL), m_counter(NULL) { }

	/*!	\brief �����������*/
	explicit _smart_ptr_t(PTRTYPE object) : m_object(object), m_counter(NULL)
	{ 
		m_counter = new _ref_counter_t<TYPE>(object);
	}

	/*!	\brief �����������*/
	explicit _smart_ptr_t(const _weak_ptr_t<TYPE>& source) : m_object(NULL), m_counter(source.m_counter)
	{
		if(m_counter && m_counter->_LockReference())
			m_object = source.m_object;
		else
			m_counter = NULL;
	}		

	/*!	\brief �����������*/
	_smart_ptr_t(const _smart_ptr_t<TYPE>& source) : m_object(NULL), m_counter(source.m_counter)
	{
		if(m_counter)
		{
			m_counter->_UseReference();
			m_object = source.m_object;
		}
	}		

	/*! \brief ����������*/
	~_smart_ptr_t() { if(m_counter) m_counter->_UseUnreference(); }

//	���������
public:

	/*! \brief �������� ������������ */
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

	/*! \brief �������� * */
	inline REFTYPE operator * () { return *m_object; }
	/*! \brief �������� * */
	inline const TYPE& operator * () const { return *m_object; }
	/*! \brief �������� -> */
	inline PTRTYPE operator -> () { return  m_object; }
	/*!	�������� const -> */
	inline const PTRTYPE operator -> () const { return  m_object; }

	/*! \brief �������� �������������� */
	inline operator REFTYPE () { return *m_object; }
	/*! \brief const �������� �������������� */
	inline operator const TYPE& () const { return *m_object; }
	/*! \brief	�������� �������������� */
	inline operator PTRTYPE () { return  m_object; }
	/*! \brief const �������� �������������� */
	inline operator const PTRTYPE () const { return  m_object; }

//	������
public:
	
	/*! \brief ���������� ����� ������*/
	inline long References() const { return (m_counter ? m_counter->_UseCount() : 0); }

	/*! \brief �������� �� ������� ����� ������������ ������
    * \returns true - �������� ������ 1 ������
    */
	inline bool Unique() const { return (References() == 1); }

	/*! \brief ������� - ���������*/
	inline void Reset() { *this = _smart_ptr_t<TYPE>(); }

//	���������
public:

	/*! \brief �������� >> */	
	template <class STREAM> friend inline
	STREAM& operator >> (STREAM& input, _smart_ptr_t<TYPE>& target)
	{
		ASSERT(target.m_object != NULL);
		input >> *target.m_object;
		return input;
	}
	
	/*! \brief �������� << */
	template <class STREAM> friend inline
	STREAM& operator << (STREAM& output, const _smart_ptr_t<TYPE>& source)
	{
		ASSERT(source.m_object != NULL);
		output << *source.m_object;
		return output;
	}

	/*! \brief �������� == */
	friend inline bool operator == (const _smart_ptr_t<TYPE>& left, const _smart_ptr_t<TYPE>& right)
		{ return (left.m_object == right.m_object);	}

	/*! \brief �������� != */
	friend inline bool operator != (const _smart_ptr_t<TYPE>& left, const _smart_ptr_t<TYPE>& right)
		{ return (left.m_object != right.m_object);	}

};

#endif
