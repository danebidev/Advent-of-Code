#include <bits/stdc++.h>
#include <chrono>

using namespace std;

struct element {

	int num = -1;
	deque<element*> elems;

	element(int n) : num(n) {
		elems = deque<element*>();
	}
	element() {
		elems = deque<element*>();
	}

};

int compare(element left, element right);

int main() {

	auto start = chrono::steady_clock::now();

	ifstream in("input.txt");
	string line;

	int sum = 0;
	int index = 1;

	vector<element> packets;

	while (in.peek() != EOF) {

		element root1;
		element root2;

		stack<element*> elements;

		for (int n = 0; n < 2; n++) {
			getline(in, line);
			for (int i = 0; i < line.length(); i++) {
				if (line[i] == ',') continue;
				else if (line[i] >= 48 && line[i] <= 57) {
					string str = string(1, line[i]);
					while (i + 1 < line.length() && line[i + 1] >= 48 && line[i + 1] <= 57) {
						i++;
						str += string(1, line[i]);
					}
					elements.top()->elems.push_back(new element(stoi(str)));
				}
				else if (line[i] == '[') {
					elements.push(new element());
				}
				else if (line[i] == ']') {
					element* e = elements.top();
					elements.pop();
					if (!elements.empty()) {
						elements.top()->elems.push_back(e);
					}
					else {
						n == 0 ? root1 = *e : root2 = *e;
					}
				}
			}
		}
		getline(in, line);
		if (compare(root1, root2) == 1) sum += index;
		index++;

		packets.push_back(root1);
		packets.push_back(root2);
	}

	element* dec = new element();
	dec->elems.push_back(new element());
	dec->elems[0]->elems.push_back(new element(2));

	int ans = 1;
	int cur;

	for (int n = 0; n < 2; n++) {
		cur = 0;
		for (int i = 0; i < packets.size(); i++) {
			if (compare(packets[i], *dec) == 1) cur++;
		}
		ans *= cur + n + 1;
		dec->elems[0]->elems[0]->num = 6;
	}

	cout << "The sum of indices of the packets in order is: " << sum << endl;
	cout << "The decoder key for the distress signal is: " << ans << endl;

	auto end = chrono::steady_clock::now();
	cout << "Time:\n"
		<< chrono::duration_cast<chrono::microseconds>(end - start).count() << " us\n"
		<< chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms";

}

int compare(element left, element right) {
	while (!left.elems.empty()) {

		if (right.elems.empty()) return -1;

		element e1 = *left.elems.front();
		element e2 = *right.elems.front();
		int res = 0;

		if (e1.num != -1 && e2.num != -1) {
			if (e1.num != e2.num) return e1.num < e2.num ? 1 : -1;
		}
		else if (e1.num != -1) {
			element l = element();
			l.elems.push_back(new element(e1.num));
			res = compare(l, e2);
		}
		else if (e2.num != -1) {
			element l = element();
			l.elems.push_back(new element(e2.num));
			res = compare(e1, l);
		}
		else {
			res = compare(e1, e2);
		}
		if (res != 0) return res;

		left.elems.pop_front();
		right.elems.pop_front();
	}
	if (!right.elems.empty()) return 1;
	return 0;
}