#include <bits/stdc++.h>
#include <chrono>

using namespace std;

char grid[42][136];
bool checked[42][136];

int main() {

	auto start = chrono::steady_clock::now();

	ifstream in("input.txt");
	string line;

	int xS{}, yS{}, xE{}, yE{};

	int i = 0;
	while (in.peek() != EOF) {
		getline(in, line);
		for (int j = 0; j < 136; j++) {
			grid[i][j] = line[j];
			if (grid[i][j] == 'S') {
				xS = j;
				yS = i;
			}
			if (grid[i][j] == 'E') {
				xE = j;
				yE = i;
			}
		}
		i++;
	}

	queue<tuple<int, int, int>> queue;
	pair<int, int> edges[]{ {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
	bool f1 = false;
	bool f2 = false;

	grid[yS][xS] = 'a';
	grid[yE][xE] = 'z';

	checked[yE][xE] = true;
	queue.push(make_tuple(0, xE, yE));

	while (!queue.empty()) {
		tuple<int, int, int> cur = queue.front();
		queue.pop();
		if (get<1>(cur) == xS && get<2>(cur) == yS && !f1) {
			cout << "Minimum number of steps starting from S: " << get<0>(cur) << endl;
			if (f2) break;
			else f1 = true;
		}
		if (grid[get<2>(cur)][get<1>(cur)] == 'a' && !f2) {
			cout << "Minimum number of steps starting from any 'a': " << get<0>(cur) << endl;
			if (f1) break;
			else f2 = true;
		}
		for (auto p : edges) {
			int x = get<1>(cur) + p.first;
			int y = get<2>(cur) + p.second;
			if (x < 0 || x >= 136 || y < 0 || y >= 42 || checked[y][x] || grid[y][x] + 1 < grid[get<2>(cur)][get<1>(cur)]) continue;
			checked[y][x] = true;
			queue.push(tuple<int, int, int> { get<0>(cur) + 1, x, y });
		}
	}

	auto end = chrono::steady_clock::now();
	cout << "Time:\n" 
		<< chrono::duration_cast<chrono::microseconds>(end - start).count() << " us\n" 
		<< chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms";

}