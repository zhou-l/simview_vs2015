#include "QSimView.h"
#include "QObj3DView.h"
#include "QGLAdjMatrixView.h"
#include <QKeyEvent>
#include "global.h"
#include "Volume/octree.h"

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
		_graphView->initData();
		horizontalSlider_level->setMaximum(MAX(0, g_params.ensembleOctree()->levels() - 1));
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

	_objView = new QObj3DView(this);
	horizontalLayout_objView->addWidget(_objView);

	_objView->resize(QSize(800, 600));

	// Create the graph viewer
	_graphView = new QGLAdjMatrixView();
	horizontalLayout_graphView->addWidget(_graphView);
	_graphView->resize(QSize(600, 600));


	horizontalSlider_runs->setMaximum(g_params.numRuns() - 1);
	//resize(QSize(1024, 768));
}

void QSimView::octreeLevelChanged(int value)
{
	if (g_params.ensembleOctree())
	{
		if (value < g_params.ensembleOctree()->levels())
		{
			g_params.setOctreeLevel(value);
			_graphView->octreeLevelChanged(g_params.octreeLevel());
		}
	}
}

void QSimView::setRunId(int value)
{
	// Check the folder of runs
	int runs = g_params.numRuns();
	if (value < runs)
	{
		if (value != g_params.currentRun())
		{
			if (_objView)
				_objView->setRun(value);


			g_params.currentRun(value);

		}

	}
}