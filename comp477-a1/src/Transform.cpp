#include "Transform.h"

#include <algorithm>


///////////////////////////////////////
//			CONSTRUCTORS
///////////////////////////////////////

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

///////////////////////////////////////
//			DESTRUCTORS
///////////////////////////////////////

Transform::~Transform() {}

///////////////////////////////////////
//			PUBLIC METHODS
///////////////////////////////////////

auto Transform::getParent() const -> Transform*
{
	return parent;
}

auto Transform::setParent(Transform* newParent) -> void
{
	auto worldPos = getWorldPosition();

	// If this Transform has a parent, remove it from the parent's list of children
	if (parent != nullptr)
		parent->removeChild(this);

	// Set the new parent
	parent = newParent;
	parent->addChild(this);
	setWorldPosition(worldPos);
}

auto Transform::getLocalPosition() const -> Vector3f
{
	return localPosition;
}

auto Transform::setLocalPosition(const Vector3f& pos) -> void
{
	localPosition = pos;
	propogateSetWorldPosGood(false);
}

auto Transform::getLocalRotation() const -> Quatf
{
	return localRotation;
}

auto Transform::setLocalRotation(const Quatf& rot) -> void
{
	localRotation = rot.normalized();

	propogateSetWorldPosGood(false);
	propogateSetWorldRotGood(false);
}

auto Transform::getWorldPosition() const -> Vector3f
{
	if (!worldPosGood)
		updateWorldPosition();

	return worldPosition;
}

auto Transform::setWorldPosition(const Vector3f& pos) -> void
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

auto Transform::getWorldRotation() const -> Quatf
{
	if (!worldRotGood)
		updateWorldRotation();

	return worldRotation;
}

auto Transform::setWorldRotation(const Quatf& rot) -> void
{
	worldRotation = rot;
	worldRotGood = true;
}

auto Transform::getChildren() const -> const std::vector<Transform*>&
{
	return children;
}

auto Transform::addChild(Transform* child) -> void
{
	using namespace std;

	// Ensure the child is not already in the list
	if (find(begin(children), end(children), child) == end(children))
		children.push_back(child);

}

auto Transform::removeChild(const Transform* child) -> void
{
	using namespace std;

	children.erase( remove(begin(children), end(children), child), end(children) );
}

auto Transform::getJoint() const -> Joint*
{
	return containingJoint;
}

auto Transform::setJoint(Joint* newJoint) -> void
{
	containingJoint = newJoint;
}

auto Transform::reset() -> void
{
	localPosition = Vector3f{ 0, 0, 0 };
	localRotation = Quatf{ 1, 0, 0, 0 };
	localScale = Vector3f{ 1, 1, 1 };

	updateWorldPosition();
	updateWorldRotation();
}

auto Transform::getMatrix() const -> Matrix4f
{
	auto parentMat = parent == nullptr
		? Matrix4f::createTranslation(0, 0, 0)
		: parent->getMatrix();

	auto parentPos = parent == nullptr
		? Vector3f{ 0, 0, 0 }
	: parent->getWorldPosition();

	auto toParentNeg = Matrix4f::createTranslation(-parentPos.x, -parentPos.y, -parentPos.z);
	auto toParent = Matrix4f::createTranslation(parentPos.x, parentPos.y, parentPos.z);
	auto rot = localRotation.mat4();

	return toParent * rot * toParentNeg * parentMat;
}

///////////////////////////////////////
//			PRIVATE METHODS
///////////////////////////////////////

auto Transform::updateWorldPosition() const -> void
{
	auto parentWorldPos = parent == nullptr
		? Vector3f{ 0, 0, 0 }
	: parent->getWorldPosition();

	auto pos = getWorldRotation().rotatePoint(localPosition);

	worldPosition = parentWorldPos + pos;

	worldPosGood = true;
}

auto Transform::updateWorldRotation() const -> void
{
	auto parentWorldRot = parent == nullptr
		? Quatf{ 1, 0, 0, 0 }
	: parent->getWorldRotation();

	worldRotation = localRotation * parentWorldRot;

	worldRotGood = true;
}

auto Transform::propogateSetWorldPosGood(bool good) -> void
{
	worldPosGood = good;
	for (auto& t : children)
	{
		t->worldPosGood = good;
		t->propogateSetWorldPosGood(good);
	}
}

auto Transform::propogateSetWorldRotGood(bool good) -> void
{
	worldRotGood = good;
	for (auto& t : children)
	{
		t->worldRotGood = good;
		t->propogateSetWorldRotGood(good);
	}
}