#include <bits/stdc++.h>
#include <chrono>
#include <regex>

using namespace std;

struct valve {
	string name;
	int flow;
	vector<string> connectsTo;
	bool open = false;

	valve(string n, int f, vector<string> valves) : name(n), flow(f), connectsTo(valves) {}
};

void bfs(valve);
int maxPressure(int, string, int, int);

map<string, valve*> valves;
map<string, map<string, int>> distances;

int main() {

	ifstream in("input.txt");
	string line;

	while (in.peek() != EOF) {
		getline(in, line);

		regex rgx("Valve ([A-Z]{2}) has flow rate=([0-9]+); tunnels? leads? to valves? (([A-Z]{2}(, )?)+)");
		smatch match;
		regex_search(line, match, rgx);
		string v = match[3].str();
		string name = match[1];
		int flow = stoi(match[2]);
		int size = (v.size() + 2) / 4;
		vector<string> va(size);

		int i = 0;
		int pos;
		while ((pos = v.find(", ") != v.npos)) {
			va[i] = v.substr(0, pos + 1);
			v.erase(0, pos + 3);
			i++;
		}
		va[i] = v;

		valve* val = new valve(name, flow, va);
		valves[name] = val;
	}

	for(auto v : valves) {
		bfs(*v.second);
		for(auto valve : valves) {
			if(valve.second->flow == 0 || valve.first == v.first) distances[v.first].erase(valve.first);
		}
	}

	cout << "1: " << maxPressure(29, "AA", 0, 0) << "\n";

}

int maxPressure(int time, string curValve, int pressPerMin, int curPress) {
	
	curPress += pressPerMin;
	if(time <= 0) return curPress;
	int maxPress = curPress;
	bool stop = true;
	
	for(auto valve : distances[curValve]) {
		if(valves[valve.first]->open || valve.second + 1 > time) continue;
		stop = false;
		valves[valve.first]->open = true;
		maxPress = max(maxPress, maxPressure(time - valve.second - 1, valve.first, pressPerMin + valves[valve.first]->flow, curPress + pressPerMin * valve.second));
		valves[valve.first]->open = false;
	}

	return maxPress + pressPerMin * time * stop;

}

void bfs(valve valve) {
	queue<pair<int, string>> q;
	q.push(make_pair(0, valve.name));
	distances[valve.name][valve.name] = 0;

	while(!q.empty()) {
		pair<int, string> p = q.front();
		q.pop();
		for(string adj : valves[p.second]->connectsTo) {
			if(distances[valve.name].find(adj) == distances[valve.name].end()) {
				distances[valve.name][adj] = p.first + 1;
				q.push(make_pair(p.first + 1, adj));
			}
		}
	}
}