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
	
	/*! \brief �������� ��������� �����
    *   \param where ������
    */
	inline _smart_ptr_t<TYPE> Get(unsigned long where) { return m_array[where]; }
	/*! \brief �������� ������
    *   \param where ������
    */
	inline const _smart_ptr_t<TYPE>& Get(unsigned long where) const { return m_array[where]; }
	
	/*! \brief �������� ������*/
	inline unsigned long Size() const { return static_cast<unsigned long>(m_array.size()); }
	/*! \brief ��������� �������*/
	inline void Resize(unsigned long size) { m_array.resize(size); }
	/*!	\brief �������������� �������*/
	inline void Reserve(unsigned long size) { m_array.reserve(size); }
	/*! \brief ������� - ���������*/
	inline void Clear() { m_array.clear(); }

	/*! \brief ��������� � �����*/
	inline void Add(const _smart_ptr_t<TYPE>& object)
		{ m_array.push_back(object); }
	/*! \brief ������� � ��������*/ 
	inline void Ins(unsigned long where, const _smart_ptr_t<TYPE>& object)
		{ m_array.insert(m_array.begin() + where, object); }
	/*! \brief ������������� �� �������*/
	inline void Set(unsigned long where, const _smart_ptr_t<TYPE>& object)
		{ m_array[where] = object; }
	/*! \brief ��������*/
	inline void Del(unsigned long where)
		{ m_array.erase(m_array.begin() + where); }

//	������
public:
	
	/*! \brief ��������*/
	inline void Del(const _smart_ptr_t<TYPE>& object)
	{
		unsigned long where = Find(object, 0);
		while(where != unsigned long_MAX)
		{
			Del(where);
			where = Find(object, where);
		}
	}

	/*! \brief �����
    *   \returns unsigned long_MAX ���� �� ������
    */
	inline long Find(const _smart_ptr_t<TYPE>& object, unsigned long where = 0) const
	{
		for(unsigned long index = where; index < Size(); ++index)
			if(m_array[index] == object) return index;
		return -1;
	}

	/*! \brief ����������*/
	template <class PREDICATE > inline
	void Sort(PREDICATE predicate) { std::sort(m_array.begin(), m_array.end(), predicate); }

//	���������
public:
	
	/*! \brief �������� >> */
	template <class STREAM> friend inline
	STREAM& operator >> (STREAM& input, _smart_array_t<TYPE>& target)
	{
		unsigned long length = 0;
		input >> length;
		target.Resize(length);
		for(unsigned long index = 0; index < length; ++index)
			input >> (target.m_array[index] = _smart_ptr_t<TYPE>(new TYPE()));
		return input;
	}

	/*! \brief �������� << */
	template <class STREAM> friend inline
	STREAM& operator << (STREAM& output, const _smart_array_t<TYPE>& source)
	{
		unsigned long length = source.Size();
		output << length;
		for(unsigned long index = 0; index < length; ++index)
			output << source.m_array[index];
		return output;
	}

};

#endif
