#include "simpleMath.h"

bool eq3(const Vec3& a, const Vec3& b)
{
	return a.x == b.x
		&& a.y == b.y
		&& a.z == b.z;
}

double dot2(Vec2 a, Vec2 b)
{
    return a.x*b.x + a.y*b.y;
};

double dot3(Vec3 a, Vec3 b)
{
    return a.x*b.x + a.y*b.y + a.z*b.z;
};

double dot4(Vec4 a, Vec4 b)
{
    return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
};

Vec3 add3(const Vec3& a, const Vec3& b)
{
	return Vec3(a.x + b.x,
				a.y + b.y,
				a.z + b.z);
}

Vec3 sub3(const Vec3& a, const Vec3& b)
{
	return Vec3(a.x - b.x,
				a.y - b.y,
				a.z - b.z);
}
