#pragma once

#include "vmath\vmath.h"

class Quatf
{
public:
	float w;
	Vector3f v;

	Quatf();
	Quatf(float w, float x, float y, float z);
	Quatf(float w, const Vector3f& v);
	Quatf(const Quatf& other);

	auto rotatePoint(const Vector3f& p) const -> Vector3f;
	auto rotatePoint(const Quatf& q) const -> Vector3f;

	auto length() const -> float;
	auto normalize() -> void;
	auto normalized() const -> Quatf;

	auto mat4() const -> Matrix4f;

	auto operator=(const Quatf& rhs) -> Quatf&;
	auto operator*(const Quatf& rhs) const -> Quatf;
	auto operator~() const -> Quatf;
	auto operator/(float f) -> Quatf;

	static auto angleAxis(float angleDeg, float x, float y, float z) -> Quatf;
	static auto angleAxis(float angleDeg, Vector3f axis) -> Quatf;

	static auto lerp(const Quatf& from, const Quatf& to, float t) -> Quatf;
};

auto operator*(float f, const Quatf& q) -> Quatf;
auto operator*(const Quatf& q, float f) -> Quatf;
