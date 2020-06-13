#include <iostream>
#include <vector>
#include<algorithm>
#include<string>

using namespace std;

template <typename T>
void RemoveDuplicates(vector<T>& elements)
{
    sort(begin(elements),end(elements));
    auto it = unique(begin(elements),end(elements));
    elements.erase(it, end(elements));

}

    int main() {
        int va[] = {6, 4, 7, 6, 4, 4, 0, 1};
        vector<int> v1(va,va+8);
        RemoveDuplicates(v1);
        for (int x : v1) {
            cout << x << " ";
        }
        cout << endl;

        
        string sa[] = {"C", "C++", "C++", "C", "C++"};
        vector<string> v2(sa,sa+5);
        RemoveDuplicates(v2);
        for (const string& s : v2) {
            cout << s << " ";
        }
        cout << endl;
        return 0;
    }

