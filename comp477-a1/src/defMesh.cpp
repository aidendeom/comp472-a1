#include "defMesh.h"
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <assert.h>
#include <ctime>

DefMesh::DefMesh()
{
	mySkeleton.loadSkeleton("./resources/skeleton.out");
	loadWeights("./resources/weights.out");

	int numJoints = mySkeleton.getNumJoints();
	assert(numJoints == numBones + 1);

    pmodel = NULL;
    if (!pmodel) {	/* load up the model */

    char meshFile[] = "./resources/cheb.obj";
    pmodel = glmReadOBJ(meshFile);
    if (!pmodel) {
        return;
    }
        glmFacetNormals(pmodel);
        glmVertexNormals(pmodel, 0);
        glmFacetNormals(pmodel);
    }
}

void DefMesh::loadWeights(const std::string& path)
{
	int bonesPerLine{ -1 };

	std::ifstream file(path);
	if (file)
	{
		std::string line;
		while (std::getline(file, line))
		{
			int bonesThisLine{ 0 };
			std::stringstream ss(line);

			float weight;
			while (ss >> weight)
			{
				weights.push_back(weight);
				bonesThisLine++;
			}

			if (bonesPerLine == -1)
			{
				bonesPerLine = bonesThisLine;
			}
			else if (bonesPerLine != bonesThisLine)
			{
				std::cout << "There is an incorrect number of bones on this line!" << std::endl;
			}
		}

		numBones = bonesPerLine;
	}
	else
	{
		std::cout << "Unable to open file " << path << std::endl;
	}
}

void DefMesh::glDraw(int type)
{
    
    switch(type){
    case 0:
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); break;
    case 1:
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); break;
    case 2:
        mySkeleton.glDrawSkeleton(); return;
    
    }
    glColor3f(0.5, 0.5, 0.5);
    mode = GLM_NONE;
    mode = mode | GLM_SMOOTH;
    glPushMatrix();
		glScalef(2,2,2);
		glTranslatef(-0.5, -0.5, -0.5);
		glmDraw(pmodel, mode, &weights, mySkeleton.getJoints());
    glPopMatrix();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    mySkeleton.glDrawSkeleton();
}

DefMesh::~DefMesh()
{
	glmDelete(pmodel);
}