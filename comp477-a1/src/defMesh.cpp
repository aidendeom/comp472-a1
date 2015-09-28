#include "defMesh.h"

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <assert.h>
#include <ctime>

#include "WeightFileReader.h"
#include "Transform.h"

DefMesh::DefMesh()
{
	mySkeleton.loadSkeleton("./resources/skeleton.out");
	loadWeights("./resources/weights.out");

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

DefMesh::~DefMesh()
{
	glmDelete(pmodel);
}

void DefMesh::loadWeights(const std::string& path)
{
	WeightFileReader wf{ path };
	weights = wf.getWeights();
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
		transformVerts();
		glmDraw(pmodel, mode);
    glPopMatrix();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    mySkeleton.glDrawSkeleton();
}

void DefMesh::transformVerts()
{
	auto verts = pmodel->vertices;

	for (GLuint i = 0; i < pmodel->numvertices; i++)
	{
		// The vert index is 1-based, not 0-based
		auto vertIdx = (i + 1) * 3;
		auto currentVert = &verts[vertIdx];
		Vector3f v{ currentVert[0], currentVert[1], currentVert[2] };

		// TODO: Un-hardcode this value
		for (size_t b = 0; b < 17; b++)
		{
			auto weightIdx = i * 17;
			auto currentWeight = weights[weightIdx + b];
			auto& bone = (*mySkeleton.getJoints())[b + 1];

			if (!bone->hasDelta)
				continue;
			
			auto& trans = bone->delta;

			Vector3f vTransformed{ v };
			trans.transformPoint(vTransformed);

			vTransformed -= v;
			vTransformed *= currentWeight;
			v += vTransformed;
		}

		currentVert[0] = v.x;
		currentVert[1] = v.y;
		currentVert[2] = v.z;
	}
}

void DefMesh::resetSkeletonDeltas()
{
	mySkeleton.resetDeltas();
}