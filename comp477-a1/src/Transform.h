#pragma once

#include <vector>

#include "vmath/vmath.h"

struct Joint;

class Transform
{
private:
	Vector3f localPosition;
	Vector3f localScale;
	Vector3f localRotation;

	Transform* parent;
	std::vector<Transform*> children;

	Joint* containingJoint;

public:
	Transform();
	~Transform();

	Transform* getParent();
	void setParent(Transform* newParent);

	Vector3f getLocalPosition() const;
	void setLocalPosition(const Vector3f& pos);

	Vector3f getWorldPosition() const;
	void setWorldPosition(const Vector3f& pos);

	std::vector<Transform*>& getChildren();
	void addChild(Transform* child);
	void removeChild(const Transform* child);

	Joint* getJoint();
	void setJoint(Joint* newJoint);
};

