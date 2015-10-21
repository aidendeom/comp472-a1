#pragma once

#include <vector>
#include "vmath\vmath.h"
#include "Quatf.h"

class Skeleton;

class AnimationKeyFrame
{
public:
	std::vector<Quatf> orientations;

	AnimationKeyFrame();
	~AnimationKeyFrame();

	auto capture(const Skeleton& skeleton) -> void;
};

