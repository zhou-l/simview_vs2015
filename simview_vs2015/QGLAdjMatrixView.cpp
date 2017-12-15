#include "QGLAdjMatrixView.h"
#include "global.h"
#include "volume\octree.h"
//#include "GLError.h"
using namespace std;

#define PROGRAM_VERTEX_ATTRIBUTE 0
#define PROGRAM_TEXCOORD_ATTRIBUTE 1


QGLAdjMatrixView::QGLAdjMatrixView(QWidget* parent) :
	QOpenGLWidget(parent),
	_program(NULL),
	_curTexInList(0)
	//,
	//m_renderer("./shaders/simpleTextured2D.frag")
{

}


QGLAdjMatrixView::~QGLAdjMatrixView()
{
	makeCurrent();
	_vbo.destroy();
	_object.destroy();
	destroyTexList();
	SAFE_DELETE(_program);
	doneCurrent();
}

void QGLAdjMatrixView::initializeGL()
{

	if (!_vbo.isCreated())
	{
		initializeOpenGLFunctions();
		makeObject();
	}


	glDisable(GL_DEPTH_TEST);

	if (_program == NULL)
	{
		_program = new QOpenGLShaderProgram;
		_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "./shaders/simple2Dview.vert");
		_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "./shaders/simpleTex2DColormapped.frag");
		_program->bindAttributeLocation("position", PROGRAM_VERTEX_ATTRIBUTE);
		_program->bindAttributeLocation("texCoord", PROGRAM_TEXCOORD_ATTRIBUTE);
		_program->link();

		_program->bind();
		_program->setUniformValue("tex2D", 0);
		_program->setUniformValue("flipY", true);

		// Create object
		_vbo.bind();
		_object.create();
		_object.bind();

		_program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
		_program->enableAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE);
		_program->setAttributeBuffer(PROGRAM_VERTEX_ATTRIBUTE, GL_FLOAT, 0, 3, 5 * sizeof(GLfloat)); // a vertex contains 5 float: 3-position, 2-tex
		_program->setAttributeBuffer(PROGRAM_TEXCOORD_ATTRIBUTE, GL_FLOAT, 3 * sizeof(GLfloat), 2, 5 * sizeof(GLfloat));

		_object.release();
		_vbo.release();
		_program->release();

	}

	// Destroy the list of textures if there's anything inside
	if (!_texAdjMatrixList.empty())
	{
		destroyTexList();
	}

	if (!_adjMatList.empty())
	{
		if (_adjMatList[0].cols() > 0) // make sure the matrices do have elements
		{
			// Setup texture
			// upload to the GPU

			//for (size_t i = 0; i < _adjMatList.size(); i++)
			//{
			//	cout<<"mat level = "<<i<<endl
			//		<<_adjMatList[i]<<endl;
			//}
			for (int i = 0; i < int(_adjMatList.size()); i++)
			{
				int nodesPerSide = 1 << i;
				int lw = nodesPerSide * nodesPerSide * nodesPerSide;
				// Get matrix content
				Eigen::MatrixXf mat = _adjMatList.at(i);
				float* matContent = mat.data();
				// create GPU tex handle
				GLuint texId = 0;
				glPixelStorei(GL_PACK_ALIGNMENT, 1);
				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
				glGenTextures(1, &texId);
				glBindTexture(GL_TEXTURE_2D, texId);

				cout << "Level = " << i << ", mat cols = " << lw << endl;
				// texture with a single mipmap level

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, lw, lw, 0, GL_RED, GL_FLOAT, matContent);

				//GLenum err = glGetError();
				//check_gl_error();
				glBindTexture(GL_TEXTURE_2D, 0);
				_texAdjMatrixList.push_back(texId);
				cout << "TexId generated = " << texId << endl;


				// Get tex image content
				memset(matContent, 0, UINT64(lw) * UINT64(lw) * sizeof(float));

				glBindTexture(GL_TEXTURE_2D, _texAdjMatrixList.back());
				glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, matContent);
				glBindTexture(GL_TEXTURE_2D, 0);
				//check_gl_error();
				QString ofName = QString("matContentLevel%1.txt").arg(i);
				ofstream ofMatContent(ofName.toStdString().c_str());
				for (UINT64 r = 0; r < UINT64(lw); r++)
				{
					for (UINT64 c = 0; c < UINT64(lw); c++)
					{
						//ofMatContent << texContent[3 * (r * UINT64(lw) + c)] << " ";
						ofMatContent << matContent[(r * UINT64(lw) + c)] << " ";
					
					}
					ofMatContent << endl;
				}
				ofMatContent.close();
			}

			for (size_t i = 0; i < _texAdjMatrixList.size(); i++)
				cout << _texAdjMatrixList[i] << endl;
		}
		else
			_adjMatList.clear();
	}

}

void QGLAdjMatrixView::resizeGL(int width, int height)
{
	int side = MIN(width, height); // keep aspect ratio
	glViewport((width - side) / 2, (height - side) / 2, side, side);
}

void QGLAdjMatrixView::paintGL()
{
	makeCurrent();
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_program->bind();
	_object.bind();

	// bind current texture
	GLuint curTexId = 0;
	glActiveTexture(GL_TEXTURE0);

	if (!_texAdjMatrixList.empty())
	{
		if( _curTexInList < _texAdjMatrixList.size())
		{
			curTexId = _texAdjMatrixList[_curTexInList];
		}
	}
	if (curTexId != 0)
		qWarning() << "TexId = " << curTexId << endl;
	glBindTexture(GL_TEXTURE_2D, curTexId);
	// Draw!
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	_object.release();
	
	//unbind	
	_program->release();

}

void QGLAdjMatrixView::initData()
{
	
	_adjMatList.clear();
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
	
		int nodesPerSide = 1 << i;
		int lw = nodesPerSide * nodesPerSide * nodesPerSide;
		// allocate CPU memories
		Eigen::MatrixXf M = Eigen::MatrixXf::Constant(lw, lw, 0.0f);
		_adjMatList.push_back(M);

	}

	// set adjacent matrix content
	oct_tree->bfsSetLevelNodesInfo(_adjMatList);

	
	cout << __FUNCTION__<<"...done!" << endl;
	// Reinitialize opengl
	initializeGL();
	// Update rendering
	update();
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
		{0.f, 0.f},
		{0.f, 1.f},
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
	_vbo.release();
}

void QGLAdjMatrixView::octreeLevelChanged(int val)
{
	_curTexInList = val;
	update();
}