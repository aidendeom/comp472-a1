/*
 * Mesh with skeleton attached
 * You could put attachment here, or create an attachment class to manage weights
 */
#ifndef MESH_H
#define MESH_H

#include "./glm.h"
#include "./skeleton.h"
#include <vector>
#include <string>

class DefMesh
{
private:
	int numBones;
	std::vector<float> weights;

	void loadWeights(const std::string& path);

public:
    Skeleton mySkeleton;
    GLMmodel * pmodel;
    GLuint mode;
    DefMesh(); 
	~DefMesh();

    void glDraw(int type);
};
#endif
