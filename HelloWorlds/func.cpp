#include "defs.h"
#include <iostream>
#include "obj.h"
using namespace std;
extern Obj obj;

int transferX(double x) {
	double y_center = (obj.g_ymax + obj.g_ymin) / 2;
	double x_center = (obj.g_xmax + obj.g_xmin) / 2;
	double width = (obj.g_xmax - obj.g_xmin);
	double height = (obj.g_ymax - obj.g_ymin);
	double l = x_center - width;
	double down = y_center - height;
	double scale1 = WINDOW_HEIGHT*0.5/height;
	double scale2 = WINDOW_WIDTH*0.5/width;
	double scale = min(scale1, scale2);
	return (x - l) * scale;
}
int transferY(double y) {
	double y_center = (obj.g_ymax + obj.g_ymin) / 2;
	double x_center = (obj.g_xmax + obj.g_xmin) / 2;
	double width = (obj.g_xmax - obj.g_xmin);
	double height = (obj.g_ymax - obj.g_ymin);
	double l = x_center - width;
	double down = y_center - height;
	double scale1 = WINDOW_HEIGHT * 0.5 / height;
	double scale2 = WINDOW_WIDTH * 0.5 / width;
	double scale = min(scale1, scale2);
	return (y - down) * scale;
}
int transferZ(double z) {
	double y_center = (obj.g_ymax + obj.g_ymin) / 2;
	double x_center = (obj.g_xmax + obj.g_xmin) / 2;
	double width = (obj.g_xmax - obj.g_xmin);
	double height = (obj.g_ymax - obj.g_ymin);
	double l = x_center - width;
	double down = y_center - height;
	double scale1 = WINDOW_HEIGHT * 0.5 / height;
	double scale2 = WINDOW_WIDTH * 0.5 / width;
	double scale = min(scale1, scale2);
	return z * scale;
}
WPoint transfer(Point& p) {
	return WPoint(transferX(p.x), transferY(p.y), transferZ(p.z));
}
Vec3 cross(Vec3& v1, Vec3& v2) {
	return Vec3{ v1.y * v2.z - v1.z * v2.y,
				v1.z * v2.x - v1.x * v2.z,
				v1.x * v2.y - v1.y * v2.x };
}
WPoint cross(WPoint& v1, WPoint& v2) {
	return WPoint{ v1.y * v2.z - v1.z * v2.y,
				v1.z * v2.x - v1.x * v2.z,
				v1.x * v2.y - v1.y * v2.x };
}
Vec4 calFactor(vector<Point>& points) {
	Vec4 ans;
	Point& A = points[0];
	Vec3 v1 = points[1] - points[0];
	Vec3 v2 = points[2] - points[1];
	Vec3 v3 = cross(v1, v2);
	//print(v1);
	//print(v2);
	//print(v3);
	ans.a = v3.x;
	ans.b = v3.y;
	ans.c = v3.z;
	int d = 0.0 - v3.x * A.x - v3.y*A.y - v3.z * A.z;
	ans.d = d;
	return ans;
}

Vec3 operator +(const Vec3& A, const Vec3 B) //给结构体定义加法； 
{
	return Vec3(A.x + B.x, A.y + B.y, A.z + B.z);
}
Vec3 operator - (const Vec3& A, const Vec3 B) //给结构体定义减法； 
{
	return Vec3(A.x - B.x, A.y - B.y, A.z - B.z);
}

Color getColor(Vec4& v) {
	v.a = fabs(v.a);
	v.b = fabs(v.b);
	v.c = fabs(v.c);
	//cout << v.a << ' ' << v.b << ' ' << v.c << endl;
	//与z轴的夹角的余弦值
	GLdouble costheta = v.c / sqrt(v.a*v.a + v.b*v.b + v.c*v.c);
	//GLdouble costheta = v.c / v.a + v.b + v.c;
	//cout << 255 * costheta << 255 * costheta << 255 * costheta << endl;
	return Color( 255 * costheta, 255 * costheta, 255 * costheta);
}