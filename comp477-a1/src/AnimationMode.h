#include <string>

enum class AnimationMode
{
	Edit,
	Playback,
	Count = 2,
};

auto getAnimationModeString(AnimationMode mode) -> std::string
{
	switch (mode)
	{
	case AnimationMode::Edit:
		return "Edit";
	case AnimationMode::Playback:
		return "Playback";
	default:
		throw "Not an animation mode";
	}
}