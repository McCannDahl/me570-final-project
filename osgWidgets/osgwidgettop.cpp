#include "OSGWidgetTop.h"
#include "outputwindow/outputwindow.h"
#include "osgwidget.h"

#include <osg/Camera>
#include <osg/DisplaySettings>
#include <osg/Geode>
#include <osg/Material>
#include <osg/Shape>
#include <osg/ShapeDrawable>
#include <osg/StateSet>
#include <osgDB/WriteFile>
#include <osgGA/EventQueue>
#include <osgViewer/View>
#include <osgViewer/ViewerEventHandlers>
#include <osg/MatrixTransform>
#include <osg/NodeVisitor>
#include <osg/LineWidth>
#include <osgUtil/SmoothingVisitor>
#include <osg/Geometry>
#include <osg/PositionAttitudeTransform>
#include <osg/Node>
#include <QVector4D>
#include <osgParticle/FireEffect>
#include <cassert>
#include <vector>
#include <QKeyEvent>
#include <QPainter>
#include <QWheelEvent>
#include <valarray>

OSGWidgetTop::OSGWidgetTop( MainWindow* parent, Qt::WindowFlags flags, OutputWindow* outputWindow ):
    OSGWidget{ parent, flags, outputWindow }
{
    camera->setViewMatrixAsLookAt(osg::Vec3d(-1600,0,0),osg::Vec3d(0,0,0),osg::Vec3d(0,0,1));
    mView->setCameraManipulator( nullptr );
}

OSGWidgetTop::~OSGWidgetTop()
{

}
