#include "defs.h"
using namespace std;

int transfer(double x) {
	int scale = WINDOW_HEIGHT > WINDOW_WIDTH ? WINDOW_WIDTH : WINDOW_HEIGHT;
	return (x + 1) * scale / 2;
}
WPoint transfer(Point& p) {
	return WPoint(transfer(p.x), transfer(p.y), transfer(p.z));
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
	Vec3 v1 = points[0] - points[1];
	Vec3 v2 = points[2] - points[2];
	Vec3 v3 = cross(v1, v2);
	ans.a = v3.x;
	ans.b = v3.y;
	ans.c = v3.z;
	int d = 0.0 - v3.x * A.x - v3.y*A.y - v3.z * A.z;
	ans.d = d;
	return ans;
}

Vec3 operator +(const Vec3& A, const Vec3 B) //给结构体定义加法； 
{
	return Vec3(A.x + B.x, A.y + B.y, A.z + B.y);
}
Vec3 operator - (const Vec3& A, const Vec3 B) //给结构体定义减法； 
{
	return Vec3(A.x - B.x, A.y - B.y, A.z - B.y);
}