#pragma once
#include <gl/glut.h>
#include <vector>
#include <algorithm>
#include <map>
#include <string>
extern int WINDOW_HEIGHT, WINDOW_WIDTH;

struct Vector3{
	GLdouble x, y,z;
	Vector3(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}
};
typedef struct Vector3 Vec3;
typedef struct Vector3 Point;
Vec3 operator +(const Vec3& A, const Vec3 B); //���ṹ�嶨��ӷ��� 
Vec3 operator - (const Vec3& A, const Vec3 B); //���ṹ�嶨������� 

struct WPoint {
	int x, y, z;
	WPoint(int x_, int y_, int z_) : x(x_), y(y_), z(z_) {}
};
struct Vector4 {
	GLdouble a, b, c, d;
	Vector4(){}
};
typedef struct Vector4 Vec4;

struct Color {
	GLubyte r, b, g;
	Color() :r(0), b(0), g(0) {};
	Color(int r_, int b_, int g_) :r(r_), b(b_), g(g_) {}
	Color(double R, double B, double G) :r(GLubyte(R)), b(GLubyte(B)), g(GLubyte(G)) {};
};
typedef struct Color Color;
extern Color EDGE_COLOR;

struct Face {
	std::vector<Point> points;
	double y_max;
	double y_min;
	Face() :y_max(INT_MIN), y_min(INT_MAX) {};
};
struct Edge {
	Point p1, p2;
	double y_max;
};
struct nodeClassifiedPolygon {//��������
	GLdouble a, b, c, d;
	GLint id;
	GLint dy;//��Խ��ɨ������Ŀ
	Color color;
	nodeClassifiedPolygon* next;
};

struct nodeClassifiedEdge {// �����
	GLdouble x;
	GLdouble dx;
	//COORD_TYPE e_len;//����ˮƽ��ʱ��e_lenΪ��0ֵ������e_len=0
	GLint dy;
	GLint id;
	bool used;
	nodeClassifiedEdge* next;
};

struct nodeActivePolygon { // ������
	GLdouble a, b, c, d;
	GLint id;
	GLint dy;
	Color color;
	nodeActivePolygon* next;
	nodeActivePolygon* pre;
	nodeActivePolygon() :a(0), b(0), c(0), d(0), id(-1), dy(-1), color(0, 0, 0), next(nullptr),pre(nullptr) {};
	nodeActivePolygon(nodeClassifiedPolygon* t) {
		a = t->a, b = t->b, c = t->c, d = t->d;
		id = t->id;
		dy = t->dy - 1;
		color = t->color;
	}
};

struct nodeActiveEdgePair { //��߶�
	GLdouble xl;
	GLdouble dxl;//����ɨ���߽���x����֮��
	GLint dyl;//����ı߿�Խ��ɨ������Ŀ
	GLdouble xr;
	GLdouble dxr;
	GLint dyr;
	GLdouble zl;
	GLdouble dzx;
	GLdouble dzy;
	GLint id;
	Color color;
	nodeActiveEdgePair* next;
	nodeActiveEdgePair* pre;
};

// func.cpp
int transfer(double x);
WPoint transfer(Point& p);
Vec3 cross(Vec3& v1, Vec3& v2);
WPoint cross(WPoint& v1, WPoint& v2);
Vec4 calFactor(std::vector<Point>& points);
Color getColor(Vec4& v);

//zbuffer.cpp
extern std::vector<nodeClassifiedPolygon*> PolygonTable;
extern std::vector<nodeClassifiedPolygon*> PolygonTableTail;
extern std::vector<nodeClassifiedEdge*> EdgeTable;
extern std::vector<nodeClassifiedEdge*> EdgeTableTail;
extern std::vector<float> zbuffer_array;
extern nodeActivePolygon* activePolygonHead;
extern nodeActivePolygon* activePolygonTail;
extern nodeActiveEdgePair* activeEdgePairHead;
extern nodeActiveEdgePair* activeEdgePairTail;
extern std::map<int, nodeClassifiedPolygon*> id2Polygon;

void zbufferInit();
void zbuffer();
void addActive(int y);
void addActiveEdge(int y, nodeClassifiedPolygon* pCP);
void updateFrameBuffer(int y);
void updateActivePolygonTable(int y);
void updateActiveEdgeTable(int y);

//nodelist.cpp
void addToPolygonTail(nodeActivePolygon* p);
void addToEdgePairTail(nodeActiveEdgePair* p);
void deletePolygon(nodeActivePolygon* p);
void deleteEdge(nodeActiveEdgePair* p);
//clear.cpp
void clearData();
void clearClassifiedPolygon();
void clearClassifiedEdge();
void clearActivePolygonTable();//��ջ����α�
void clearActiveEdgeTable();//��ջ�߱�

//test.cpp
void traversePolygonTable();
void traverseEdgeTable();
void print(std::string s);
void traverseActivePolygon(int y);
void traverseActiveEdge(int y);
