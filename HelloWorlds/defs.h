#pragma once
#include <gl/glut.h>
#include <vector>
#include <algorithm>

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
	nodeActivePolygon() :a(0), b(0), c(0), d(0), id(-1), dy(-1), color(0, 0, 0), next(nullptr) {};
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
};

// func.cpp
int transfer(double x);
WPoint transfer(Point& p);
Vec3 cross(Vec3& v1, Vec3& v2);
WPoint cross(WPoint& v1, WPoint& v2);
Vec4 calFactor(std::vector<Point>& points);


void zbuffer();

void clearData();