// Ydx.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main()
{
	int plane;
	cin >> plane;
	vector<string> seats(plane);
	for (int i = 0; i < plane; ++i) {
		cin >> seats[i];
		seats[i] = seats[i];
	}
	int passengers;
	cin >> passengers;
	char letters[] = { 'A','B','C','.','D','E' ,'F' };
	while (passengers--) {
		int n;
		string side, pos;
		cin >> n >> side >> pos;
		string pattern(n, '.');
		int shift{ 0 };
		if (side == "left") {
			if (pos == "aisle") shift = 3 - n;
		}
		else {
			if (pos == "aisle") shift = 4;
			else shift = 7 - n;
		}
		int row = 0;
		for (; row < plane; ++row) {
			if (seats[row].substr(shift, n) == pattern) {
				for (int i = shift; i < shift + n; ++i) seats[row][i] = 'X';
				cout << "Passengers can take seats:";
				for (int i{ 0 }; i < n; ++i) cout << " " << row + 1 << letters[shift + i];
				cout << '\n';
				for (auto& s : seats) cout << s << '\n';
				for (int i = shift; i < shift + n; ++i) seats[row][i] = '#';
				break;
				}
		}
		if (row == plane) 
			cout << "Cannot fulfill passengers requirements\n";
	}
	return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
