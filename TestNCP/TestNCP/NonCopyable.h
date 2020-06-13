//	����:		NonCopyable.h
//	��������:	��������������� ������ � ������  
//	(�) 2011 ����������� ����� ������������

#pragma once
#ifndef __FTCUTIL_NONCOPYABLE_H_INCLUDED
#define __FTCUTIL_NONCOPYABLE_H_INCLUDED

//	�����:		_non_copyable_t
//	��������:	������� ����� ��� ������������ ��������
class _non_copyable_t
{

//	����������� / ����������
public:

	//	�����������
	_non_copyable_t() { }
	//	����������
	virtual ~_non_copyable_t() { }

//	��������� 
private:

	//	�����������
	_non_copyable_t(const _non_copyable_t& source);
	//	�������� ������������
	_non_copyable_t& operator = (const _non_copyable_t& source);

};

#endif
