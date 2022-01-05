#include "defs.h"
#include "frame.h"
#include <iostream>
using namespace std;
extern FrameBuffer frameBuffer;

void zbuffer() {
	
	for (int y = 3; y >= 0; y--) {
		zbufferInit();
		addActive(y);
		updateFrameBuffer(y);
		//updateActivePolygonTable(y);
		//updateActiveEdgeTable(y);
	}
}
void zbufferInit() {
	zbuffer_array.resize(WINDOW_WIDTH, -1e9);
}
void addActive(int y) {
	nodeClassifiedPolygon* pCP = PolygonTable[y]->next;
	while (pCP != nullptr) {
		nodeActivePolygon* cur = new nodeActivePolygon(pCP);
		addToPolygonTail(cur);
		addActiveEdge(y, pCP);
		pCP = pCP->next;
	}
	//traverseActivePolygon(y);
	//traverseActiveEdge(y);
}
vector<nodeClassifiedEdge*> findEdges(int y, int pid) {
	vector<nodeClassifiedEdge*> res;
	nodeClassifiedEdge* pCE = EdgeTable[y]->next;
	while (pCE != nullptr) {
		int eid = pCE->id;
		if (eid == pid) {
			res.push_back(pCE);
		}
		pCE = pCE->next;
	}
	return res;
}
nodeActiveEdgePair* constructActiveEdge(nodeClassifiedPolygon* pCP,nodeClassifiedEdge* l, nodeClassifiedEdge* r,int y) {
	nodeActiveEdgePair* pAEP = new nodeActiveEdgePair();
	pAEP->xl = l->x;
	pAEP->dxl = l->dx;
	pAEP->dyl = l->dy;
	pAEP->xr = r->x;
	pAEP->dxr = r->dx;
	pAEP->dyr = r->dy;
	pAEP->zl = (-pCP->d - l->x * pCP->a - y * pCP->b)*1.0 / pCP->c;
	pAEP->dzx = (-pCP->a)*1.0 / pCP->c;
	pAEP->dzy = pCP->b / pCP->c;
	pAEP->id = l->id;
	pAEP->color = pCP->color;
	pAEP->next = nullptr;
	return pAEP;
}
void addActiveEdge(int y, nodeClassifiedPolygon* pCP) {//活化多边形的两边加入活化边表
	nodeClassifiedEdge* pCE = EdgeTable[y]->next;
	nodeClassifiedEdge* l = nullptr, *r = nullptr;
	int pid = pCP->id;
	vector<nodeClassifiedEdge*> find_edges = findEdges(y, pid);
	//cout << "find_deges:" << find_edges.size() << endl;
	l = find_edges[0], r = find_edges[1];
	if (l->x > r->x || (l->x == r->x && l->dx > r->dx)) {
		swap(l, r);
	}
	//构造活化边
	nodeActiveEdgePair* pAEP = constructActiveEdge(pCP, l, r, y);
	addToEdgePairTail(pAEP);
}

void updateFrameBuffer(int y) {
	nodeActiveEdgePair* pAEP = activeEdgePairHead->next;
	while (pAEP != activeEdgePairTail) {
		GLdouble z = pAEP->zl;
		int x = pAEP->xl;
		frameBuffer.setPixel(x, y, EDGE_COLOR);
		while (x < zbuffer_array.size() && x <pAEP->xr) {
			z += pAEP->dzx;
			x++;
			if (z > zbuffer_array[x]) {
				zbuffer_array[x] = z;
				frameBuffer.setPixel(x, y, pAEP->color);
			}
		}
		frameBuffer.setPixel(x, y, EDGE_COLOR);
		pAEP = pAEP->next;
	}
}
void updateActivePolygonTable(int y) {
	nodeActiveEdgePair* pAEP = activeEdgePairHead->next;
	while (pAEP) {
		pAEP->dyl--;
		pAEP->dyr--;
		if (pAEP->dyl <= 0 && pAEP->dyr <= 0) {//左右两边同时扫描完
			nodeClassifiedEdge *l, *r;
			vector<nodeClassifiedEdge*> find_edges = findEdges(y, pAEP->id);
			if (find_edges.size() == 0) {
				deleteEdge(pAEP);
				auto pre = pAEP;
				pAEP = pAEP->next;
				free(pre);
				continue;
			}
			else {
				l = find_edges[0], r = find_edges[1];
				if (l->x > r->x || (l->x == r->x && l->dx > r->dx)) {
					nodeClassifiedEdge* t = l;
					l = r;
					r = t;
				}
				nodeClassifiedPolygon *pCP = id2Polygon[pAEP->id];
				pAEP->xl = l->x;
				pAEP->dxl = l->dx;
				pAEP->dyl = l->dy;
				pAEP->xr = r->x;
				pAEP->dxr = r->dx;
				pAEP->dyr = r->dy;
				pAEP->zl = (-pCP->d - l->x * pCP->a - y * pCP->b) / pCP->c;
				pAEP->dzx = (-pCP->a) / pCP->c;
				pAEP->dzy = pCP->b / pCP->c;
				pAEP = pAEP->next;
			}
		}
		else {
			if (pAEP->dyl <= 0) {//左边扫描完
				nodeClassifiedEdge* l;
				auto find_edge = findEdges(y, pAEP->id);
				if (find_edge.size() > 0) {
					l = find_edge[0];
					pAEP->xl = l->x;
					pAEP->dxl = l->dx;
					pAEP->dyl = l->dy;
				}
				else {
					auto pre = pAEP;
					pAEP = pAEP->next;
					free(pre);
					continue;
				}
			}
			else {//左边未扫描完
				pAEP->xl += pAEP->dxl;
				pAEP->zl = pAEP->zl + pAEP->dxl * pAEP->dzx + pAEP->dzy;
			}

			if (pAEP->dyr <= 0) {//右边扫描完
				nodeClassifiedEdge* r;
				auto find_edge = findEdges(y, pAEP->id);
				if (find_edge.size() > 0) {
					r = find_edge[0];
					pAEP->xl = r->x;
					pAEP->dxl = r->dx;
					pAEP->dyl = r->dy;
				}
				else {
					auto pre = pAEP;
					pAEP = pAEP->next;
					free(pre);
					continue;
				}
			}
			else {//右边未扫描完
				pAEP->xr += pAEP->dxr;
			}
			pAEP = pAEP->next;
		}
	}
}
void updateActiveEdgeTable(int y) {
	nodeActivePolygon* p = activePolygonHead->next;
	while (p) {
		p->dy -= 1;
		if (p->dy <= 0) deletePolygon(p);
		nodeActivePolygon* pre = p;
		p = p->next;
		free(pre);
	}
}