#pragma once

#include <vector>
#include "AnimationKeyFrame.h"
#include "skeleton.h"

class Animation
{
public:
	Animation();
	~Animation();

	auto addKeyFrame(const Skeleton& skeleton) -> void;

private:
	std::vector<AnimationKeyFrame> keyframes;
};

