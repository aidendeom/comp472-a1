#include "defMesh.h"

#include "WeightFileReader.h"
#include "Transform.h"

DefMesh::DefMesh()
	: transMats( 17 )
{
	mySkeleton.loadSkeleton("./resources/skeleton.out");
	loadWeights("./resources/weights.out");

    pmodel = nullptr;
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
		//mySkeleton.drawskel();
        mySkeleton.glDrawSkeleton();
		return;
    
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
    
	//mySkeleton.drawskel();
    mySkeleton.glDrawSkeleton();
}

void DefMesh::updateTransMats()
{
	auto& joints = *mySkeleton.getJoints();

	for (size_t b = 1; b < 18; b++)
	{
		auto& bone = *joints[b].get();

		//if (!bone->hasDelta)
		//	continue;

		auto& parent = *bone.transform.getParent();
		auto pPos = parent.getWorldPosition();

		auto toParent = Matrix4f::createTranslation(pPos.x, pPos.y, pPos.z);
		auto toParentNeg = Matrix4f::createTranslation(-pPos.x, -pPos.y, -pPos.z);

		auto rot = bone.transform.getLocalRotation().transform();

		auto T = toParent * rot * toParentNeg;

		transMats[b - 1] = T;
	}
}

void DefMesh::transformVerts()
{
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

Vector4f DefMesh::transformVert(const Vector4f& p, const int vertIdx) //const
{
	auto& joints = *mySkeleton.getJoints();

	// Assumes there is at least one bone
	const int numBones = joints.size() - 1;

	Vector4f sum{ 0, 0, 0, 0};

	for (auto j = 0; j < numBones; j++)
	{
		// Disregard root joint - it's not a bone
		auto& bone = joints[j + 1];
		auto ppos = bone->transform.getParent()->getWorldPosition();

		auto currentWeight = weights[vertIdx * 17 + j];

		auto T = transMats[j];
		auto transP = T * p;
		//auto transP = bone->transform.getWorldRotation().rotatePoint(p - ppos) + ppos;
		transP *= currentWeight;

		sum += transP;
	}

	return sum;
}

void DefMesh::resetSkeletonDeltas()
{
	mySkeleton.resetDeltas();
}