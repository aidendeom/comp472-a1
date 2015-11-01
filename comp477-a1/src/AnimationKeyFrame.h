#pragma once

#include <vector>
#include <ostream>
#include "vmath\vmath.h"
#include "Quatf.h"

class Skeleton;

class AnimationKeyFrame
{
public:
	std::vector<Quatf> orientations;

	AnimationKeyFrame() { };
	~AnimationKeyFrame() { };

	auto capture(const Skeleton& skeleton) -> void;

	friend auto operator<<(std::ostream& os, const AnimationKeyFrame& frame) -> std::ostream&;
};

