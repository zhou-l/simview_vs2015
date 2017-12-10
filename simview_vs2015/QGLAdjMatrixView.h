#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include "OpenGL\OpenGLVertexArrayObject"
#include <vector>
#include "Eigen/SparseCore"
#include "FragmentToy.h"

typedef Eigen::SparseMatrix<double> SpDbMat;


class QGLAdjMatrixView : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT
public:
	QGLAdjMatrixView(QWidget* parent = NULL);
	~QGLAdjMatrixView();

	void initializeGL();
	void resizeGL(int width, int height);
	void paintGL();

	void init();	
public slots:
	void octreeLevelChanged(int val);
	
private:
	void destroyTexList();
	void makeObject();
	// opengl objects
	std::vector<GLuint>   _texAdjMatrixList; // array of textures storing all levels of the adjacency matrices
	QOpenGLShaderProgram* _program;
	QOpenGLBuffer         _vbo; // the buffer of the object
	OpenGLVertexArrayObject _object; // vao
	int                   _curTexInList; // item index of the current texture in the texList;

	FragmentToy m_renderer;
private:
	// The list of adjacent matrices for graph representation
	std::vector<Eigen::MatrixXf> _adjMatList; 
	// sparse matrix version for later extension
	std::vector<SpDbMat>         _adjSpMatList;

};

