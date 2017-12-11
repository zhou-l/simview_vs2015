#ifndef WINDOW_H
#define WINDOW_H

//#include <QOpenGLWindow>
#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLDebugMessage>
#include <QMatrix4x4>
#include <QElapsedTimer>
#include "transform3d.h"
#include "camera3d.h"

// Custom Includes
#include "OpenGL\OpenGLBuffer"
#include "OpenGL\OpenGLFunctions"
#include "OpenGL\OpenGLVertexArrayObject"


class Profiler;
class FrameResult;
class OpenGLError;
//class OpenGLShaderProgram;
class QOpenGLShaderProgram;
class QOpenGLDebugLogger;
class QMoveEvent;
class DataLoader;
class DataAnalyzer;
class QObj3DView : public QOpenGLWidget,
               protected OpenGLFunctions
{
  Q_OBJECT

// OpenGL Events
public:
  static const uint64_t FPSFrameDelay = 60;
  QObj3DView(QWidget* parent = 0);
  ~QObj3DView();
  void initializeGL();
  void resizeGL(int width, int height);
  void paintGL();

  bool loadSimData();  // load simulation runs and mesh and set vertex buffer

protected slots:
  void teardownGL();
  void update();
  void messageLogged(const QOpenGLDebugMessage &msg);
  void onFrameResult(const FrameResult& result);

protected:
  bool event(QEvent *event);
  void errorEventGL(OpenGLError *event);
  void keyPressEvent(QKeyEvent *event);
  void keyReleaseEvent(QKeyEvent *event);
  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent* event);
  void wheelEvent(QWheelEvent* event);
  void moveEvent(QMoveEvent *ev);

  bool loadData();
  void analyzeData();
private:
  // OpenGL State Information
  OpenGLBuffer m_vertex;
  OpenGLBuffer m_vertexAddOn;


  OpenGLVertexArrayObject m_object;
  OpenGLVertexArrayObject m_objectAddOn;
  //OpenGLShaderProgram *m_program;
  QOpenGLShaderProgram *m_program;
  QOpenGLDebugLogger *m_debugLogger;
  Profiler *m_profiler;
  uint64_t m_frameCount;
  QElapsedTimer m_frameTimer;

  // Shader Information
  int u_modelToWorld;
  int u_worldToCamera;
  int u_cameraToView;
  QMatrix4x4 m_projection;
  Camera3D m_camera;
  QMatrix4x4 m_transform;

  // Data loader
  DataLoader*     m_dataLoader;
  DataAnalyzer*   m_dataAnalyzer;

  // Private Helpers
  void printVersionInformation();

  // arcball
  QVector3D get_arcball_vector(int x, int y);

 
};

#endif // WINDOW_H
