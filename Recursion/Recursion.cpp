// Recursion.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <set>
#include <string>

using namespace std;
// Сигнатура вспомогательной рекурсивной функции
void generate(
    int left_open, // Сколько открывающих скобок ещё можно поставить
    int left_closed, // Сколько закрывающих скобок ещё можно поставить
    string accum, // Текущая последовательность
    vector<string>* result
);

vector<string> generate(int N) {
    // Наша рекурсивная функция ничего не возвращает,
    // вместо этого заполняет результирующий массив
    vector<string> result;

    generate(N, N, "", &result);
    return result;
}

void generate(
    int left_open,
    int left_closed,
    string accum,
    vector<string>* result
) {
    cout << accum << '\n';
    if (!left_open && !left_closed) {
        result->push_back(accum);
        cout << '\t' << accum << '\n';
        return;
    }
    if (left_open)
        generate(left_open - 1, left_closed, accum + "(", result);
    if (left_closed > left_open)
        generate(left_open, left_closed - 1, accum + ")", result);
}
int main()
{
    vector<string> res;
    res = generate(5);
    set<string> ress;
    for (auto const& i : res) ress.insert(i);
    std::cout << "Hello World!\n";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
