#include "QSimView.h"
#include "QObj3DView.h"
#include "QGLAdjMatrixView.h"
#include <QKeyEvent>

QSimView::QSimView(QWidget* parent):
	QMainWindow(parent),
	_objView(NULL),
	_graphView(NULL)
{
	setupUi();

}


QSimView::~QSimView()
{
}

void QSimView::loadData()
{
	if (_objView->loadSimData())
	{
		_graphView->init();
	}
}

void QSimView::keyPressEvent(QKeyEvent * event)
{
	QMainWindow::keyPressEvent(event);
	switch (event->key())
	{
	case Qt::Key_L:
		loadData();
		break;
	default:
		break;
	}
}

void QSimView::setupUi()
{
	Ui_QSimView::setupUi(this);
	dockWidget_graphView_2->setMinimumSize(QSize(600, 600));



#ifdef    GL_DEBUG
	format.setOption(QSurfaceFormat::DebugContext);
#endif // GL_DEBUG


	// Create the object viewer
	QSurfaceFormat format;
	format.setRenderableType(QSurfaceFormat::OpenGL);
	format.setProfile(QSurfaceFormat::CoreProfile);
	format.setVersion(3, 3);


	_objView = new QObj3DView(this);
	horizontalLayout_objView->addWidget(_objView);
	_objView->setFormat(format);
	_objView->resize(QSize(800, 600));

	// Create the graph viewer
	_graphView = new QGLAdjMatrixView(this);
	horizontalLayout_graphView->addWidget(_graphView);
	_graphView->setFormat(format);
	_graphView->resize(QSize(600, 600));


	//resize(QSize(1024, 768));
}