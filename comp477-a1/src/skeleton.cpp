#include "skeleton.h"
#include "splitstring.h"
#include <cmath>
#include <iostream>

/*
 * Load skeleton file
 */
void Skeleton::loadSkeleton(std::string skelFileName)
{
    std::string strBone;
    std::ifstream skelFile(skelFileName.c_str());
    if (skelFile.is_open())
    {
        while ( std::getline(skelFile, strBone)) { //Read a line to build a bone
            std::vector<std::string> boneParams;
            splitstring splitStr(strBone);
            boneParams = splitStr.split(' ');
            Joint temp;
			temp.position =
			{
				std::atof(boneParams[1].c_str()),
				std::atof(boneParams[2].c_str()),
				std::atof(boneParams[3].c_str())
			};

			temp.parentIdx = std::atoi(boneParams[4].c_str());

			// This is not working yet... I'm pointing to the wrong thing.
			if (temp.parentIdx > -1)
				temp.parent = &joints[temp.parentIdx];

			if (std::atoi(boneParams[0].c_str()) != joints.size())
            {
                std::cout<<"[Warning!!!] Bone index not match\n";
            }
            joints.push_back(temp);
        }
    }

	printSkeletonHierarchy();
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
    
    for (unsigned i=0; i<joints.size(); i++)
    {
        if (joints[i].isPicked)
            glColor3f(1.0f, 0.0f, 0.0f);
        else if (joints[i].isHovered)
            glColor3f(0.7f, 0.7f, 0.7f);
        else
            glColor3f(0.3f, 0.3f, 0.3f);

		auto pos = joints[i].position;

		glPushMatrix();
			glTranslated(pos.x, pos.y, pos.z);
			glutSolidSphere(0.01, 15, 15);
		glPopMatrix();

		auto& j = joints[i];
		auto parentIdx = j.parentIdx;
		if (parentIdx > -1)
		{
			auto pos = j.position;
			auto parentPos = joints[parentIdx].position;

			glColor3f(0.3f, 0.3f, 0.3f);
			glLineWidth(2.5);
			glBegin(GL_LINES);
				glVertex3d(pos.x, pos.y, pos.z);
				glVertex3d(parentPos.x, parentPos.y, parentPos.z);
			glEnd();
		}
    }
    glPopMatrix();
    
    glEnable(GL_DEPTH_TEST);
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
		auto& pos = joints[i].position;
        gluProject((GLdouble)pos.x, (GLdouble)pos.y, (GLdouble)pos.z,
                modelview, projection, viewport,
                &winX, &winY, &winZ );
        joints[i].screenCoord.x = winX;
        joints[i].screenCoord.y = (double)glutGet(GLUT_WINDOW_HEIGHT)-winY;
    }
}
void Skeleton::checkHoveringStatus(int x, int y)
{
    double distance = 0.0f;
    double minDistance = 1000.0f;
    int hoveredJoint = -1;
    for(unsigned i=0; i < joints.size(); i++)
    {
        joints[i].isHovered = false;
        distance = sqrt((x - joints[i].screenCoord.x)*(x - joints[i].screenCoord.x) 
                + (y - joints[i].screenCoord.y)*(y - joints[i].screenCoord.y));
        if (distance > 50) continue;
        if (distance < minDistance)
        {
            hoveredJoint = i;
            minDistance = distance;
        }
    }
    if (hoveredJoint != -1) joints[hoveredJoint].isHovered = true;
}

void Skeleton::release()
{
    hasJointSelected = false;
    for (unsigned i=0; i<joints.size(); i++)
    {
        joints[i].isPicked = false;
    }
}

void Skeleton::selectOrReleaseJoint()
{
    bool hasHovered=false;
    for (unsigned i=0; i<joints.size(); i++)
    {
        joints[i].isPicked = false;
        if (joints[i].isHovered)
        {
            hasHovered = true;
            joints[i].isPicked = true;
            hasJointSelected = true;
        }
    }
    if (!hasHovered)    //Release joint
        hasJointSelected = false;
}


void Skeleton::printSkeletonHierarchy()
{
}