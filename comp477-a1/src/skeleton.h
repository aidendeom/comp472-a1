#ifndef SKELETON_H
#define SKELETON_H

#include <vector>
#include <memory>
#include <functional>

#include "Transform.h"
#include "Animation.h"

struct Joint
{
	Transform transform;
    Vector2i screenCoord;
    bool isHovered;
    bool isPicked;
	bool hasDelta;
	int index;
    
	Joint() :
		isHovered{ false },
		isPicked{ false },
		hasDelta{ false }
	{
		transform.setJoint(this);
	}

	void setDelta(bool delta)
	{
		hasDelta = delta;
		for (auto& t : transform.getChildren())
		{
			t->getJoint()->setDelta(delta);
		}
	}
};

class Skeleton
{
private:
    std::vector<std::unique_ptr<Joint>> joints;

    /*Update screen coordinates of joints*/
	void updateScreenCoord();

public:
    /*True if the skeleton has a joint selected*/
    bool hasJointSelected;   

	Animation animation;
	size_t currentFrameIdx;
	bool isAnimating;

	AnimationKeyFrame* from;
	AnimationKeyFrame* to;

	float time;
	float duration;

	std::function<Quatf(const Quatf&, const Quatf&, float t)>  interpFunction;

	Skeleton()
		: hasJointSelected{ false }
		, currentFrameIdx{ 0 }
		, isAnimating{ false }
		, interpFunction{Quatf::slerp}
	{
		animation.keyframes.push_back(AnimationKeyFrame());
	};
    /*
     * Load Skeleton file
     */
    void loadSkeleton(std::string skelFileName);

    /*
     * Load animation file
     */
    void loadAnimation(std::string skelFileName);

    /*
     * Draw skeleton with OpenGL
     */
    void glDrawSkeleton();

	void glDrawTransformHierarchy(Joint& root) const;

    /*
     * Check if any joint is hovered by given mouse coordinate
     */
    void checkHoveringStatus(int x, int y);

    void release();
    
    void selectOrReleaseJoint();

	Joint* getSelectedJoint();

	const std::vector<std::unique_ptr<Joint>>* getJoints() const;

	void resetDeltas() const;

	auto setPose(const AnimationKeyFrame& frame) -> void;

	auto updateAnimation(float delta) -> void;

	auto resetAnimParams() -> void;
};

#endif
