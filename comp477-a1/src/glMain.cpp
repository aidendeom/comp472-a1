#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#ifdef _WIN32
#include "GL/glut.h"
#else
#include <GL/freeglut.h>
#endif
#endif

#include <iostream>
#include <cmath>
#include <cstring>
#include "skeleton.h"
#include "defMesh.h"
using namespace std;


//Create Mesh
DefMesh myDefMesh;

//Switches
int meshModel=0;
bool drawSkeleton=true;

//Window parameters
int width = 1024;
int height = 768;
///* Ortho (if used) */
double _left = 0.0;		/* ortho view volume params */
double _right = 0.0;
double _bottom = 0.0;
double _top = 0.0;
double _zNear = 0.1;
double _zFar = 50.0;
double fovy = 45.0;
double prev_z = 0;

//Model matrices
double _matrix[16];
double _matrixI[16];

/* Mouse Interface  */
int _mouseX = 0;		/* mouse control variables */
int _mouseY = 0;
bool _mouseLeft = false;
bool _mouseMiddle = false;
bool _mouseRight = false;

double _dragPosX = 0.0;
double _dragPosY = 0.0;
double _dragPosZ = 0.0;

double vlen(double x, double y, double z)
{
    return sqrt(x * x + y * y + z * z);
}

float getAngle(Vector2f v1, Vector2f v2)
{
	auto l1 = v1.lengthSq();
	auto l2 = v2.lengthSq();

	// Make sure we don't divide by zero
	if (std::abs(l1) < EPSILON || std::abs(l2) < EPSILON)
		return 0;

	v1.normalize();
	v2.normalize();

	auto angle = std::acos(Vector2f::dot(v1, v2));
	auto orientation = (v1.x * v2.y) - (v2.x * v1.y);

	if (orientation > 0)
		angle = -angle;

	// Radians to degrees
	return static_cast<float>(angle * (180.0 / M_PI));
}

Vector3f getEyePosition()
{
	auto& m = _matrix;

	return -Vector3f
	{
		static_cast<float>(m[12]),
		static_cast<float>(m[13]),
		static_cast<float>(m[14])
	};
}

Vector3f getEyeDirection()
{
	Matrix4d m{ _matrixI };

	Vector4d forward{ 0, 0, -1, 0 };
	Vector4f f{ m * forward };

	return Vector3f{ f.x, f.y, f.z };
}

void invertMatrix(const GLdouble * m, GLdouble * out)
{

/* NB. OpenGL Matrices are COLUMN major. */
#define MAT(m,r,c) (m)[(c)*4+(r)]

/* Here's some shorthand converting standard (row,column) to index. */
#define m11 MAT(m,0,0)
#define m12 MAT(m,0,1)
#define m13 MAT(m,0,2)
#define m14 MAT(m,0,3)
#define m21 MAT(m,1,0)
#define m22 MAT(m,1,1)
#define m23 MAT(m,1,2)
#define m24 MAT(m,1,3)
#define m31 MAT(m,2,0)
#define m32 MAT(m,2,1)
#define m33 MAT(m,2,2)
#define m34 MAT(m,2,3)
#define m41 MAT(m,3,0)
#define m42 MAT(m,3,1)
#define m43 MAT(m,3,2)
#define m44 MAT(m,3,3)

    GLdouble det;
    GLdouble d12, d13, d23, d24, d34, d41;
    GLdouble tmp[16];		/* Allow out == in. */

    /* Inverse = adjoint / det. (See linear algebra texts.) */

    /* pre-compute 2x2 dets for last two rows when computing */
    /* cofactors of first two rows. */
    d12 = (m31 * m42 - m41 * m32);
    d13 = (m31 * m43 - m41 * m33);
    d23 = (m32 * m43 - m42 * m33);
    d24 = (m32 * m44 - m42 * m34);
    d34 = (m33 * m44 - m43 * m34);
    d41 = (m34 * m41 - m44 * m31);

    tmp[0] = (m22 * d34 - m23 * d24 + m24 * d23);
    tmp[1] = -(m21 * d34 + m23 * d41 + m24 * d13);
    tmp[2] = (m21 * d24 + m22 * d41 + m24 * d12);
    tmp[3] = -(m21 * d23 - m22 * d13 + m23 * d12);

    /* Compute determinant as early as possible using these cofactors. */
    det = m11 * tmp[0] + m12 * tmp[1] + m13 * tmp[2] + m14 * tmp[3];

    /* Run singularity test. */
    if (det == 0.0) {
	/* printf("invert_matrix: Warning: Singular matrix.\n"); */
/* 	  memcpy(out,_identity,16*sizeof(double)); */
    } else {
	GLdouble invDet = 1.0 / det;
	/* Compute rest of inverse. */
	tmp[0] *= invDet;
	tmp[1] *= invDet;
	tmp[2] *= invDet;
	tmp[3] *= invDet;

	tmp[4] = -(m12 * d34 - m13 * d24 + m14 * d23) * invDet;
	tmp[5] = (m11 * d34 + m13 * d41 + m14 * d13) * invDet;
	tmp[6] = -(m11 * d24 + m12 * d41 + m14 * d12) * invDet;
	tmp[7] = (m11 * d23 - m12 * d13 + m13 * d12) * invDet;

	/* Pre-compute 2x2 dets for first two rows when computing */
	/* cofactors of last two rows. */
	d12 = m11 * m22 - m21 * m12;
	d13 = m11 * m23 - m21 * m13;
	d23 = m12 * m23 - m22 * m13;
	d24 = m12 * m24 - m22 * m14;
	d34 = m13 * m24 - m23 * m14;
	d41 = m14 * m21 - m24 * m11;

	tmp[8] = (m42 * d34 - m43 * d24 + m44 * d23) * invDet;
	tmp[9] = -(m41 * d34 + m43 * d41 + m44 * d13) * invDet;
	tmp[10] = (m41 * d24 + m42 * d41 + m44 * d12) * invDet;
	tmp[11] = -(m41 * d23 - m42 * d13 + m43 * d12) * invDet;
	tmp[12] = -(m32 * d34 - m33 * d24 + m34 * d23) * invDet;
	tmp[13] = (m31 * d34 + m33 * d41 + m34 * d13) * invDet;
	tmp[14] = -(m31 * d24 + m32 * d41 + m34 * d12) * invDet;
	tmp[15] = (m31 * d23 - m32 * d13 + m33 * d12) * invDet;

	memcpy(out, tmp, 16 * sizeof(GLdouble));
    }

#undef m11
#undef m12
#undef m13
#undef m14
#undef m21
#undef m22
#undef m23
#undef m24
#undef m31
#undef m32
#undef m33
#undef m34
#undef m41
#undef m42
#undef m43
#undef m44
#undef MAT
}



void screenToWorldPos(double *px, double *py, double *pz, const int x, const int y,
	 const int *viewport)
{
    /*
       Use the ortho projection and viewport information
       to map from mouse co-ordinates back into world 
       co-ordinates
     */

    *px = (double) (x - viewport[0]) / (double) (viewport[2]);
    *py = (double) (y - viewport[1]) / (double) (viewport[3]);

    *px = _left + (*px) * (_right - _left);
    *py = _top + (*py) * (_bottom - _top);
    *pz = _zNear;
}

void getMatrix()
{
    glGetDoublev(GL_MODELVIEW_MATRIX, _matrix);
    invertMatrix(_matrix, _matrixI);
}

void init()
{
    //OpenGL initialize functions goes here

    /*glutInitContextVersion(4, 2);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitContextFlags(GLUT_DEBUG);

    std::cout<<"Vendor: "<<glGetString(GL_VENDOR)<<std::endl;
    std::cout<<"Version: "<<glGetString(GL_VERSION)<<std::endl;
    std::cout<<"GLSL: "<<glGetString(GL_SHADING_LANGUAGE_VERSION)<<std::endl;*/

      //Light values and coordinates
     GLfloat ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0f };
     GLfloat diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
     GLfloat lightPos[] = {20.0f, 20.0f, 50.0f};
     glEnable(GL_DEPTH_TEST);
     glFrontFace(GL_CCW);
     //glEnable(GL_CULL_FACE);
     glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
     // Hidden surface removal // Counterclockwise polygons face out // Do not calculate inside of jet // Enable lighting
     glEnable(GL_LIGHTING);
     // Set up and enable light 0
     glLightfv(GL_LIGHT0,GL_AMBIENT,ambientLight);
     glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseLight);
     glEnable(GL_LIGHT0);
     // Enable color tracking
     glEnable(GL_COLOR_MATERIAL);
     // Set material properties to follow glColor values
     glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

     glClearColor(0.2f, 0.2f, 0.2f, 3.0f );
    
     //Rescale normals to unit length
     glEnable(GL_NORMALIZE);
     glLightfv(GL_LIGHT0,GL_POSITION,lightPos);

     glShadeModel(GL_FLAT);
     getMatrix(); //Init matrix

     //Translate camera
     glPushMatrix();
     glLoadIdentity();
     glTranslatef(0,0,-5.0);
     glMultMatrixd(_matrix);
     getMatrix();
     glPopMatrix();

}

void changeSize(int w, int h)
{
    //GLfloat aspectRatio;
    //if(h==0)
    //    h = 1;
    //glViewport(0, 0, w, h);
    //glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
    //aspectRatio = (GLfloat)w / (GLfloat)h;
    //gluPerspective(45.0f, aspectRatio, 1.0f, 900.0f);    //using perspective
    //
    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();

    glViewport(0, 0, w, h);


    _top = 1.0;
    _bottom = -1.0;
    _left = -(double) w / (double) h;
    _right = -_left;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    /* glOrtho(_left,_right,_bottom,_top,_zNear,_zFar);  Ortho */
    gluPerspective(fovy, (double) w / (double) h, _zNear, _zFar);	/* PErspective for stereo */

    glMatrixMode(GL_MODELVIEW);
}

void timerFunction(int value)       
{
    glutTimerFunc(10,timerFunction,1);
    glutPostRedisplay();
}
void handleKeyPress(unsigned char key, int x, int y)
{ 
    switch(key)
    {
        case 'm':
            meshModel = (meshModel+1)%3; break;
		case 27: // ESC key
        case 'q':
            exit(0);
    }
}


void mouseEvent(int button, int state, int x, int y)
{
    int viewport[4];

    _mouseX = x;
    _mouseY = y;

    if (state == GLUT_UP)
	switch (button) {
    case GLUT_LEFT_BUTTON:
        myDefMesh.mySkeleton.release();
            _mouseLeft =false;
            break;
	case GLUT_MIDDLE_BUTTON:
	    _mouseMiddle = false;
	    break;
	case GLUT_RIGHT_BUTTON:
	    _mouseRight = false;
	    break;
    } else
	switch (button) {
	case GLUT_LEFT_BUTTON:
        myDefMesh.mySkeleton.selectOrReleaseJoint();
        _mouseLeft = true;
        break;
	case GLUT_MIDDLE_BUTTON:
	    _mouseMiddle = true;
	    break;
	case GLUT_RIGHT_BUTTON:
	    _mouseRight = true;
	    break;
    case 4:         //Zoomout
        glLoadIdentity();
		glTranslatef(0.0f, 0.0f, -0.1f);
        glMultMatrixd(_matrix);
        getMatrix();
        glutPostRedisplay();
        break;
    case 3:         //Zoomin
        glLoadIdentity();
        glTranslatef(0.0f,0.0f,0.1f);
        glMultMatrixd(_matrix);
        getMatrix();
        glutPostRedisplay();
        break;
    default:
        break;
        //std::cout<<button<<std::endl;
	}

    glGetIntegerv(GL_VIEWPORT, viewport);
    screenToWorldPos(&_dragPosX, &_dragPosY, &_dragPosZ, x, y, viewport);
}

void mousePassiveFunc(int x, int y)
{
    myDefMesh.mySkeleton.checkHoveringStatus(x, y);
}
void mouseMoveEvent(int x, int y)
{
	const int dx = x - _mouseX;
	const int dy = y - _mouseY;

	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

    if (!myDefMesh.mySkeleton.hasJointSelected)
    {
        bool changed = false;

        if (dx == 0 && dy == 0)
            return;

        if (_mouseMiddle || (_mouseLeft && _mouseRight)) {
			/* double s = exp((double)dy*0.01); */
			/* glScalef(s,s,s); */
			/* if(abs(prev_z) <= 1.0) */

			glLoadIdentity();
			glTranslatef(0.0f, 0.0f, dy * 0.01f);
			glMultMatrixd(_matrix);

			changed = true;
        } else if (_mouseLeft) {
			double ax, ay, az;
			double bx, by, bz;
			double angle;

			ax = dy;
			ay = dx;
			az = 0.0;
			angle = vlen(ax, ay, az) / (double) (viewport[2] + 1) * 180.0;

			/* Use inverse matrix to determine local axis of rotation */

			bx = _matrixI[0] * ax + _matrixI[4] * ay + _matrixI[8] * az;
			by = _matrixI[1] * ax + _matrixI[5] * ay + _matrixI[9] * az;
			bz = _matrixI[2] * ax + _matrixI[6] * ay + _matrixI[10] * az;

			glRotated(angle, bx, by, bz);

			changed = true;
        } else if (_mouseRight) {
			double px, py, pz;

			screenToWorldPos(&px, &py, &pz, x, y, viewport);

			glLoadIdentity();
			glTranslated(px - _dragPosX, py - _dragPosY, pz - _dragPosZ);
			glMultMatrixd(_matrix);

			_dragPosX = px;
			_dragPosY = py;
			_dragPosZ = pz;

			changed = true;
        }

        _mouseX = x;
        _mouseY = y;

        if (changed) {
            getMatrix();
            glutPostRedisplay();
        }
    }
    /*
     * Do joint jobs
     */
    else    
    {
		Joint* selectedJoint = myDefMesh.mySkeleton.getSelectedJoint();

		// Leave if there is no selected joint or if it is the root
		// Might be fun to translate the model if the root is selected
		if (selectedJoint == nullptr || selectedJoint->transform.getParent() == nullptr)
			return;

		auto& j = *selectedJoint;
		auto& p = *j.transform.getParent()->getJoint();

		Vector2i mousePos{ x, y };
		Vector2f v1 = mousePos - p.screenCoord;
		Vector2f v2 = j.screenCoord - p.screenCoord;

		float angle = getAngle(v1, v2);

		Quatf rot = Quatf::fromAxisRot(getEyeDirection(), angle);

		auto newPos = rot.rotatePoint(j.transform.getLocalPosition());
		auto newRot = rot * j.transform.getLocalRotation();

		// Set the delta for this frame
		j.delta.setLocalPosition(newPos - j.transform.getLocalPosition());
		j.delta.setLocalRotation(rot);
		j.setDelta(true);

		j.transform.setLocalPosition(newPos);
		j.transform.setLocalRotation(newRot);
    }
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
	glMultMatrixd(_matrix);

	//draw terrain
    //glColor3f(0.5,0.5,0.5);
	glPushMatrix();
	glColor3f(0.7f, 0.7f, 0.7f);
    glBegin(GL_QUADS);
		glVertex3f(-3.0f, -0.85f, 3.0f);
		glVertex3f(3.0f, -0.85f, 3.0f);
		glVertex3f(3.0f, -0.85f, -3.0f);
		glVertex3f(-3.0f, -0.85f, -3.0f);
    glEnd();
	glPopMatrix();

    glPushMatrix();

    myDefMesh.glDraw(meshModel);
	myDefMesh.resetSkeletonDeltas();
    
    glPopMatrix();
    
	// Drawing reference coord system
	const int length = 3;
	const GLfloat r[]{ 1, 0, 0, 1 };
	const GLfloat g[]{ 0, 1, 0, 1 };
	const GLfloat b[]{ 0, 0, 1, 1 };

	glLineWidth(5);
	glBegin(GL_LINES);
		glColor4fv(r);
		glVertex3i(0, 0, 0);
		glVertex3i(length, 0, 0);

		glColor4fv(g);
		glVertex3i(0, 0, 0);
		glVertex3i(0, length, 0);

		glColor4fv(b);
		glVertex3i(0, 0, 0);
		glVertex3i(0, 0, length);
	glEnd();

    glutSwapBuffers();
}

//#define _USE_MATH_DEFINES
//#include <math.h>
//
//#define DEG2RADF(x) static_cast<float>(DEG2RAD(x))
//
//void pr(Quatf q)			{ cout << q << endl; }
//void pr(Vector3f v)			{ cout << v << endl; }
//void pr()					{ cout << endl; }
//void pr(const char* str)	{ cout << str << endl; }
//
//typedef Vector3f v;
//typedef Quatf q;

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    //Print context info
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);	//double buffer
    glutInitWindowSize(width, height);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("COMP477");
    glutDisplayFunc(display);
    glutReshapeFunc(changeSize);
    glutTimerFunc(10, timerFunction, 1);

    glutMouseFunc(mouseEvent);
    glutMotionFunc(mouseMoveEvent);
    glutKeyboardFunc(handleKeyPress);
    glutPassiveMotionFunc(mousePassiveFunc);
    
 
    init();
    glutMainLoop();
    //delete something
    return 0;
}

