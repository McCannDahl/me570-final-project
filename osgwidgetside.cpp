#include "OSGWidgetSide.h"
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

OSGWidgetSide::OSGWidgetSide( MainWindow* parent, Qt::WindowFlags flags, OutputWindow* outputWindow, osg::PositionAttitudeTransform* frame, osg::PositionAttitudeTransform* knife, osg::Vec3Array* knifeV, Block* b  ):
    OSGWidget{ parent, flags, outputWindow, frame, knife, knifeV, false  }
{
    this->block = b;
    camera->setViewMatrixAsLookAt(osg::Vec3d(0.0,1600,0.0),osg::Vec3d(0,0,0),osg::Vec3d(-1,0,0));
    mView->setCameraManipulator( nullptr );
}

OSGWidgetSide::~OSGWidgetSide()
{

}

void OSGWidgetSide::mouseMoveEvent( QMouseEvent* event )
{

    mouseX = ((((double)event->pos().x())/((double)this->width()))*mccWidth*2) - mccWidth;
    mouseY = ((((double)event->pos().y())/((double)this->height()))*mccHeight*2) - mccHeight;
    if(mouseDown)
    {

        osg::Vec3f eye;
        osg::Vec3f center;
        osg::Vec3f up;
        camera->getViewMatrixAsLookAt(eye,center,up);
        if(eye.x()>0||eye.y()>0||eye.z()>0)
        {
            endZ = -mouseX;
        }
        else
        {
            endZ = mouseX;
        }

        endX = mouseY;
        create_wireframe_knife();
        mainWindow->update_all_views();
    }
    else
    {
        osg::Vec3f eye;
        osg::Vec3f center;
        osg::Vec3f up;
        camera->getViewMatrixAsLookAt(eye,center,up);
        if(eye.x()>0||eye.y()>0||eye.z()>0)
        {
            endZ = -mouseX;
        }
        else
        {
            endZ = mouseX;
        }

        endX = mouseY;
        begZ = endZ+1;
        begX = endX+1;
        create_wireframe_knife();
        mainWindow->update_all_views();
    }

    OSGWidget::mouseMoveEvent(event);
}

void OSGWidgetSide::mousePressEvent( QMouseEvent* event )
{
    unsigned int button = 0;
    osg::Vec3f eye;
    osg::Vec3f center;
    osg::Vec3f up;

    switch( event->button() )
    {
    case Qt::LeftButton:
        button = 1;
        mouseX = ((((double)event->pos().x())/((double)this->width()))*mccWidth*2) - mccWidth;
        mouseY = ((((double)event->pos().y())/((double)this->height()))*mccHeight*2) - mccHeight;

        camera->getViewMatrixAsLookAt(eye,center,up);
        if(eye.x()>0||eye.y()>0||eye.z()>0)
        {
            begZ = -mouseX;
        }
        else
        {
            begZ = mouseX;
        }

        begX = mouseY;
        mouseDown = true;
        //outputWindow->print_string( QString::number(event->pos().x())+" "+ QString::number(event->pos().y()));
        break;
    case Qt::RightButton:
        camera->getViewMatrixAsLookAt(eye,center,up);
        block->paint_face(begX,eye.y(),begZ,endX,-eye.y(),begZ);
        redraw_block();
        break;
    default:
        break;
    }
    OSGWidget::mousePressEvent(event);
}

void OSGWidgetSide::mouseReleaseEvent(QMouseEvent* event)
{
    unsigned int button = 0;
    switch( event->button() )
    {
    case Qt::LeftButton:
        button = 1;
        mouseDown = false;
        block->slice(begX,begY,begZ,endX,endY,endZ);
        //outputWindow->print_string( QString::number(event->pos().x())+" "+ QString::number(event->pos().y()));
        redraw_block();
        break;
    default:
        break;
    }
    OSGWidget::mouseReleaseEvent(event);
}

bool OSGWidgetSide::event( QEvent* event )
{
    camera->setProjectionMatrixAsOrtho2D(-1*mccWidth,mccWidth,-1*mccHeight,mccHeight);
    return OSGWidget::event(event);
}


void OSGWidgetSide::create_wireframe_knife()
{
    (*knifeV)[0].set( begX, -175, begZ );
    (*knifeV)[1].set( begX, 175, begZ );
    (*knifeV)[2].set( endX, 175, endZ );
    (*knifeV)[3].set( endX, -175, endZ );
    //this->update();
}


