#include "defs.h"
//链表操作

void addToPolygonTail(nodeActivePolygon* p) {//加入活化多边形尾部
	p->next = activePolygonTail;
	p->pre = activePolygonTail->pre;
	activePolygonTail->pre->next = p;
	activePolygonTail->pre = p;
}

void addToEdgePairTail(nodeActiveEdgePair* p) {//加入活化边表尾部
	p->next = activeEdgePairTail;
	p->pre = activeEdgePairTail->pre;
	activeEdgePairTail->pre->next = p;
	activeEdgePairTail->pre = p;
}
void deletePolygon(nodeActivePolygon* p) {
	p->pre->next = p->next;
	p->next->pre = p->pre;
}
void deleteEdge(nodeActiveEdgePair* p) {
	p->pre->next = p->next;
	p->next->pre = p->pre;
}
