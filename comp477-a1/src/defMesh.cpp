#include "defMesh.h"

#include <string>
#include <iostream>

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
        throw "Failed to load OBJ model";
    }
        glmFacetNormals(pmodel);
        glmVertexNormals(pmodel, 0);
        glmFacetNormals(pmodel);

		GLuint vertsLen = (pmodel->numvertices + 1) * 3;

		defaultVerts.resize(vertsLen);
		for (GLuint i = 0; i < vertsLen; i++)
		{
			defaultVerts[i] = pmodel->vertices[i];
		}
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

void DefMesh::updateTransMats()
{
	for (size_t b = 1; b < 18; b++)
	{
		auto& bone = (*mySkeleton.getJoints())[b];

		if (!bone->hasDelta)
			continue;

		auto T = bone->transform.getWorldRotation().inverted().transform();
		//for (size_t i = 0; i < 16; i++)
		//	transMats[b][i] = T[i];
	}
}

void DefMesh::transformVerts()
{
	auto verts = pmodel->vertices;
	updateTransMats();

	for (GLuint i = 0; i < pmodel->numvertices; i++)
	{
		// The vert index is 1-based, not 0-based
		auto vertIdx = (i + 1) * 3;

		Vector3f v
		{
			defaultVerts[vertIdx + 0],
			defaultVerts[vertIdx + 1],
			defaultVerts[vertIdx + 2]
		};

		

		// TODO: Un-hardcode this value
		for (size_t b = 0; b < 17; b++)
		{
			auto& bone = (*mySkeleton.getJoints())[b + 1];
			auto currentWeight = weights[i * 17 + b];

			//bone->transform.transformPoint(v, currentWeight);
		}

		auto currentVert = &pmodel->vertices[vertIdx];
		currentVert[0] = v.x;
		currentVert[1] = v.y;
		currentVert[2] = v.z;
	}
}

void DefMesh::resetSkeletonDeltas()
{
	mySkeleton.resetDeltas();
}