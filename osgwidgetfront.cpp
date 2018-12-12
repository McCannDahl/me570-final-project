#include "OSGWidgetFront.h"
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

OSGWidgetFront::OSGWidgetFront( MainWindow* parent, Qt::WindowFlags flags, OutputWindow* outputWindow, osg::PositionAttitudeTransform* frame, osg::PositionAttitudeTransform* knife, osg::Vec3Array* knifeV, Block* b  ):
    OSGWidget{ parent, flags, outputWindow, frame, knife, knifeV, false  }
{
    this->block = b;
    camera->setViewMatrixAsLookAt(osg::Vec3d(0,0,-1600),osg::Vec3d(0,0,0),osg::Vec3d(-1,0,0));
    mView->setCameraManipulator( nullptr );
}

OSGWidgetFront::~OSGWidgetFront()
{

}

void OSGWidgetFront::mouseMoveEvent( QMouseEvent* event )
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
            endY = mouseX;
        }
        else
        {
            endY = -mouseX;
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
            endY = mouseX;
        }
        else
        {
            endY = -mouseX;
        }

        endX = mouseY;
        begY = endY+1;
        begX = endX+1;
        create_wireframe_knife();
        mainWindow->update_all_views();
    }

    OSGWidget::mouseMoveEvent(event);
}

void OSGWidgetFront::mousePressEvent( QMouseEvent* event )
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
            begY = mouseX;
        }
        else
        {
            begY = -mouseX;
        }

        begX = mouseY;
        mouseDown = true;
        break;
    case Qt::RightButton:
        camera->getViewMatrixAsLookAt(eye,center,up);
        block->paint_face(begX,begY,eye.z(),endX,endY,-eye.z());
        redraw_block();
        outputWindow->print_string("Paint Face");
        break;
    case Qt::MiddleButton:
        camera->getViewMatrixAsLookAt(eye,center,up);
        block->remove_face(begX,begY,eye.z(),endX,endY,-eye.z());
        redraw_block();
        outputWindow->print_string("Remove Face");
        break;
    default:
        break;
    }
    OSGWidget::mousePressEvent(event);
}

void OSGWidgetFront::mouseReleaseEvent(QMouseEvent* event)
{
    unsigned int button = 0;
    switch( event->button() )
    {
    case Qt::LeftButton:
        button = 1;
        mouseDown = false;
        block->slice(begX,begY,begZ,endX,endY,endZ);
        redraw_block();
        break;
    default:
        break;
    }
    OSGWidget::mouseReleaseEvent(event);
}

bool OSGWidgetFront::event( QEvent* event )
{
    camera->setProjectionMatrixAsOrtho2D(-1*mccWidth,mccWidth,-1*mccHeight,mccHeight);
    return OSGWidget::event(event);
}


void OSGWidgetFront::create_wireframe_knife()
{
    (*knifeV)[0].set( begX, begY,-700 );
    (*knifeV)[1].set( begX, begY, 700 );
    (*knifeV)[2].set( endX, endY, 700 );
    (*knifeV)[3].set( endX, endY,-700 );
}


