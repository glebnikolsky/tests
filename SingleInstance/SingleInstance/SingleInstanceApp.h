#pragma once


// ѕозвол€ет запустить ровно один экземпл€р приложени€

class SingleAppImpl;
class SingleApp
{
public:
	SingleApp(TCHAR *name); // параметр - уникальное им€ приложени€
	~SingleApp(){}

	bool IsAlredyRunningThisMachine(); // проверка - запущен ли уже экземпл€р

	void RemoveAnyway(); // удалить мьютекс, он может зависнуть при аварийном выходе их приложени€

protected:
	std::shared_ptr<SingleAppImpl> impl_;

};

