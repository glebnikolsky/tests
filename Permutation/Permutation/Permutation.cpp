#include <algorithm>
#include <iostream>
#include<vector>

using namespace std;


int main()
{
    int n;
    cin>>n;
    vector<int>v;
    for(int i = n; i >= 1; --i) v.push_back(i);
    do{
        for(auto i: v ) cout<<i<< ' ';
        cout<<endl;
    }while(prev_permutation(begin(v),end(v)));
    

    return 0;
}