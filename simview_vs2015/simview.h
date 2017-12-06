#ifndef SIMVIEW_H
#define SIMVIEW_H

#include <QMainWindow>
#include "ui_simview.h"

class simview : public QMainWindow
{
	Q_OBJECT

public:
	simview(QWidget *parent = 0);
	~simview();

private:
	Ui::simviewClass ui;
};


#endif // SIMVIEW_H
