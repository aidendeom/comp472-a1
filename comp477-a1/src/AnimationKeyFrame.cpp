#include "AnimationKeyFrame.h"

#include "skeleton.h"

AnimationKeyFrame::AnimationKeyFrame()
{
}


AnimationKeyFrame::~AnimationKeyFrame()
{
}

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