/*
 * Mesh with skeleton attached
 * You could put attachment here, or create an attachment class to manage weights
 */
#ifndef MESH_H
#define MESH_H
#include "./glm.h"
#include "./skeleton.h"
class DefMesh
{
public:
    Skeleton mySkeleton;
    GLMmodel * pmodel;
    GLuint mode;
    DefMesh(); 
    void glDraw(int type);
};
#endif
