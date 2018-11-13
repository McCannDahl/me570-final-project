#ifndef OSGWIDGETFRONT_H
#define OSGWIDGETFRONT_H


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

class OSGWidgetFront : public OSGWidget
{
  Q_OBJECT

public:
  OSGWidgetFront( MainWindow* parent = nullptr, Qt::WindowFlags f = 0, OutputWindow* outputWindow = nullptr );
  virtual ~OSGWidgetFront();
};
#endif // OSGWIDGETFRONT_H
