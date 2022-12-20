#include <bits/stdc++.h>
#include <chrono>
#include <regex>

#define p pair<int, int>

using namespace std;

struct sensor {
	int x;
	int y;
	int beaconX;
	int beaconY;
	int range;
	pair<p, p> lines[4];

	sensor(int sx, int sy, int bx, int by, int r, p top, p right, p bottom, p left): x(sx), y(sy), beaconX(bx), beaconY(by), range(r) {
		lines[0] = {top, right};
		lines[1] = {right, bottom};
		lines[2] = {bottom, left};
		lines[3] = {left, top};
	}
};

bool inRange(int x, int y);
pair<double, double> intersection(p A, p B, p C, p D);

vector<sensor> sensors;
vector<int> beacons;

int main() {

	ifstream in("input.txt");
	string line;

	int lx = INT_MAX;
	int hx = INT_MIN;

	while (in.peek() != EOF) {

		getline(in, line);
		regex rgx("Sensor at x=(-?[0-9]+), y=(-?[0-9]+): closest beacon is at x=(-?[0-9]+), y=(-?[0-9]+)");
		std::smatch match;
		regex_search(line, match, rgx);

		int x1 = stoi(match[1]);
		int y1 = stoi(match[2]);
		int x2 = stoi(match[3]);
		int y2 = stoi(match[4]);
		int range = abs(x1 - x2) + abs(y1 - y2);
		p top{ x1, y1 - range };
		p right{ x1 + range, y1 };
		p bottom{ x1, y1 + range };
		p left{ x1 - range, y1 };

		lx = min(lx, x1 - range);
		hx = max(hx, x1 + range);

		sensors.push_back(sensor(x1, y1, x2, y2, range, top, right, bottom, left));
		if(y2 == 2000000) beacons.push_back(x2);

	}

	int ans = 0;
	for (int x = lx; x <= hx; x++) {
		if (inRange(x, 2000000) && find(beacons.begin(), beacons.end(), x) == beacons.end()) ans++;
	}

	vector<p> possiblePoints;

	for (int i = 0; i < sensors.size() - 1; i++) {
		for (int j = i + 1; j < sensors.size(); j++) {
			for (int k = 0; k < 4; k++) {
				for (int l = 0; l < 4; l++) {
					if (k % 2 == l % 2) continue;
					pair<double, double> inter = intersection(sensors[i].lines[k].first, sensors[i].lines[k].second,
							sensors[j].lines[l].first, sensors[j].lines[l].second);

					int x = (int) ceil(inter.first);
					int y = (int) floor(inter.second);

					if ((x == INT_MAX && y == INT_MAX)
						|| x > max(sensors[i].lines[k].first.first, sensors[i].lines[k].second.first)
						|| x < min(sensors[i].lines[k].first.first, sensors[i].lines[k].second.first)
						|| y > max(sensors[i].lines[k].first.second, sensors[i].lines[k].second.second)
						|| y < min(sensors[i].lines[k].first.second, sensors[i].lines[k].second.second)) {
						continue;
					}

					possiblePoints.push_back({ x + 1, y });
					possiblePoints.push_back({ x - 1, y });
					possiblePoints.push_back({ x, y + 1 });
					possiblePoints.push_back({ x, y - 1 });
				}
			}
		}
	}

	cout << "In the row 2000000, " << ans << " positions cannot contain a beacon.\n";

	int x{};
	int y{};

	for (auto point : possiblePoints) {
		if (!inRange(point.first, point.second) 
			&& point.first >= 0 && point.first <= 4000000 && point.second >= 0 && point.second <= 4000000) {
			x = point.first;
			y = point.second;
			break;
		}
	}

	cout << "The distress beacon is at X: " << x << " Y: " << y << " and it's tuning frequency is " << x * 4000000ll + y;

}

bool inRange(int x, int y) {
	for (auto& sensor : sensors) {
		int distance = abs(x - sensor.x) + abs(y - sensor.y);
		if (distance <= sensor.range) return true;
	}
	return false;
}

pair<double, double> intersection(p A, p B, p C, p D) {
	double a = B.second - A.second;
	double b = A.first - B.first;
	double c = a * (A.first) + b * (A.second);

	double a1 = D.second - C.second;
	double b1 = C.first - D.first;
	double c1 = a1 * (C.first) + b1 * (C.second);

	double det = a * b1 - a1 * b;

	if (det == 0) return { FLT_MAX, FLT_MAX};

	else {
		double x = (b1 * c - b * c1) / det;
		double y = (a * c1 - a1 * c) / det;
		return { x, y };
	}
}