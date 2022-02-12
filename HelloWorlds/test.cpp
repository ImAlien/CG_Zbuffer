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
		cout << "��" << y << "��: " << cnt << "�������" << endl;
		sum += cnt;
	}
	cout << "�ܹ�" << sum << "�������" << endl;
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
		cout << "��" << y << "��: " << cnt << "����" << endl;
		sum += cnt;
	}
	cout << "�ܹ�" << sum << "����" << endl;
}
void traverseEdgeTable(int y) {
	auto p = EdgeTable[y]->next;
	cout << "-----------------------------------" << y << "edges" << endl;
	while (p) {
		cout << p->x << ' ' << p->id << endl;
		p = p->next;
	}
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
	print("��ǰ" + to_string(y) +"���ܹ�" + to_string(cnt) + "��������");
}
void traverseActiveEdge(int y) {
	auto p = activeEdgePairHead->next;
	int cnt = 0;
	while (p != activeEdgePairTail) {
		cnt++;
		p = p->next;
	}
	print("��ǰ" + to_string(y) + "���ܹ�" + to_string(cnt) + "�����");
}

void print(Color& c) {
	cout << "r: " << (int)c.r << "  b:  " << (int)c.b << "  g :  " << (int)c.g << endl;
}
void traverseActivePolygonColor(int y) {
	auto p = activePolygonHead->next;
	int cnt = 0;
	while (p != activePolygonTail) {
		cnt++;
		print("��������:");
		cout << cnt << ' ';
		print(p->color);
		p = p->next;
	}
}

void print(Vec3& v) {
	cout << v.x<<' ' << v.y<<' ' << v.z << endl;
}