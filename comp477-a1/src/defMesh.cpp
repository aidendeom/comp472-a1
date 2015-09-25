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

bool first = true;

void DefMesh::transformVerts()
{
	auto v = pmodel->vertices;

	for (GLuint i = 0; i < pmodel->numvertices / 3; i++)
	{
		bool print = first;
		for (size_t b = 0; b < 17; b++)
		{
			float weight = getWeightForPointAndBone(i, b);
			//if (print)
			//	std::cout << weight << std::endl;
			Transform* trans = getTransformForBone(b);
			trans->transformPoint(&v[i]);
			//multWeight(&v[i], weight);
		}
		first = false;
	}
}

float DefMesh::getWeightForPointAndBone(int point, int bone)
{
	// points go from 0 -> numVertices - 1
	// bones go from 0 -> 17 i.e. joint 1 -> 18
	int idx = point * 17 + bone;

	return weights[idx];
}

Transform* DefMesh::getTransformForBone(int bone)
{
	bone++;

	auto& joints = *mySkeleton.getJoints();
	return &joints[bone]->transform;
}

Vector3f DefMesh::makeVector(int point)
{
	int p1 = point + 0;
	int p2 = point + 1;
	int p3 = point + 2;

	auto& v = pmodel->vertices;

	return Vector3f{ v[p1], v[p2], v[p3] };
}

void DefMesh::updatePoint(const Vector3f& p, int idx)
{
	auto& v = pmodel->vertices;
	v[idx + 0] = p.x;
	v[idx + 1] = p.y;
	v[idx + 2] = p.z;
}

void DefMesh::multWeight(GLfloat* v, float weight)
{
	v[0] *= weight;
	v[1] *= weight;
	v[2] *= weight;
}

DefMesh::~DefMesh()
{
	glmDelete(pmodel);
}