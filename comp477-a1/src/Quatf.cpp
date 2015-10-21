#include "Quatf.h"

#include <cmath>
#include "vmath\vmath.h"

///////////////////////////////////////
//			CONSTRUCTORS
///////////////////////////////////////

Quatf::Quatf()
	: w{ 0 }
	, v{}
{ }

Quatf::Quatf(float w, float x, float y, float z)
	: Quatf(w, Vector3f{ x, y, z })
{ }

Quatf::Quatf(float w, const Vector3f& v)
	: w{ w }
	, v{ v }
{ }

///////////////////////////////////////
//			PUBLIC METHODS
///////////////////////////////////////

auto Quatf::length() const -> float
{
	return std::sqrtf(w * w + v.lengthSq());
}

auto Quatf::normalize() -> void
{
	auto len = length();
	w /= len;
	v /= len;
}

auto Quatf::normalized() const -> Quatf
{
	Quatf copy{ w, v };
	copy.normalize();
	return copy;
}

auto Quatf::mat4() const -> Matrix4f
{
	Matrix4f ret;

	auto xx = v.x * v.x;
	auto xy = v.x * v.y;
	auto xz = v.x * v.z;
	auto xw = v.x * w;

	auto yy = v.y * v.y;
	auto yz = v.y * v.z;
	auto yw = v.y * w;

	auto zz = v.z * v.z;
	auto zw = v.z * w;

	ret.at(0, 0) = 1 - 2 * (yy + zz);
	ret.at(1, 0) = 2 * (xy - zw);
	ret.at(2, 0) = 2 * (xz + yw);
	ret.at(3, 0) = 0;

	ret.at(0, 1) = 2 * (xy + zw);
	ret.at(1, 1) = 1 - 2 * (xx + zz);
	ret.at(2, 1) = 2 * (yz - xw);
	ret.at(3, 1) = 0;

	ret.at(0, 2) = 2 * (xz - yw);
	ret.at(1, 2) = 2 * (yz + xw);
	ret.at(2, 2) = 1 - 2 * (xx + yy);
	ret.at(3, 2) = 0;

	ret.at(0, 3) = 0;
	ret.at(1, 3) = 0;
	ret.at(2, 3) = 0;
	ret.at(3, 3) = 1;

	return ret;
}


auto Quatf::rotatePoint(const Vector3f& p) const -> Vector3f
{
	return rotatePoint(Quatf{ 0, p });
}

auto Quatf::rotatePoint(const Quatf& q) const -> Vector3f
{
	auto& rot = *this;
	auto result = rot * q * ~rot;
	return result.v;
}

auto Quatf::operator*(const Quatf& rhs) const -> Quatf
{
	const auto& lhs = *this;

	float w = lhs.w * rhs.w - lhs.v.x * rhs.v.x - lhs.v.y * rhs.v.y - lhs.v.z * rhs.v.z;
	float x = lhs.w * rhs.v.x + lhs.v.x * rhs.w + lhs.v.y * rhs.v.z - lhs.v.z * rhs.v.y;
	float y = lhs.w * rhs.v.y - lhs.v.x * rhs.v.z + lhs.v.y * rhs.w + lhs.v.z * rhs.v.x;
	float z = lhs.w * rhs.v.z + lhs.v.x * rhs.v.y - lhs.v.y * rhs.v.x + lhs.v.z * rhs.w;

	return Quatf{ w, x, y, z };
}

auto Quatf::operator~() const -> Quatf
{
	return Quatf{ w, -v };
}

auto Quatf::operator/(float f) -> Quatf
{
	return Quatf{ w / f, v / f };
}

auto Quatf::angleAxis(float angleDeg, float x, float y, float z) -> Quatf
{
	return Quatf::angleAxis(angleDeg, Vector3f{ x, y, z });
}

auto Quatf::angleAxis(float angleDeg, Vector3f axis) -> Quatf
{
	axis.normalize();
	auto angleRad = static_cast<float>(DEG2RAD(angleDeg));
	auto sa2 = std::sinf(angleRad / 2);
	auto ca2 = std::cosf(angleRad / 2);
	return Quatf{ ca2, axis * sa2 };
}

///////////////////////////////////////
//		NON-MEMBER FUNCTIONS
///////////////////////////////////////

auto operator*(float f, const Quatf& q) -> Quatf
{
	return q * f;
}

auto operator*(const Quatf& q, float f) -> Quatf
{
	return Quatf{ q.w * f, q.v * f };
}