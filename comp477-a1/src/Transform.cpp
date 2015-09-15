#include "Transform.h"

#include <algorithm>

Transform::Transform() :
worldPosition{ 0, 0, 0 },
localScale{ 0, 0, 0 },
localRotation{ 0, 0, 0 },
parent{ nullptr }
{
}


Transform::~Transform()
{
}

Transform* Transform::getParent()
{
	return parent;
}

void Transform::setParent(Transform* newParent)
{
	using namespace std;

	// If this Transform has a parent, remove it from the parent's list of children
	if (parent != nullptr)
	{
		auto& p = *parent;
		auto children = p.getChildren();

		auto match = find_if(begin(children), end(children), [this](Transform* c) -> bool {return this == c; });
		if (match != end(children))
			children.erase(match);
	}

	// Set the new parent
	parent = newParent;
	parent->getChildren().push_back(this);
}

Vec3 Transform::getWorldPosition()
{
	return worldPosition;
}

void Transform::setWorldPosition(Vec3 pos)
{
	worldPosition = pos;
}

std::vector<Transform*>& Transform::getChildren()
{
	return children;
}