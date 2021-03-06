#include <QApplication>
#include "QSimView.h"
#include <QSurfaceFormat>
int main(int argc, char *argv[])
{
	// Set the window up
	QSurfaceFormat format;
	format.setVersion(3, 3);
	format.setProfile(QSurfaceFormat::CompatibilityProfile);
	format.setDepthBufferSize(24);
	format.setStencilBufferSize(8);
	QSurfaceFormat::setDefaultFormat(format);

  QApplication app(argc, argv);

  // Set OpenGL Version information
  // Note: This format must be set before show() is called.


 
  QSimView window;
  window.show();

  return app.exec();
}
