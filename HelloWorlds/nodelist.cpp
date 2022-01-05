#include "defs.h"
//�������

void addToPolygonTail(nodeActivePolygon* p) {//���������β��
	p->next = activePolygonTail;
	p->pre = activePolygonTail->pre;
	activePolygonTail->pre->next = p;
	activePolygonTail->pre = p;
}

void addToEdgePairTail(nodeActiveEdgePair* p) {//�����߱�β��
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
