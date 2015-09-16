#include "Transform.h"

#include <algorithm>

Transform::Transform() :
localPosition{ 0, 0, 0 },
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

	auto worldPos = getWorldPosition();

	// If this Transform has a parent, remove it from the parent's list of children
	if (parent != nullptr)
		parent->removeChild(this);

	// Set the new parent
	parent = newParent;
	parent->addChild(this);
	setWorldPosition(worldPos);
}

Vector3f Transform::getLocalPosition() const
{
	return localPosition;
}

void Transform::setLocalPosition(const Vector3f& pos)
{
	localPosition = pos;
}

Vector3f Transform::getWorldPosition() const
{
	auto parentWorldPos = parent == nullptr ? Vector3f{ 0, 0, 0 } : parent->getWorldPosition();

	return parentWorldPos + localPosition;
}

void Transform::setWorldPosition(const Vector3f& pos)
{
	if (parent == nullptr)
	{
		localPosition = pos;
	}
	else
	{
		auto parentWorldPos = parent->getWorldPosition();
		localPosition = pos - parentWorldPos;
	}
}

std::vector<Transform*>& Transform::getChildren()
{
	return children;
}

void Transform::addChild(Transform* child)
{
	using namespace std;

	if (find(begin(children), end(children), child) != end(children))
		children.push_back(child);

}

void Transform::removeChild(const Transform* child)
{
	using namespace std;

	children.erase( remove(begin(children), end(children), child), end(children) );
}