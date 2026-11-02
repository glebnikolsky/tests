#include <climits>
#include <iostream>
#include <vector>
using namespace std;

int main()
{
	int n, m;
	cin >> n >> m;
	vector<vector<int>> field;
	for (int i = 0; i < n; ++i) {
		vector<int> row(m);
		for (int j = 0; j < m; ++j) cin >> row[j];
		field.emplace_back(row);
	}
	auto right = [&](int i, int j) { if (j < m - 1) return field[i][j + 1]; else return INT_MAX; };
	auto down = [&](int i, int j) { if (i < n - 1) return field[i + 1][j]; else return INT_MAX; };
	int r{ 0 }, c{ 0 };
	int weight{ field[0][0] };
	while (!(r == n - 1 && c == m - 1)) {
		if (right(r,c) < down(r, c)) c++;
		else r++;
		weight += field[r][c];
	}
	cout << weight << endl;

	return 0;
}
