#pragma once


// ��������� ��������� ����� ���� ��������� ����������

class SingleAppImpl;
class SingleApp
{
public:
	SingleApp(TCHAR *name); // �������� - ���������� ��� ����������
	~SingleApp(){}

	bool IsAlredyRunningThisMachine(); // �������� - ������� �� ��� ���������

	void RemoveAnyway(); // ������� �������, �� ����� ��������� ��� ��������� ������ �� ����������

protected:
	std::shared_ptr<SingleAppImpl> impl_;

};

