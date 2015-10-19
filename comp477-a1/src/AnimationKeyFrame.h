#pragma once

#include <vector>
#include "vmath\vmath.h"
#include "skeleton.h"

class AnimationKeyFrame
{
public:
	AnimationKeyFrame();
	~AnimationKeyFrame();

	auto capture(const std::vector<Joint>& joints) -> void;

private:
	std::vector<Quatf> orientations;
};

