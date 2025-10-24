// CPUCache.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <array>
#include <chrono>
#include <vector>

template<typename T>
void FillR(T mtx, size_t n, size_t m) {
    int val{ 0 };
    for (size_t i = 0; i < n; ++i)
        for (size_t j = 0; j < m; ++j)
            mtx[i][j] = val++;
}

void FillP(int *f, size_t n, size_t m) {
    int val{ 0 };
    for (size_t i = 0; i < n*m; ++i, ++f)
            *f = val++;
}



template<typename T>
void FillC(T mtx, size_t n, size_t m) {
    int val{ 0 };
    for (size_t j = 0; j < m; ++j)
        for (size_t i = 0; i < n; ++i)
            mtx[i][j] = val++;
}


template<typename T>
long long SumR(T mtx, size_t n, size_t m) {
    long long val{ 0 };
    for (size_t i = 0; i < n; ++i)
        for (size_t j = 0; j < m; ++j)
            val += mtx[i][j];
    return val;
}

template<typename T>
long long SumC(T mtx, size_t n, size_t m) {
    long long val{ 0 };
    for (size_t j = 0; j < m; ++j)
        for (size_t i = 0; i < n; ++i)
            val += mtx[i][j];
    return val;
}

using namespace std;

int main()
 {
    const int N = 199;
    const int M = 201;
    vector<vector<int>> v(N,vector<int>(M));
    array<array<int, M>, N> a;
    chrono::high_resolution_clock::time_point t = chrono::high_resolution_clock::now();
    FillR(v, N, M);
    chrono::high_resolution_clock::duration d = chrono::high_resolution_clock::now() - t;
    cout << "FillR(v," << N << "," << M << ")=" << d.count() << '\n';
    t = chrono::high_resolution_clock::now();
    FillC(v, N, M);
    d = chrono::high_resolution_clock::now() - t;
    cout << "FillC(v," << N << "," << M << ")=" << d.count() << '\n';
    t = chrono::high_resolution_clock::now();
    FillR(a, N, M);
    d = chrono::high_resolution_clock::now() - t;
    cout << "FillR(a," << N << "," << M << ")=" << d.count() << '\n';
    t = chrono::high_resolution_clock::now();
    FillC(a, N, M);
    d = chrono::high_resolution_clock::now() - t;
    cout << "FillC(a," << N << "," << M << ")=" << d.count() << '\n';
    t = chrono::high_resolution_clock::now();
    FillP(&a[0][0], N, M);
    d = chrono::high_resolution_clock::now() - t;
    cout << "FillP(a," << N << "," << M << ")=" << d.count() << '\n';
    t = chrono::high_resolution_clock::now();
    FillP(&v[0][0], N, M);
    d = chrono::high_resolution_clock::now() - t;
    cout << "FillP(v," << N << "," << M << ")=" << d.count() << '\n';
    long long val;
    t = chrono::high_resolution_clock::now();
    val = SumR(v, N, M);
    d = chrono::high_resolution_clock::now() - t;
    cout << "SumR(v," << N << "," << M << ")=" << d.count() << '\n';
    t = chrono::high_resolution_clock::now();
    val = SumC(v, N, M);
    d = chrono::high_resolution_clock::now() - t;
    cout << "SumC(v," << N << "," << M << ")=" << d.count() << '\n';
    t = chrono::high_resolution_clock::now();
    val = SumR(a, N, M);
    d = chrono::high_resolution_clock::now() - t;
    cout << "SumR(a," << N << "," << M << ")=" << d.count() << '\n';
    t = chrono::high_resolution_clock::now();
    val = SumC(a, N, M);
    d = chrono::high_resolution_clock::now() - t;
    cout << "SumC(a," << N << "," << M << ")=" << d.count() << '\n';


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
