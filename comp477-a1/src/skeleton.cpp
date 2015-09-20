#include "skeleton.h"
#include "splitstring.h"
#include <cmath>
#include <iostream>

// uncomment to disable assert()
// #define NDEBUG
#include <cassert>

/*
 * Load skeleton file
 */
void Skeleton::loadSkeleton(std::string skelFileName)
{
	using std::make_unique;

    std::string strBone;
    std::ifstream skelFile(skelFileName.c_str());
    if (skelFile.is_open())
    {
        while ( std::getline(skelFile, strBone)) { //Read a line to build a bone
            std::vector<std::string> boneParams;
            splitstring splitStr(strBone);
            boneParams = splitStr.split(' ');

			joints.push_back(make_unique<Joint>());
			auto& temp = joints.back();
			auto& trans = temp->transform;

			Vector3f worldPos
			{
				std::stof(boneParams[1]),
				std::stof(boneParams[2]),
				std::stof(boneParams[3])
			};

			trans.setWorldPosition(worldPos);

			auto parentIdx = std::stoi(boneParams[4]);

			if (parentIdx > -1)
				trans.setParent(&joints[parentIdx]->transform);

			if (std::stoi(boneParams[0]) + 1 != joints.size())
            {
                std::cout<<"[Warning!!!] Bone index not match\n";
            }
        }

		numJoints = joints.size();

		for (auto& j : joints)
		{
			j->transform.updateNumChildren();
		}
    }
}

/*
 * Load Animation
 */
void Skeleton::loadAnimation(std::string skelFileName)
{
}


/*
 * Draw skeleton with OpenGL
 */
void Skeleton::glDrawSkeleton()
{
    //Rigging skeleton
    glDisable(GL_DEPTH_TEST);

    glPushMatrix();
	glTranslatef(-0.9f, -0.9f, -0.9f);
	glScalef(1.8f, 1.8f, 1.8f);
	glPointSize(6);
	glLineWidth(2.5);
	glColor3f(1, 0, 0);
	updateScreenCoord();
    
	if (joints.size() != numJoints)
		cout << "num joints changed" << endl;

	glDrawTransformHierarchy(*joints[0].get());

    glPopMatrix();
    
    glEnable(GL_DEPTH_TEST);
}

void Skeleton::glDrawTransformHierarchy(Joint& root)
{
	if (!root.transform.checkNumChildren())
		cout << "num children changed!" << endl;

	glPushMatrix();

	auto& pos = root.transform.getLocalPosition();

	glColor3f(0.3f, 0.3f, 0.3f);

	// Draw bone from child to parent.
	// The coordinate system has not yet been
	// changed, so [0,0,0] is still the parent's position
	if (root.transform.getParent() != nullptr)
	{
		glLineWidth(2.5);
		glBegin(GL_LINES);
		glVertex3i(0, 0, 0);
		glVertex3f(pos.x, pos.y, pos.z);
		glEnd();
	}

	if (root.isPicked)
	    glColor3f(1.0f, 0.0f, 0.0f);
	else if (root.isHovered)
	    glColor3f(0.7f, 0.7f, 0.7f);

	// Apply local translation
	glTranslatef(pos.x, pos.y, pos.z);

	// Apply local rotation
	auto rot = root.transform.getLocalRotation().transform();
	glMultMatrixf(rot);

	glutSolidSphere(0.01, 15, 15);

	for (auto c : root.transform.getChildren())
	{
		glDrawTransformHierarchy(*c->getJoint());
	}

	glPopMatrix();
}



void Skeleton::updateScreenCoord()
{
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLdouble winX, winY, winZ;

    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );
    for (unsigned i=0; i<joints.size(); i++)
    {
		auto& pos = joints[i]->transform.getWorldPosition();
        gluProject((GLdouble)pos.x, (GLdouble)pos.y, (GLdouble)pos.z,
                modelview, projection, viewport,
                &winX, &winY, &winZ );
        joints[i]->screenCoord.x = winX;
        joints[i]->screenCoord.y = (double)glutGet(GLUT_WINDOW_HEIGHT)-winY;
    }
}
void Skeleton::checkHoveringStatus(int x, int y)
{
    double distance = 0.0f;
    double minDistance = 1000.0f;
    int hoveredJoint = -1;
    for(unsigned i=0; i < joints.size(); i++)
    {
        joints[i]->isHovered = false;
        distance = sqrt((x - joints[i]->screenCoord.x)*(x - joints[i]->screenCoord.x) 
                + (y - joints[i]->screenCoord.y)*(y - joints[i]->screenCoord.y));
        if (distance > 50) continue;
        if (distance < minDistance)
        {
            hoveredJoint = i;
            minDistance = distance;
        }
    }
    if (hoveredJoint != -1) joints[hoveredJoint]->isHovered = true;
}

void Skeleton::release()
{
    hasJointSelected = false;
    for (unsigned i=0; i<joints.size(); i++)
    {
        joints[i]->isPicked = false;
    }
}

void Skeleton::selectOrReleaseJoint()
{
    bool hasHovered=false;
    for (unsigned i=0; i<joints.size(); i++)
    {
        joints[i]->isPicked = false;
        if (joints[i]->isHovered)
        {
            hasHovered = true;
            joints[i]->isPicked = true;
            hasJointSelected = true;
        }
    }
    if (!hasHovered)    //Release joint
        hasJointSelected = false;
}

Joint* Skeleton::getSelectedJoint()
{
	auto selected = find_if(begin(joints), end(joints), [](unique_ptr<Joint>& j) { return j->isPicked; });

	if (selected != end(joints))
		return selected->get();

	return nullptr;
}