#include "QGLGraphView.h"
#include "graph\massspringsystem.h"
#include "global.h"
#include "volume\octree.h"

using namespace std;

QGLGraphView::QGLGraphView()
{
	_massSpring = new MassSpringSystem();
}


QGLGraphView::~QGLGraphView()
{
	SAFE_DELETE(_massSpring);
}

void QGLGraphView::initializeGL()
{
}

void QGLGraphView::resizeGL(int width, int height)
{
}

void QGLGraphView::paintGL()
{
}

void QGLGraphView::initMassSpring()
{
	// get data from the octree
	octree* oct_tree = g_params.ensembleOctree();
	// get the dimension of leaf nodes
	int levels = oct_tree->levels();
	int width = 1<<levels;
	int height = width;
	int depth = width;

	// Set leaf nodes as mass
	// create grid of masses aligned to x-y plane
	_massGrid.resize(depth);
	for (int k = 0; k < depth; k++)
	{
		_massGrid.resize(height);
		for (int j = 0; j<height; j++) {
			_massGrid[j].resize(width);
			for (int i = 0; i<width; i++) {
				_massGrid[j][i][k] = _massSpring->addMass(massValue, i, j, k, Vector(gx, -gy, 0));
			}
		}

	}
	
	// Set fixed masses at the boundary
	int pad = 2;
	for (int i = 0; i<width; i++) {
		for (int j = 0; j < pad; j++)
		{
			massGrid[j][i]->setFixedPosition(true);
			massGrid[height - 1 - j][i]->setFixedPosition(true);
		}
	}
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < pad; i++)
		{
			massGrid[j][i]->setFixedPosition(true);
			massGrid[j][width - 1 - i]->setFixedPosition(true);
		}
	}
	// padding for guarding the boundaries
	// Each mass is connected to it's neighbours with a spring
	float k = springConstantValue;
	float kd = dampingConstantValue;
	float rl = restLengthValue;
	float rlLong = 1.4142 * rl; // diagonal spring
	for (int j = 0; j<height - 1; j++) {
		for (int i = 0; i<width - 1; i++) {
			PointMass *m = massGrid[j][i];
			PointMass *mRight = massGrid[j][i + 1];
			PointMass *mDown = massGrid[j + 1][i];
			PointMass *mDownRight = massGrid[j + 1][i + 1];

			Spring *s1 = _simSystem->addSpring(k, rl, m, mRight);
			Spring *s2 = _simSystem->addSpring(k, rl, m, mDown);
			Spring *s3 = _simSystem->addSpring(k, rlLong, m, mDownRight);
			Spring *s4 = _simSystem->addSpring(k, rlLong, mRight, mDown);
			s1->setDampingConstant(kd);
			s2->setDampingConstant(kd);
			s3->setDampingConstant(kd);
			s4->setDampingConstant(kd);
			_shortSprings.push_back(s1);
			_shortSprings.push_back(s2);
			_longSprings.push_back(s3);
			_longSprings.push_back(s4);
		}
	}

	// right side and bottom side are still not connected with springs
	for (int j = 0; j<height - 1; j++) {
		PointMass *m1 = massGrid[j][width - 1];
		PointMass *m2 = massGrid[j + 1][width - 1];
		Spring *s1 = _simSystem->addSpring(k, rl, m1, m2);
		s1->setDampingConstant(kd);
		_shortSprings.push_back(s1);
	}
	for (int i = 0; i<width - 1; i++) {
		PointMass *m1 = massGrid[height - 1][i];
		PointMass *m2 = massGrid[height - 1][i + 1];
		Spring *s1 = _simSystem->addSpring(k, rl, m1, m2);
		s1->setDampingConstant(kd);
		_shortSprings.push_back(s1);
	}
}
