#include "debugdraw.h"
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QRectF>
#include <QColor>
#include <QOpenGLBuffer>
#include "debugvertex.h"

static size_t m_currOffset;
static QOpenGLBuffer m_buffer;
static QOpenGLVertexArrayObject *m_object;
static QOpenGLShaderProgram *m_program;

static void drawArray(const DebugVertex *array, size_t size)
{
  m_buffer.bind();
  m_buffer.write(m_currOffset, array, size);
  m_buffer.release();
  m_currOffset += size;
}

static QRectF normalize(const QRectF &rect)
{
  float x = rect.x() * 2.0f - 1.0f;
  float y = rect.y() * -2.0f + 1.0f;
  float w = rect.width() * 2.0f;
  float h = rect.height() * 2.0f;
  return QRectF(x, y, w, h);
}

void DebugDraw::initialize(QObject *parent)
{
  // Create shader
  m_program = new QOpenGLShaderProgram(parent);
  m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "./shaders/debug.vert");
  m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "./shaders/debug.frag");
  m_program->link();
  m_program->bind();

  // Create buffer
  m_buffer.create();
  m_buffer.bind();
  m_buffer.setUsagePattern(QOpenGLBuffer::DynamicDraw);
  m_buffer.allocate(sizeof(DebugVertex) * MaxDebugVertex);

  // Create Vertex Array Object
  m_object = new QOpenGLVertexArrayObject(parent);
  m_object->create();
  m_object->bind();
  m_program->enableAttributeArray(0);
  m_program->enableAttributeArray(1);
  m_program->setAttributeBuffer(0, GL_FLOAT, DebugVertex::positionOffset(), DebugVertex::PositionTupleSize, DebugVertex::stride());
  m_program->setAttributeBuffer(1, GL_FLOAT, DebugVertex::colorOffset(), DebugVertex::ColorTupleSize, DebugVertex::stride());

  // Release (unbind) all
  m_object->release();
  m_buffer.release();
  m_program->release();
}

void DebugDraw::Screen::drawRect(const QRectF &rect, const QColor &color)
{
  // Move coordinates to OpenGL NDC
  QRectF nRect = normalize(rect);

  // Create key vertex positions
  float x1 = nRect.x();
  float y1 = nRect.y();
  float x2 = nRect.x() + nRect.width();
  float y2 = nRect.y() - nRect.height();

  // Create vertices
  DebugVertex vertices[] = {
    DebugVertex( QVector3D(x1, y1, 0.0f), color ),
    DebugVertex( QVector3D(x1, y2, 0.0f), color ),
    DebugVertex( QVector3D(x2, y1, 0.0f), color ),
    DebugVertex( QVector3D(x2, y1, 0.0f), color ),
    DebugVertex( QVector3D(x1, y2, 0.0f), color ),
    DebugVertex( QVector3D(x2, y2, 0.0f), color )
  };

  // Queue to draw
  drawArray(vertices, sizeof(vertices));
}

void DebugDraw::draw()
{
  QOpenGLContext *ctx = QOpenGLContext::currentContext();
  QOpenGLFunctions *f = ctx->functions();

  if (m_program)
  {
	  m_program->bind();
	  m_object->bind();
	  {
		  f->glDrawArrays(GL_TRIANGLES, 0, m_currOffset / sizeof(DebugVertex));
	  }
	  m_object->release();
	  m_program->release();

  }

  m_currOffset = 0;
}
