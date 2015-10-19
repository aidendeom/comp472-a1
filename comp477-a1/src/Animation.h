#pragma once

#include <vector>
#include "AnimationKeyFrame.h"

class Skeleton;

class Animation
{
public:
	std::vector<AnimationKeyFrame> keyframes;

	Animation();
	~Animation();
};

