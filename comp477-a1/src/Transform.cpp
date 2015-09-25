#include "Transform.h"

#include <algorithm>

Transform::Transform() :
localPosition{ 0, 0, 0 },
localRotation{ 1, 0, 0, 0 },
localScale{ 1, 1, 1 },
parent{ nullptr },
containingJoint{ nullptr },
numChildren{ 0 }
{}


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
	auto parentWorldRot = parent == nullptr ? Quatf{ 1, 0, 0, 0 } : parent->getWorldRotation();
	auto parentWorldPos = parent == nullptr ? Vector3f{ 0, 0, 0 } : parent->getWorldPosition();

	auto pos = localPosition;
	pos = parentWorldRot.rotatePoint(pos);

	return parentWorldPos + pos;
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

Quatf Transform::getLocalRotation() const
{
	return localRotation;
}

void Transform::setLocalRotation(const Quatf& rot)
{
	if (std::abs(rot.lengthSq()) < EPSILON)
		std::cout << "rotation has 0 length" << std::endl;

	localRotation = rot.normalized();
}

Quatf Transform::getWorldRotation() const
{
	auto parentWorldRot = parent == nullptr ? Quatf{ 1, 0, 0, 0 } : parent->getWorldRotation();

	return localRotation * parentWorldRot ;
}

void Transform::setWorldRotation(const Quatf& rot)
{

}

std::vector<Transform*>& Transform::getChildren()
{
	return children;
}

void Transform::addChild(Transform* child)
{
	using namespace std;

	// Ensure the child is not already in the list
	if (find(begin(children), end(children), child) == end(children))
		children.push_back(child);

}

void Transform::removeChild(const Transform* child)
{
	using namespace std;

	children.erase( remove(begin(children), end(children), child), end(children) );
}

Joint* Transform::getJoint()
{
	return containingJoint;
}

void Transform::setJoint(Joint* newJoint)
{
	containingJoint = newJoint;
}

void Transform::transformPoint(Vector3f& p)
{
	//p = getWorldRotation().rotatePoint(p);
	//p += localPosition;
}