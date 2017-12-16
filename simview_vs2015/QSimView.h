#pragma once
#include <QMainWindow>
#include "ui_simview.h"

class QObj3DView;
class QGLAdjMatrixView;
class QKeyEvent;
class QSimView : public QMainWindow, public Ui_QSimView
{
	Q_OBJECT
public:
	QSimView(QWidget* parent = 0);
	~QSimView();


protected:
	void keyPressEvent(QKeyEvent* event);
protected slots:
	void octreeLevelChanged(int value);
	void setRunId(int value);
private:
	void setupUi();
	void loadData(); // set pre-defined data for testing first

	QObj3DView*                _objView;
	QGLAdjMatrixView*          _graphView;
};

