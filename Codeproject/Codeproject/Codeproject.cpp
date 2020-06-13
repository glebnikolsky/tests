// Codeproject.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
    int rows,cols,n;
    cin>>rows>>cols;
    n = rows*cols;
    vector<_TCHAR> v(n); 
    for(int i = 0; i < n; i++)cin>>v[i];
    int l = min(rows,cols)/2 - (min(rows,cols)%2 ? 0: 1);
    vector<vector<int> > mask(l+1);
    vector<int> sz(l+1);
    sz[0] = 1;
    for(int i = 1; i <= l; ++i){
        sz[i] = sz[i-1]+4;
        int base = cols*i+i;
        for(int c = i; c >= 1; --c){ 
            mask[i].push_back(base-c);
            mask[i].push_back(base+c);
            mask[i].push_back(base-c*cols);
            mask[i].push_back(base+c*cols);
        }
    }

	return 0;
}

