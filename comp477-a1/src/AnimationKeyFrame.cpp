#include "AnimationKeyFrame.h"

#include "skeleton.h"

auto AnimationKeyFrame::capture(const Skeleton& skeleton) -> void
{
	auto& joints = *skeleton.getJoints();
	if (joints.size() != orientations.size())
		orientations.resize(joints.size());

	for (size_t i = 0; i < joints.size(); i++)
	{
		auto& rot = orientations[i];
		rot = joints[i]->transform.getLocalRotation();
	}
}

auto operator<<(std::ostream& os, const AnimationKeyFrame& frame) -> std::ostream&
{
	auto length = frame.orientations.size();
	for (size_t i = 1; i < length; i++)
	{
		auto o = frame.orientations[i];
		os << o;
		if (i < length - 1)
			os << ' ';
	}

	return os;
}