#ifndef MEEN_570_OSGWidgetTop
#define MEEN_570_OSGWidgetTop

#include <QOpenGLWidget>

#include <osg/ref_ptr>
#include <osgViewer/GraphicsWindow>
#include <osgViewer/CompositeViewer>
#include <osgGA/TrackballManipulator>
#include <osgText/Text>
#include <osg/ShapeDrawable>

#include "block.h"
#include "osgwidget.h"

class OutputWindow;

class OSGWidgetTop : public OSGWidget
{
  Q_OBJECT

public:
  OSGWidgetTop( MainWindow* parent = nullptr, Qt::WindowFlags f = 0, OutputWindow* outputWindow = nullptr, osg::PositionAttitudeTransform* frame = nullptr, osg::PositionAttitudeTransform* knife = nullptr, osg::Vec3Array* knifeV = nullptr,Block* b = nullptr );
  virtual ~OSGWidgetTop();
  Block* block;
  double mouseX{0};
  double mouseY{0};
  double begX{0};
  double begY{0};
  double begZ{0};
  double endX{0};
  double endY{0};
  double endZ{0};
  double mccWidth{175+100};
  double mccHeight{700+100};
  bool mouseDown{false};
  virtual void mouseMoveEvent( QMouseEvent* event );
  virtual void mousePressEvent( QMouseEvent* event );
  virtual void mouseReleaseEvent( QMouseEvent* event );
  virtual bool event( QEvent* event );
  void create_wireframe_knife();
};

#endif
