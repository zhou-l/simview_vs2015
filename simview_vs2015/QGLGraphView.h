#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <vector>

class MassSpringSystem;
class QGLGraphView : public QOpenGLWidget, public QOpenGLFunctions 
{
	Q_OBJECT
public:
	QGLGraphView();
	~QGLGraphView();
	void initializeGL();
	void resizeGL(int width, int height);
	void paintGL();


	void initMassSpring();
private:
	MassSpringSystem*          _massSpring;
	std::vector<std::vector<std::vector< PointMass*> > > _massGrid;
};

