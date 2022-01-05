#include "defs.h"

void clearData() {
	clearActivePolygonTable();//��ջ����α�
	clearActiveEdgeTable();//��ջ�߱�
	clearClassifiedPolygon();
	clearClassifiedEdge();
}
void clearActiveEdgeTable() {//��ջ�߱�
	nodeActiveEdgePair *p = activeEdgePairHead->next;
	while (p != activeEdgePairTail) {
		auto pre = p;
		deleteEdge(p);
		p = pre->next;
		free(pre);
	}
}

void clearActivePolygonTable() {//��ջ����α�
	nodeActivePolygon *p = activePolygonHead->next;
	while (p != activePolygonTail) {
		auto pre = p;
		deletePolygon(p);
		p = pre->next;
		free(pre);
	}
}

void clearClassifiedPolygon() {
	nodeClassifiedPolygon *p, *q; /*int _cnt = 0;*/
	for (int i = 0; i < PolygonTable.size(); i++) {
			p = PolygonTable[i]->next;
			PolygonTable[i]->next = nullptr;
			while (p) {
				q = p->next;
				free(p);
				p = q;
			}
	}
	//cout<<"del cnt: " <<_cnt <<endl;
}

void clearClassifiedEdge() {
	nodeClassifiedEdge *p, *q;
	for (GLuint i = 0; i < EdgeTable.size(); i++) {
		p = EdgeTable[i]->next;
		EdgeTable[i]->next = nullptr;
		while (p) {
			q = p->next;
			free(p);
			p = q;
		}
	}
}