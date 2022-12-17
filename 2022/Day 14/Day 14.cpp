#include <bits/stdc++.h>
#include <chrono>
#include <regex>

using namespace std;

bool grid[200][500];

int main() {

	auto start = chrono::steady_clock::now();

	ifstream in("input.txt");
	string line;

	int hy = 0;

	while (in.peek() != EOF) {

		getline(in, line);
		regex rgx("([0-9]{1,3}),([0-9]{1,3})( ->)?");
		std::smatch match;

		regex_search(line, match, rgx);
		int x = stoi(match[1]) - 250;
		int y = stoi(match[2]);

		while (regex_search(line, match, rgx)) {

			hy = max(y, hy);

			int x0 = stoi(match[1]) - 250;
			int y0 = stoi(match[2]);
			grid[y0][x0] = true;

			while (x != x0 || y != y0) {
				if (x > x0) x0++;
				if (x < x0) x0--;
				if (y > y0) y0++;
				if (y < y0) y0--;
				grid[y0][x0] = true;
			}

			x = stoi(match[1]) - 250;
			y = stoi(match[2]);

			line = match.suffix().str();
		}

	}

	hy += 2;
	int cx = 0;
	while (cx < 500) {
		grid[hy][cx] = true;
		cx++;
	}

	int ans = 0;
	int sand = 0;

	int x = 250;
	int y = 0;

	while (true) {
		if (y == hy - 1 && ans == 0) ans = sand;
		if (!grid[y + 1][x]) y++;
		else if (!grid[y + 1][x - 1]) {
			y++;
			x--;
		}
		else if (!grid[y + 1][x + 1]) {
			y++;
			x++;
		}
		else {
			sand++;
			grid[y][x] = true;
			if (x == 250 && y == 0) break;
			x = 250;
			y = 0;
		}
	}

	cout << "The number of sand coming to rest without floor is: " << ans << endl;
	cout << "The number of sand coming to rest with the floor is: " << sand << endl;

	auto end = chrono::steady_clock::now();
	cout << "Time:\n"
		<< chrono::duration_cast<chrono::microseconds>(end - start).count() << " us\n"
		<< chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms";

}