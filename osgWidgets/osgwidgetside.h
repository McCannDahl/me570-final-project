#ifndef OSGWIDGETSIDE_H
#define OSGWIDGETSIDE_H


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

class OSGWidgetSide : public OSGWidget
{
  Q_OBJECT

public:
  OSGWidgetSide( MainWindow* parent = nullptr, Qt::WindowFlags f = 0, OutputWindow* outputWindow = nullptr );
  virtual ~OSGWidgetSide();

};

#endif // OSGWIDGETSIDE_H
