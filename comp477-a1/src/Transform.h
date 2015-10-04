#pragma once

#include <vector>

#include "vmath/vmath.h"
#include "GL/freeglut.h"

struct Joint;

class Transform
{
private:
	Vector3f localPosition;
	Quatf localRotation;
	Vector3f localScale;

	// Cached, so it does not have to calculate multiple times per frame
	Vector3f worldPosition;
	Quatf worldRotation;
	Vector3f worldScale;

	bool worldPosGood;
	bool worldRotGood;
	bool worldScaleGood;

	Transform* parent;
	std::vector<Transform*> children;

	Joint* containingJoint;

	void updateWorldPosition();
	void updateWorldRotation();

	void propogateSetWorldPosGood(bool good);
	void propogateSetWorldRotGood(bool good);

public:
	Transform();
	~Transform();

	Transform* getParent();
	void setParent(Transform* newParent);

	Vector3f getLocalPosition() const;
	void setLocalPosition(const Vector3f& pos);

	Quatf getLocalRotation() const;
	void setLocalRotation(const Quatf& rot);

	Vector3f getWorldPosition();
	void setWorldPosition(const Vector3f& pos);

	Quatf getWorldRotation();
	void setWorldRotation(const Quatf& rot);

	std::vector<Transform*>& getChildren();
	void addChild(Transform* child);
	void removeChild(const Transform* child);

	Joint* getJoint();
	void setJoint(Joint* newJoint);

	void transformPoint(Vector3f& p, float weight) const;

	void reset();
};

