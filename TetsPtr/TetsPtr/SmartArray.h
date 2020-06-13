/*!
*   \file SmartArray.h
*/
//	Файл:		SmartArray.h
//	Описание:	Вспомогательные классы и методы  
//	(с) 2011 - 2014 Федеральный центр тестирования

#pragma once
#ifndef __FTCUTIL_SMARTARRAY_H_INCLUDED
#define __FTCUTIL_SMARTARRAY_H_INCLUDED

/*! 
*   \class _smart_array_t
*   \brief Массив умных указателей
*/
template <class TYPE> class _smart_array_t
{

//	свойства
protected:

	
	typedef std::vector< _smart_ptr_t<TYPE> > CONTAINER;    //!< вектор интеллектуальных указателей типа 
	typedef CONTAINER& REFCONTAINER;    //!< ссылка на вектор интеллектуальных указателей

	
	CONTAINER m_array;  //!<	массив объектов

//	методы
public:
    /*! \brief размер массива в байтах при записи в файл*/
    ULONGLONG total_bytes;

	/*! \brief получаем временную копию
    *   \param where индекс
    */
	inline _smart_ptr_t<TYPE> Get(ULONG where) { return m_array[where]; }
	/*! \brief получаем ссылку
    *   \param where индекс
    */
	inline const _smart_ptr_t<TYPE>& Get(ULONG where) const { return m_array[where]; }
	
	/*! \brief получаем размер*/
	inline ULONG Size() const { return static_cast<ULONG>(m_array.size()); }
	/*! \brief изменение размера*/
	inline VOID Resize(ULONG size) { m_array.resize(size); }
	/*!	\brief резервирование размера*/
	inline VOID Reserve(ULONG size) { m_array.reserve(size); }
	/*! \brief очистка - зануление*/
	inline VOID Clear() { m_array.clear(); }

	/*! \brief добавляем в конец*/
	inline VOID Add(const _smart_ptr_t<TYPE>& object)
		{ m_array.push_back(object); }
	/*! \brief вставка в середину*/ 
	inline VOID Ins(ULONG where, const _smart_ptr_t<TYPE>& object)
		{ m_array.insert(m_array.begin() + where, object); }
	/*! \brief устанавливаем по индексу*/
	inline VOID Set(ULONG where, const _smart_ptr_t<TYPE>& object)
		{ m_array[where] = object; }
	/*! \brief удаление*/
	inline VOID Del(ULONG where)
		{ m_array.erase(m_array.begin() + where); }

//	методы
public:
	
	/*! \brief удаление*/
	inline VOID Del(const _smart_ptr_t<TYPE>& object)
	{
		ULONG where = Find(object, 0);
		while(where != ULONG_MAX)
		{
			Del(where);
			where = Find(object, where);
		}
	}

	/*! \brief поиск
    *   \returns ULONG_MAX если не найден
    */
	inline ULONG Find(const _smart_ptr_t<TYPE>& object, ULONG where = 0) const
	{
		for(ULONG index = where; index < Size(); ++index)
			if(m_array[index] == object) return index;
		return ULONG_MAX;
	}

	/*! \brief сортировка*/
	template <class PREDICATE > inline
	VOID Sort(PREDICATE predicate) { std::sort(m_array.begin(), m_array.end(), predicate); }

//	операторы
public:
	
	/*! \brief оператор >> */
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

	/*! \brief оператор << */
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
