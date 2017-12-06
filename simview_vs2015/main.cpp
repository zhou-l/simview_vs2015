#include <QApplication>
#include "QSimView.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  // Set OpenGL Version information
  // Note: This format must be set before show() is called.


  // Set the window up
  QSimView window;
  window.show();

  return app.exec();
}
