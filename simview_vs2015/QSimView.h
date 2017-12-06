#pragma once
#include <QMainWindow>
#include "ui_simview.h"

class QObj3DView;
class QGLAdjMatrixView;

class QSimView : public QMainWindow, public Ui_QSimView
{
	Q_OBJECT
public:
	QSimView(QWidget* parent = 0);
	~QSimView();
private:
	void setupUi();

	QObj3DView*                _objView;
	QGLAdjMatrixView*          _graphView;
};

