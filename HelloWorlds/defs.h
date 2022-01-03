#pragma once
#include <gl/glut.h>
#include <vector>
struct Vector3{
	GLdouble x, y;
	GLdouble z;
	Vector3(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}
};
typedef struct Vector3 Vec3;
typedef struct Vector3 Point;

struct Color {
	GLubyte r, b, g;
	Color(int r_, int b_, int g_) :r(r_), b(b_), g(g_) {}
};
typedef struct Color Color;

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
struct nodeClassifiedPolygon {//分类多边形
	GLdouble a, b, c, d;
	GLint id;
	GLint dy;//跨越的扫描线数目
	Color color;
	nodeClassifiedPolygon* next;
};

struct nodeClassifiedEdge {// 分类边
	GLdouble x;
	GLdouble dx;
	//COORD_TYPE e_len;//边是水平边时，e_len为非0值，否则e_len=0
	GLint dy;
	GLint id;
	bool used;
	nodeClassifiedEdge* next;
};

struct nodeActivePolygon { // 活化多边形
	GLdouble a, b, c, d;
	GLint id;
	GLint dy;
	Color color;
	nodeActivePolygon* next;
};

struct nodeActiveEdgePair { //活化边对
	GLdouble xl;
	GLdouble dxl;//相邻扫描线交点x坐标之差
	GLint dyl;//靠左的边跨越的扫描线数目
	GLdouble xr;
	GLdouble dxr;
	GLint dyr;
	GLdouble zl;
	GLdouble dzx;
	GLdouble dzy;
	GLint id;
	Color color;
	nodeActiveEdgePair* next;
};
