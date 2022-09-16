#ifndef _TYPOGRAPHY_TYPOGRAPHYTBL_H
#define _TYPOGRAPHY_TYPOGRAPHYTBL_H
#pragma once

namespace TechnologiesTable{

	std::pair<bool,CString> CheckDB( CDatabase &db );
	
	// ������� ����������� ������ �� rbdc_ExamTechnologies
	void FillRows( CDatabase &db );

	// ��������, ��� ��� ������ ���������
	bool IsAllRowsFilled( CDatabase *db );
}

#endif _TYPOGRAPHY_TYPOGRAPHYTBL_H