#include "defMesh.h"

#include <string>
#include <iostream>

#include "WeightFileReader.h"
#include "Transform.h"

DefMesh::DefMesh()
: transMats{ 17 }
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

 		auto rot = bone->transform.getWorldRotation();
		//rot.invert();
		auto T = rot.transform();

		transMats[b - 1] = T;
	}
}

void DefMesh::transformVerts()
{
	//auto& bone = (*mySkeleton.getJoints())[b + 1];
	//auto currentWeight = weights[i * 17 + b];
	//auto vertIdx = (i + 1) * 3;

	updateTransMats();

	for (GLuint i = 0; i < pmodel->numvertices; i++)
	{
		// Vertices start at idx 1
		auto vertIdx = (i + 1) * 3;

		// Homogenous coordinate
		Vector4f p
		{
			defaultVerts[vertIdx + 0],
			defaultVerts[vertIdx + 1],
			defaultVerts[vertIdx + 2],
			1
		};

		auto pTrans = transformVert(p,i);

		auto vert = &pmodel->vertices[vertIdx];
		vert[0] = pTrans.x;
		vert[1] = pTrans.y;
		vert[2] = pTrans.z;
	}

}

Vector4f DefMesh::transformVert(const Vector4f& p, const int vertIdx) const
{
	auto& joints = *mySkeleton.getJoints();

	// Assumes there is at least one bone
	const int numBones = joints.size() - 1;

	Vector4f sum{ 0, 0, 0, 1 };

	for (auto j = 0; j < numBones; j++)
	{
		auto currentWeight = weights[vertIdx * 17 + j];
		auto& T = transMats[j];

		auto transP = T * p;
		transP *= currentWeight;

		sum += transP;
	}

	return sum;
}

void DefMesh::resetSkeletonDeltas()
{
	mySkeleton.resetDeltas();
}