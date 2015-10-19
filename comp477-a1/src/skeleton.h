#ifndef SKELETON_H
#define SKELETON_H

#include <vector>
#include <memory>

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

	int currentFrameIdx;

	Skeleton()
		: hasJointSelected{ false }
		, currentFrameIdx{ 0 }
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
};

#endif
