#include "QGLAdjMatrixView.h"
#include "global.h"
#include "volume\octree.h"
using namespace std;

#define PROGRAM_VERTEX_ATTRIBUTE 0
#define PROGRAM_TEXCOORD_ATTRIBUTE 1


QGLAdjMatrixView::QGLAdjMatrixView(QWidget* parent):
	QOpenGLWidget(parent),
	_program(NULL),
	_curTexInList(0)
{

}


QGLAdjMatrixView::~QGLAdjMatrixView()
{
	makeCurrent();
	_vbo.destroy();
	destroyTexList();
	SAFE_DELETE(_program);
	doneCurrent();
}

void QGLAdjMatrixView::initializeGL()
{
	initializeOpenGLFunctions();
	makeObject();

	glDisable(GL_DEPTH_TEST);

	_program = new QOpenGLShaderProgram;
	_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "./shaders/simple2Dview.vert");
	_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "./shaders/simpleTex2D.frag");
	_program->bindAttributeLocation("position", PROGRAM_VERTEX_ATTRIBUTE);
	_program->bindAttributeLocation("texCoord", PROGRAM_TEXCOORD_ATTRIBUTE);
	_program->link();

	_program->bind();
	_program->setUniformValue("tex2D", 0);
	_program->release();


}

void QGLAdjMatrixView::resizeGL(int width, int height)
{
	int side = MIN(width, height); // keep aspect ratio
	glViewport((width - side) / 2, (height - side) / 2, side, side);
}

void QGLAdjMatrixView::paintGL()
{
	QOpenGLWidget::paintGL();

	//glClearColor(1.f, 1.f, 1.f, 1.f);
	glClearColor(1.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	_program->bind();

	_program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
	_program->enableAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE);
	_program->setAttributeBuffer(PROGRAM_VERTEX_ATTRIBUTE, GL_FLOAT, 0, 3, 5 * sizeof(GLfloat)); // a vertex contains 5 float: 3-position, 2-tex
	_program->setAttributeBuffer(PROGRAM_TEXCOORD_ATTRIBUTE, GL_FLOAT, 2 * sizeof(GLfloat), 2, 5 * sizeof(GLfloat));

	// bind current texture

	if (!_texAdjMatrixList.empty())
	{
		int curTexId = -1;
		if( _curTexInList < _texAdjMatrixList.size())
		{
			curTexId = _texAdjMatrixList[_curTexInList];
			if(curTexId > 0)
				glBindTexture(GL_TEXTURE_2D, curTexId);
		}
	}
	
	// Draw!
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindTexture(GL_TEXTURE_2D, 0);
	_program->release();

}

void QGLAdjMatrixView::init()
{
	// Destroy the list of textures if there's anything inside
	if (!_texAdjMatrixList.empty())
	{
		destroyTexList();
		// destroy CPU storage
		_adjMatList.clear();
	}

	// get data from the octree
	octree* oct_tree = g_params.ensembleOctree();

	// get the dimension of leaf nodes
	int levels = oct_tree->levels();
	// create textures
	_texAdjMatrixList.reserve(levels);

	// create CPU storage
	_adjMatList.reserve(levels);

	// allocate memories
	for (int i = 0; i < levels; i++)
	{
		// create GPU tex handle
		GLuint texId = 0;
		glGenTextures(1, &texId);
		glBindTexture(GL_TEXTURE_2D, texId);
		int nodesPerSide = 1 << i;
		int lw = nodesPerSide * nodesPerSide;
		// texture with a single mipmap level
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, lw, lw, 0, GL_RGBA, GL_FLOAT, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		_texAdjMatrixList.push_back(texId);

		// allocate CPU memories
		Eigen::MatrixXf M(lw, lw);
		_adjMatList.push_back(M);

	}

	// set adjacent matrix content
	oct_tree->bfsSetLevelNodesInfo(_adjMatList);

	// upload to the GPU

	for (int i = 0; i < levels; i++)
	{
		int nodesPerSide = 1 << i;
		int lw = nodesPerSide * nodesPerSide;
		GLuint texId = _texAdjMatrixList[i];
		glBindTexture(GL_TEXTURE_2D, texId);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, lw, lw, GL_RGBA, GL_FLOAT, _adjMatList.at(i).data());
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void QGLAdjMatrixView::destroyTexList()
{
	for (size_t i = 0; i < _texAdjMatrixList.size(); i++)
	{
		GLuint texid = _texAdjMatrixList.at(i);
		if (texid != 0)
		{
			glBindTexture(GL_TEXTURE_2D, texid);
			glDeleteTextures(1, &texid);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
	_texAdjMatrixList.clear();
}

void QGLAdjMatrixView::makeObject()
{
	static const float vertCoords[4][3] = {
		{-1.f,-1.f, 0.f},
		{-1.f, 1.f, 0.f},
		{1.f, -1.f, 0.f},
		{1.f, 1.f, 0.f}
	};

	static const float texCoords[4][2] = {
		{0.f, 1.f},
		{0.f, 0.f},
		{1.f, 0.f},
		{1.f, 1.f}
	};
	// create a simple quad with triangle strip!
	QVector<GLfloat> vertData;
	for (int i = 0; i < 4; i++)
	{
		vertData.append(vertCoords[i][0]);
		vertData.append(vertCoords[i][1]);
		vertData.append(vertCoords[i][2]);
		vertData.append(texCoords[i][0]);
		vertData.append(texCoords[i][1]);
	}


	_vbo.create();
	_vbo.bind();
	_vbo.allocate(vertData.constData(), vertData.count() * sizeof(GLfloat));

}