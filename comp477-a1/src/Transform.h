#pragma once

#include <vector>

#include "vmath/vmath.h"

struct Joint;

class Transform
{
public:
	Transform();
	~Transform();

	auto getParent() const->Transform*;
	auto setParent(Transform* newParent) -> void;

	auto getLocalPosition() const->Vector3f;
	auto setLocalPosition(const Vector3f& pos) -> void;

	auto getLocalRotation() const->Quatf;
	auto setLocalRotation(const Quatf& rot) -> void;

	auto getWorldPosition() const->Vector3f;
	auto setWorldPosition(const Vector3f& pos) -> void;

	auto getWorldRotation() const->Quatf;
	auto setWorldRotation(const Quatf& rot) -> void;

	auto getChildren() const -> const std::vector<Transform*>&;
	auto addChild(Transform* child) -> void;
	auto removeChild(const Transform* child) -> void;

	auto getJoint() const->Joint*;
	auto setJoint(Joint* newJoint) -> void;

	auto reset() -> void;

	auto getMatrix() const->Matrix4f;

private:
	Vector3f localPosition;
	Quatf localRotation;
	Vector3f localScale;

	// Cached, so it does not have to calculate multiple times per frame
	// We declare these as mutable, so functions like getWorldPos can update 
	// them when necessary, but still be declared const
	mutable Vector3f worldPosition;
	mutable Quatf worldRotation;
	mutable Vector3f worldScale;

	mutable bool worldPosGood;
	mutable bool worldRotGood;
	mutable bool worldScaleGood;

	Transform* parent;
	std::vector<Transform*> children;

	Joint* containingJoint;

	auto updateWorldPosition() const -> void;
	auto updateWorldRotation() const -> void;

	void propogateSetWorldPosGood(bool good);
	void propogateSetWorldRotGood(bool good);
};

