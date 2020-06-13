/*!
*   \file NonCopyable.h
*/
//	����:		NonCopyable.h
//	��������:	��������������� ������ � ������  
//	(�) 2011 - 2014 ����������� ����� ������������

#pragma once
#ifndef __FTCUTIL_NONCOPYABLE_H_INCLUDED
#define __FTCUTIL_NONCOPYABLE_H_INCLUDED

/*!
*	\class _non_copyable_t
*   \brief ������� ����� ��� ������������ ��������
*/
class __FTCUTIL_CLASS _non_copyable_t
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
    _non_copyable_t& operator = (const _non_copyable_t& source){return *this;}


};

#endif
