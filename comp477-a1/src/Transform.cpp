#include "Transform.h"

#include <algorithm>

Transform::Transform()
: localPosition{ 0, 0, 0 }
, localRotation{ 1, 0, 0, 0 }
, localScale{ 1, 1, 1 }
, worldPosition{ 0, 0, 0 }
, worldRotation{ 1, 0, 0, 0 }
, worldScale{ 1, 1, 1 }
, worldPosGood{ true }
, worldRotGood{ true }
, worldScaleGood{ true }
, parent{ nullptr }
, containingJoint{ nullptr }
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
	propogateSetWorldPosGood(false);
	//worldPosGood = false;
}

Vector3f Transform::getWorldPosition()
{
	if (!worldPosGood)
		updateWorldPosition();

	return worldPosition;
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

	worldPosition = pos;
	worldPosGood = true;
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

	propogateSetWorldPosGood(false);
	propogateSetWorldRotGood(false);
	//worldRotGood = false;
}

Quatf Transform::getWorldRotation()
{
	if (!worldRotGood)
		updateWorldRotation();

	return worldRotation;
}

void Transform::setWorldRotation(const Quatf& rot)
{
	worldRotation = rot;
	worldRotGood = true;
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

void Transform::transformPoint(Vector3f& p, float weight) const
{
	p = localRotation.rotatePoint(p) * weight;
}

void Transform::reset()
{
	localPosition = Vector3f{ 0, 0, 0 };
	localRotation = Quatf{ 1, 0, 0, 0 };
	localScale = Vector3f{ 1, 1, 1 };
}

void Transform::updateWorldPosition()
{
	auto parentWorldPos = parent == nullptr
		? Vector3f{ 0, 0, 0 }
	: parent->getWorldPosition();

	auto pos = getWorldRotation().rotatePoint(localPosition);

	worldPosition = parentWorldPos + pos;

	worldPosGood = true;
}

void Transform::updateWorldRotation()
{
	auto parentWorldRot = parent == nullptr
		? Quatf{ 1, 0, 0, 0 }
	: parent->getWorldRotation();

	worldRotation = localRotation * parentWorldRot;

	worldRotGood = true;
}

void Transform::propogateSetWorldPosGood(bool good)
{
	worldPosGood = good;
	for (auto& t : children)
	{
		t->worldPosGood = good;
		t->propogateSetWorldPosGood(good);
	}
}

void Transform::propogateSetWorldRotGood(bool good)
{
	worldRotGood = good;
	for (auto& t : children)
	{
		t->worldRotGood = good;
		t->propogateSetWorldRotGood(good);
	}
}

auto Transform::getMatrix() -> Matrix4f
{
	auto parentMat = parent == nullptr
		? Matrix4f::createTranslation(0, 0, 0)
		: parent->getMatrix();

	auto parentPos = parent == nullptr
		? Vector3f{ 0, 0, 0 }
		: parent->getWorldPosition();

	auto toParentNeg	= Matrix4f::createTranslation(-parentPos.x, -parentPos.y, -parentPos.z);
	auto toParent		= Matrix4f::createTranslation(parentPos.x, parentPos.y, parentPos.z);
	auto rot			= localRotation.transform();

	return toParent * rot * toParentNeg * parentMat;
}