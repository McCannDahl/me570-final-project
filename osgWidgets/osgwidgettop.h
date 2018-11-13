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
  OSGWidgetTop( MainWindow* parent = nullptr, Qt::WindowFlags f = 0, OutputWindow* outputWindow = nullptr );
  virtual ~OSGWidgetTop();
};

#endif
