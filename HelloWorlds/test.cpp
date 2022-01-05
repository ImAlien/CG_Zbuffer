#include "defs.h"
#include <iostream>
#include <string>
using namespace std;
void traversePolygonTable() {
	int sum = 0;
	for (int y = 0; y < WINDOW_HEIGHT; y++) {
		auto p = PolygonTable[y]->next;
		int cnt = 0;
		while (p) {
			cnt++;
			p = p->next;
		}
		cout << "第" << y << "行: " << cnt << "个多边形" << endl;
		sum += cnt;
	}
	cout << "总共" << sum << "个多边形" << endl;
}
void traverseEdgeTable() {
	int sum = 0;
	for (int y = 0; y < WINDOW_HEIGHT; y++) {
		auto p = EdgeTable[y]->next;
		int cnt = 0;
		while (p) {
			cnt++;
			p = p->next;
		}
		cout << "第" << y << "行: " << cnt << "条边" << endl;
		sum += cnt;
	}
	cout << "总共" << sum << "条边" << endl;
}

void print(string s) {
	cout << s << endl;
}
void traverseActivePolygon(int y) {
	auto p = activePolygonHead->next;
	int cnt = 0;
	while (p != activePolygonTail) {
		cnt++;
		p = p->next;
	}
	print("当前" + to_string(y) +"下总共" + to_string(cnt) + "个活化多边形");
}
void traverseActiveEdge(int y) {
	auto p = activeEdgePairHead->next;
	int cnt = 0;
	while (p != activeEdgePairTail) {
		cnt++;
		p = p->next;
	}
	print("当前" + to_string(y) + "下总共" + to_string(cnt) + "个活化边");
}
