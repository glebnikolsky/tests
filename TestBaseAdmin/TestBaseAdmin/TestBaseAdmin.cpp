// TestBaseAdmin.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <iostream>
#include <string>

int main()
{
    {    
        std::string s{ "ERBD_EGE_MAIN_23_APP" };
        s += std::string(s.length() % 4, ' ');
        uint32_t seed{ 0 };
        const char* ch = s.c_str();
        while (*ch) {
            seed += *((uint32_t*)ch);
            ch += 4;
        }
        std::cout << s.length() << '\t' << seed << '\t' << (seed ^ 3)<<'\n';
    }
    {
        std::string s{ "ERBD_EGE_MAIN_23" };
        s += std::string(s.length() % 4, ' ');
        uint32_t seed{ 0 };
        const char* ch = s.c_str();
        while (*ch) {
            seed += *((uint32_t*)ch);
            ch += 4;
        }
        std::cout << s.length() << '\t' << seed << '\t' << (seed ^ 3) << '\n';
    }
    {
        std::string s{ "ERBD_EGE_MAIN_23_APP" };
        s += std::string(s.length() % 4, ' ');
        uint32_t seed{ 0 };
        const char* ch = s.c_str();
        while (*ch) {
            seed += *((uint32_t*)ch);
            ch += 4;
        }
        std::cout << s.length() << '\t' << seed << '\t' << (seed ^ 4) << '\n';
    }
    {
        std::string s{ "ERBD_EGE_MAIN_23" };
        s += std::string(s.length() % 4, ' ');
        uint32_t seed{ 0 };
        const char* ch = s.c_str();
        while (*ch) {
            seed += *((uint32_t*)ch);
            ch += 4;
        }
        std::cout << s.length() << '\t' << seed << '\t' << (seed ^ 4) << '\n';
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
