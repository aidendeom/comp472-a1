#pragma once

#include <vector>

#include "vmath/vmath.h"

struct Joint;

class Transform
{
private:
	Vector3f localPosition;
	Quatf localRotation;
	Vector3f localScale;

	Transform* parent;
	std::vector<Transform*> children;

	Joint* containingJoint;

	size_t numChildren;

public:
	Transform();
	~Transform();

	Transform* getParent();
	void setParent(Transform* newParent);

	Vector3f getLocalPosition() const;
	void setLocalPosition(const Vector3f& pos);

	Quatf getLocalRotation() const;
	void setLocalRotation(const Quatf& rot);

	Quatf getWorldRotation() const;
	void setWorldRotation(const Quatf& rot);

	Vector3f getWorldPosition() const;
	void setWorldPosition(const Vector3f& pos);

	std::vector<Transform*>& getChildren();
	void addChild(Transform* child);
	void removeChild(const Transform* child);

	Joint* getJoint();
	void setJoint(Joint* newJoint);

	void updateNumChildren()
	{
		numChildren = children.size();
	}

	bool checkNumChildren()
	{
		return numChildren == children.size();
	}

	void transformPoint(Vector3f& p);
};

