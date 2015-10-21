#pragma once

#include <vector>
#include "AnimationKeyFrame.h"

class Animation
{
public:
	std::vector<AnimationKeyFrame> keyframes;

	Animation();
	~Animation();
};

