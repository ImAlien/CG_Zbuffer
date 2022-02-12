#include "defs.h"
#include "frame.h"
#include <iostream>
#include<set>
using namespace std;
extern FrameBuffer frameBuffer;

set<int> PolygonSet;
void zbuffer() {
	for (int y = WINDOW_HEIGHT - 1; y >= 0; y--) {
		zbufferInit();
		addActive(y);
		updateFrameBuffer(y);
		updateActivePolygonTable(y);
		updateActiveEdgeTable(y);
	}
}
void zbufferInit() {
	for (int i = 0; i < zbuffer_array.size(); i++) {//不能用resize，resize不会去除之前的值。
		zbuffer_array[i] = -1e8;
	}
}
void addActive(int y) {
	nodeClassifiedPolygon* pCP = PolygonTable[y]->next;
	while (pCP != nullptr) {
		nodeActivePolygon* cur = new nodeActivePolygon(pCP);
		addToPolygonTail(cur);
		PolygonSet.insert(cur->id);
		addActiveEdge(y, pCP);
		pCP = pCP->next;
	}
	//traverseActivePolygonColor(y);
	//traverseActivePolygon(y);
	//traverseEdgeTable(y);
	//traverseActiveEdge(y);
}
vector<nodeClassifiedEdge*> findEdges(int y, int pid) {
	vector<nodeClassifiedEdge*> res;
	if (PolygonSet.count(pid) == 0) return res;
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
nodeClassifiedEdge* findLeftEdge(int y, int pid) {
	if (PolygonSet.count(pid) == 0) return nullptr;
	nodeClassifiedEdge* res = nullptr;
	nodeClassifiedEdge* pCE = EdgeTable[y]->next;
	while (pCE != nullptr) {
		int eid = pCE->id;
		if (eid == pid) {
			if (res == nullptr) res = pCE;
			else if (pCE->x < res->x) res = pCE;
			else if (pCE->x == res->x && pCE->dx < res->dx) res = pCE;
		}
		pCE = pCE->next;
	}
	return res;
}
nodeClassifiedEdge* findRightEdge(int y, int pid) {
	if (PolygonSet.count(pid) == 0) return nullptr;
	nodeClassifiedEdge* res = nullptr;
	nodeClassifiedEdge* pCE = EdgeTable[y]->next;
	while (pCE != nullptr) {
		int eid = pCE->id;
		if (eid == pid) {
			if (res == nullptr) res = pCE;
			else if (pCE->x > res->x) res = pCE;
			else if (pCE->x == res->x && pCE->dx > res->dx) res = pCE;
		}
		pCE = pCE->next;
	}
	return res;
}
nodeActiveEdgePair* constructActiveEdge(nodeClassifiedPolygon* pCP, nodeClassifiedEdge* l, nodeClassifiedEdge* r, int y) {
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
	int n = find_edges.size();
	//if (n != 2) cerr << "活化多边形找到多个点";
	l = findLeftEdge(y, pid);
	r = findRightEdge(y, pid);
	if (l == nullptr || r == nullptr) return;
	if (l == r) return;
	//l = find_edges[0], r = find_edges[1];
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
		//frameBuffer.setPixel(x, y, EDGE_COLOR); 
		while (x < zbuffer_array.size() && x <= pAEP->xr) {
			//cout << z << ' ' << zbuffer_array[x] << endl;
			if (z > zbuffer_array[x]) {
				zbuffer_array[x] = z;
				frameBuffer.setPixel(x, y, pAEP->color);
			}
			z += pAEP->dzx;
			x++;
		}
		//frameBuffer.setPixel(x, y, pAEP->color);
		pAEP = pAEP->next;
	}
}
//void updateActiveEdgeTable(int y) {
//	nodeActiveEdgePair* pAEP = activeEdgePairHead->next;
//	while (pAEP != activeEdgePairTail) {
//		pAEP->dyl--;
//		pAEP->dyr--;
//		if (pAEP->dyl <= 0 && pAEP->dyr <= 0) {//左右两边同时扫描完
//			nodeClassifiedEdge *l, *r;
//			vector<nodeClassifiedEdge*> find_edges = findEdges(y, pAEP->id);
//			int n = find_edges.size();
//			if (n <= 1) {
//				//	if (find_edges.size() == 1)
//				//		cerr << "删除两边却只找到一点" << endl;
//				deleteEdge(pAEP);
//				auto pre = pAEP;
//				pAEP = pAEP->next;
//				free(pre);
//				continue;
//			}
//			else {
//				//	if (n > 2) cerr << "删除两边找到的点大于2个" << endl;
//				l = find_edges[0], r = find_edges[1];
//				if (l->x > r->x || (l->x == r->x && l->dx > r->dx)) {
//					nodeClassifiedEdge* t = l;
//					l = r;
//					r = t;
//				}
//				nodeClassifiedPolygon *pCP = id2Polygon[pAEP->id];
//				pAEP->xl = l->x;
//				pAEP->dxl = l->dx;
//				pAEP->dyl = l->dy;
//				pAEP->xr = r->x;
//				pAEP->dxr = r->dx;
//				pAEP->dyr = r->dy;
//				pAEP->zl = (-pCP->d - l->x * pCP->a - y * pCP->b) / pCP->c;
//				pAEP->dzx = (-pCP->a) / pCP->c;
//				pAEP->dzy = pCP->b / pCP->c;
//				pAEP = pAEP->next;
//			}
//		}
//		else {
//			if (pAEP->dyl <= 0) {//左边扫描完
//				nodeClassifiedEdge* l;
//				auto find_edge = findEdges(y, pAEP->id);
//				if (find_edge.size() > 0) {
//					l = find_edge[0];
//					pAEP->xl = l->x;
//					pAEP->dxl = l->dx;
//					pAEP->dyl = l->dy;
//				}
//				else {
//					deleteEdge(pAEP);
//					auto pre = pAEP;
//					pAEP = pAEP->next;
//					free(pre);
//					continue;
//				}
//			}
//			else {//左边未扫描完
//				pAEP->xl += pAEP->dxl;
//				pAEP->zl = pAEP->zl + pAEP->dxl * pAEP->dzx + pAEP->dzy;
//			}
//
//			if (pAEP->dyr <= 0) {//右边扫描完
//				nodeClassifiedEdge* r;
//				auto find_edge = findEdges(y, pAEP->id);
//				if (find_edge.size() > 0) {
//					r = find_edge[0];
//					pAEP->xr = r->x;
//					pAEP->dxr = r->dx;
//					pAEP->dyr = r->dy;
//				}
//				else {
//					deleteEdge(pAEP);
//					auto pre = pAEP;
//					pAEP = pAEP->next;
//					free(pre);
//					continue;
//				}
//			}
//			else {//右边未扫描完
//				pAEP->xr += pAEP->dxr;
//			}
//			pAEP = pAEP->next;
//		}
//	}
//}

void updateActiveEdgeTable(int y) {
	nodeActiveEdgePair* pAEP = activeEdgePairHead->next;
	while (pAEP != activeEdgePairTail) {
		pAEP->dyl--;
		pAEP->dyr--;
		if (pAEP->dyl < 0 && pAEP->dyr < 0) {//左右两边同时扫描完
			nodeClassifiedEdge *l, *r;
			vector<nodeClassifiedEdge*> find_edges = findEdges(y, pAEP->id);
			l = findLeftEdge(y, pAEP->id);
			r = findRightEdge(y, pAEP->id);
			//int n = find_edges.size();
			if (l == nullptr && r == nullptr) {
				//	if (find_edges.size() == 1)
				//		cerr << "删除两边却只找到一点" << endl;
				deleteEdge(pAEP);
				auto pre = pAEP;
				pAEP = pAEP->next;
				free(pre);
				continue;
			}
			else {
				//l = find_edges[0], r = find_edges[1];
				if ((l->x == r->x && l->dx > r->dx)) {
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
			if (pAEP->dyl < 0) {//左边扫描完
				nodeClassifiedEdge* l = findLeftEdge(y, pAEP->id);
				if (l) {
					if (l->x == pAEP->xl) {
						pAEP->dxl = l->dx;
						pAEP->dyl = l->dy;
					}
				}
				else {
					deleteEdge(pAEP);
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

			if (pAEP->dyr < 0) {//右边扫描完
				nodeClassifiedEdge* r = findRightEdge(y, pAEP->id);
				//auto find_edge = findEdges(y, pAEP->id);
				if (r) {
					//r = find_edge[0];
					if (r->x == pAEP->xr) {
						pAEP->dxr = r->dx;
						pAEP->dyr = r->dy;
					}
				}
				else {
					deleteEdge(pAEP);
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
void updateActivePolygonTable(int y) {
	nodeActivePolygon* p = activePolygonHead->next;
	while (p != activePolygonTail) {
		p->dy -= 1;
		if (p->dy < 0)
		{
			PolygonSet.erase(p->id);
			deletePolygon(p);
			nodeActivePolygon* pre = p;
			p = p->next;
			free(pre);
			continue;
		}
		p = p->next;
	}
}