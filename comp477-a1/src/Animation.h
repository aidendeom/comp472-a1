#pragma once

#include <vector>
#include <string>
#include "AnimationKeyFrame.h"

class Animation
{
public:
	std::vector<AnimationKeyFrame> keyframes;

	Animation() { };
	~Animation() { };

	auto saveToFile(std::string filename) -> void;
};

