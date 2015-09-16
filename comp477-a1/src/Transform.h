#pragma once

#include <vector>

#include "simpleMath.h"

class Transform
{
private:
	Vec3 localPosition;
	Vec3 localScale;
	Vec3 localRotation;

	Transform* parent;
	std::vector<Transform*> children;

public:
	Transform();
	~Transform();

	Transform* getParent();
	void setParent(Transform* newParent);

	Vec3 getLocalPosition() const;
	void setLocalPosition(const Vec3& pos);

	Vec3 getWorldPosition() const;
	void setWorldPosition(const Vec3& pos);

	std::vector<Transform*>& getChildren();
	void addChild(Transform* child);
	void removeChild(const Transform* child);
};

