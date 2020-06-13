/*!
*   \file SmartArray.h
*/
//	����:		SmartArray.h
//	��������:	��������������� ������ � ������  
//	(�) 2011 - 2014 ����������� ����� ������������

#pragma once
#ifndef __FTCUTIL_SMARTARRAY_H_INCLUDED
#define __FTCUTIL_SMARTARRAY_H_INCLUDED

/*! 
*   \class _smart_array_t
*   \brief ������ ����� ����������
*/
template <class TYPE> class _smart_array_t
{

//	��������
protected:

	
	typedef std::vector< _smart_ptr_t<TYPE> > CONTAINER;    //!< ������ ���������������� ���������� ���� 
	typedef CONTAINER& REFCONTAINER;    //!< ������ �� ������ ���������������� ����������

	
	CONTAINER m_array;  //!<	������ ��������

//	������
public:
    /*! \brief ������ ������� � ������ ��� ������ � ����*/
    ULONGLONG total_bytes;

	/*! \brief �������� ��������� �����
    *   \param where ������
    */
	inline _smart_ptr_t<TYPE> Get(ULONG where) { return m_array[where]; }
	/*! \brief �������� ������
    *   \param where ������
    */
	inline const _smart_ptr_t<TYPE>& Get(ULONG where) const { return m_array[where]; }
	
	/*! \brief �������� ������*/
	inline ULONG Size() const { return static_cast<ULONG>(m_array.size()); }
	/*! \brief ��������� �������*/
	inline VOID Resize(ULONG size) { m_array.resize(size); }
	/*!	\brief �������������� �������*/
	inline VOID Reserve(ULONG size) { m_array.reserve(size); }
	/*! \brief ������� - ���������*/
	inline VOID Clear() { m_array.clear(); }

	/*! \brief ��������� � �����*/
	inline VOID Add(const _smart_ptr_t<TYPE>& object)
		{ m_array.push_back(object); }
	/*! \brief ������� � ��������*/ 
	inline VOID Ins(ULONG where, const _smart_ptr_t<TYPE>& object)
		{ m_array.insert(m_array.begin() + where, object); }
	/*! \brief ������������� �� �������*/
	inline VOID Set(ULONG where, const _smart_ptr_t<TYPE>& object)
		{ m_array[where] = object; }
	/*! \brief ��������*/
	inline VOID Del(ULONG where)
		{ m_array.erase(m_array.begin() + where); }

//	������
public:
	
	/*! \brief ��������*/
	inline VOID Del(const _smart_ptr_t<TYPE>& object)
	{
		ULONG where = Find(object, 0);
		while(where != ULONG_MAX)
		{
			Del(where);
			where = Find(object, where);
		}
	}

	/*! \brief �����
    *   \returns ULONG_MAX ���� �� ������
    */
	inline ULONG Find(const _smart_ptr_t<TYPE>& object, ULONG where = 0) const
	{
		for(ULONG index = where; index < Size(); ++index)
			if(m_array[index] == object) return index;
		return ULONG_MAX;
	}

	/*! \brief ����������*/
	template <class PREDICATE > inline
	VOID Sort(PREDICATE predicate) { std::sort(m_array.begin(), m_array.end(), predicate); }

//	���������
public:
	
	/*! \brief �������� >> */
	template <class STREAM> friend inline
	STREAM& operator >> (STREAM& input, _smart_array_t<TYPE>& target)
	{
		ULONG length = 0;
		input >> length;
		target.Resize(length);        
		for(ULONG index = 0; index < length; ++index)
			input >> (target.m_array[index] = _smart_ptr_t<TYPE>(new TYPE()));
		return input;
	}

	/*! \brief �������� << */
	template <class STREAM> friend inline
	STREAM& operator << (STREAM& output, const _smart_array_t<TYPE>& source)
	{
		ULONG length = source.Size();
		output << length;		
        for(ULONG index = 0; index < length; ++index)
			output << source.m_array[index];
		return output;
	}

};

#endif
