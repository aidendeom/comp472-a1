#pragma once

#include <vector>

#include "simpleMath.h"

class Transform
{
private:
	Vec3 worldPosition;

	Vec3 localScale;
	Vec3 localRotation;

	Transform* parent;
	std::vector<Transform*> children;

public:
	Transform();
	~Transform();

	Transform* getParent();
	void setParent(Transform* newParent);

	Vec3 getWorldPosition();
	void setWorldPosition(Vec3 pos);

	std::vector<Transform*>& getChildren();

	//Vec3 worldPos();
	//Transform& getChild(int index);
};

