#include "osgwidget.h"
#include "outputwindow/outputwindow.h"
#include "mainWindow/mainwindow.h"

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

OSGWidget::OSGWidget( MainWindow* parent, Qt::WindowFlags flags, OutputWindow* outputWindow, osg::PositionAttitudeTransform* frame,osg::PositionAttitudeTransform* knife, osg::Vec3Array* knifeV, bool drawWheels  ):
    QOpenGLWidget{ parent,flags },
    mGraphicsWindow{ new osgViewer::GraphicsWindowEmbedded{ this->x(),this->y(),this->width(),this->height() } },
    mViewer{ new osgViewer::CompositeViewer }
{
    this->mainWindow = parent;
    this->outputWindow = outputWindow;
    this->frame = frame;
    this->knife = knife;
    this->knifeV = knifeV;
    this->drawWheels = drawWheels;
    set_up_window();
    set_up_widget();
}

OSGWidget::~OSGWidget()
{
    killTimer(mTimerId);
}


void OSGWidget::timerEvent(QTimerEvent *)
{
    update();
}

void OSGWidget::set_up_window()
{
    set_up_binary_tree();

    set_aspect_ratio();
    set_pixel_ratio();

    set_up_camera();

    set_up_view();

    set_up_manipulator();

    set_up_viewer();
    go_home();
    rootNode->addChild(frame);
    rootNode->addChild(knife);

    add_wheels();
}

void OSGWidget::set_up_binary_tree()
{
    rootNode = new osg::Group;
    mView = new osgViewer::View;
}
void OSGWidget::set_aspect_ratio()
{
    aspectRatio = static_cast<float>( this->width() ) / static_cast<float>( this->height() );
}
void OSGWidget::set_pixel_ratio()
{
    pixelRatio =  this->devicePixelRatio();
}
void OSGWidget::set_up_camera()
{
    camera = new osg::Camera;
    camera->setViewport( 0, 0, this->width() * pixelRatio, this->height() * pixelRatio );
    camera->setClearColor( osg::Vec4( 0.3f, 0.3f, 0.3f, 1.f ) );
    camera->setProjectionMatrixAsPerspective( 45.f, aspectRatio, 1.f, 1000.f );
    camera->setGraphicsContext( mGraphicsWindow );
    mView->setCamera( camera );
}

void OSGWidget::set_up_manipulator()
{
    manipulator = new osgGA::TrackballManipulator;
    //manipulator->setAllowThrow( false );
    manipulator->setHomePosition(osg::Vec3d(-500.0,500.0,-1600.0),osg::Vec3d(0,0,0),osg::Vec3d(-1,0,0));
    mView->setCameraManipulator( manipulator );
}

void OSGWidget::set_up_view()
{
    mView->setSceneData( rootNode.get() );
    mView->addEventHandler( new osgViewer::StatsHandler );
}
void OSGWidget::set_up_viewer()
{
    mViewer->addView( mView );
    mViewer->setThreadingModel( osgViewer::CompositeViewer::SingleThreaded );
    mViewer->realize();
}
void OSGWidget::go_home()
{
    mView->home();
}

void OSGWidget::set_up_widget()
{
    this->setFocusPolicy( Qt::StrongFocus );
    this->setMinimumSize( 100, 100 );
    this->setMouseTracking( true );
    this->update();
    double timeStep{1.0/60.0};
    double timerDurationInMilliSeconds{timeStep * 1000};
    mTimerId = startTimer(timerDurationInMilliSeconds);
}


void OSGWidget::paintEvent( QPaintEvent* )
{
    this->makeCurrent();
    QPainter painter( this );
    painter.setRenderHint( QPainter::Antialiasing );
    this->paintGL();
    painter.end();
    this->doneCurrent();
}

void OSGWidget::paintGL()
{
    mViewer->frame();
}

void OSGWidget::resizeGL( int width, int height )
{
    this->getEventQueue()->windowResize( this->x(), this->y(), width, height );
    mGraphicsWindow->resized( this->x(), this->y(), width, height );
    this->on_resize( width, height );
}

void OSGWidget::keyPressEvent( QKeyEvent* event )
{
    QString keyString   = event->text();
    const char* keyData = keyString.toLocal8Bit().data();

    if( event->key() == Qt::Key_H )
    {
        mView->home();
        return;
    }

    this->getEventQueue()->keyPress( osgGA::GUIEventAdapter::KeySymbol( *keyData ) );
}

void OSGWidget::keyReleaseEvent( QKeyEvent* event )
{
    QString keyString   = event->text();
    const char* keyData = keyString.toLocal8Bit().data();

    this->getEventQueue()->keyRelease( osgGA::GUIEventAdapter::KeySymbol( *keyData ) );
}

void OSGWidget::mouseMoveEvent( QMouseEvent* event )
{

    auto pixelRatio = this->devicePixelRatio();
    //outputWindow->print_string( QString::number(event->pos().x())+" "+ QString::number(event->pos().y()));

    this->getEventQueue()->mouseMotion( static_cast<float>( event->x() * pixelRatio ), static_cast<float>( event->y() * pixelRatio ) );
}

void OSGWidget::mousePressEvent( QMouseEvent* event )
{
    unsigned int button = 0;

    switch( event->button() )
    {
    case Qt::LeftButton:
        button = 1;
        //outputWindow->print_string( QString::number(event->pos().x())+" "+ QString::number(event->pos().y()));
        break;

    case Qt::MiddleButton:
        button = 2;
        break;

    case Qt::RightButton:
        button = 3;
        break;

    default:
        break;
    }

    auto pixelRatio = this->devicePixelRatio();

    this->getEventQueue()->mouseButtonPress( static_cast<float>( event->x() * pixelRatio ), static_cast<float>( event->y() * pixelRatio ), button );
}

void OSGWidget::mouseReleaseEvent(QMouseEvent* event)
{
    unsigned int button = 0;

    switch( event->button() )
    {
    case Qt::LeftButton:
        button = 1;
        //outputWindow->print_string( QString::number(event->pos().x())+" "+ QString::number(event->pos().y()));
        (*knifeV)[0].set( 0, 0, 0 );
        (*knifeV)[1].set( 0, 0, 0 );
        (*knifeV)[2].set( 0, 0, 0 );
        (*knifeV)[3].set( 0, 0, 0 );
        break;

    case Qt::MiddleButton:
        button = 2;
        break;

    case Qt::RightButton:
        button = 3;
        break;

    default:
        break;
    }

    auto pixelRatio = this->devicePixelRatio();

    this->getEventQueue()->mouseButtonRelease( static_cast<float>( pixelRatio * event->x() ), static_cast<float>( pixelRatio * event->y() ), button );
}

void OSGWidget::wheelEvent( QWheelEvent* event )
{
    event->accept();
    int delta = event->delta();

    osgGA::GUIEventAdapter::ScrollingMotion motion = delta > 0 ?   osgGA::GUIEventAdapter::SCROLL_UP : osgGA::GUIEventAdapter::SCROLL_DOWN;

    this->getEventQueue()->mouseScroll( motion );
}

bool OSGWidget::event( QEvent* event )
{
    bool handled = QOpenGLWidget::event( event );
    repaint_osg_graphics_after_interaction(event);
    return handled;
}



void OSGWidget::repaint_osg_graphics_after_interaction(QEvent* event)
{
    switch( event->type() )
    {
        case QEvent::KeyPress:
        case QEvent::KeyRelease:
        case QEvent::MouseButtonDblClick:
        case QEvent::MouseButtonPress:
        case QEvent::MouseButtonRelease:
        case QEvent::MouseMove:
        case QEvent::Wheel:
            this->update();
            break;
        default:
            break;
    }
}


void OSGWidget::on_resize( int width, int height )
{
    std::vector<osg::Camera*> cameras;
    mViewer->getCameras( cameras );

    auto pixelRatio = this->devicePixelRatio();

    cameras[0]->setViewport( 0, 0, width * pixelRatio, height * pixelRatio );
}

osgGA::EventQueue* OSGWidget::getEventQueue() const
{
    osgGA::EventQueue* eventQueue = mGraphicsWindow->getEventQueue();

    if( eventQueue )
        return eventQueue;
    else
        throw std::runtime_error( "Unable to obtain valid event queue");
}

void OSGWidget::redraw_block()
{
    mainWindow->redraw_wireframe_tetrahedron();
}

void OSGWidget::add_wheels()
{
    if(drawWheels)
    {
        osg::Vec3 StartPoint = osg::Vec3(125,175,269*2);
        osg::Vec3 EndPoint = osg::Vec3(125,175+41.5*2,269*2);
        rootNode->addChild(create_wheels(StartPoint,EndPoint));
        StartPoint = osg::Vec3(125,175,-172*2);
        EndPoint = osg::Vec3(125,175+41.5*2,-172*2);
        rootNode->addChild(create_wheels(StartPoint,EndPoint));
        StartPoint = osg::Vec3(125,-175,-172*2);
        EndPoint = osg::Vec3(125,-175-41.5*2,-172*2);
        rootNode->addChild(create_wheels(StartPoint,EndPoint));
        StartPoint = osg::Vec3(125,-175,269*2);
        EndPoint = osg::Vec3(125,-175-41.5*2,269*2);
        rootNode->addChild(create_wheels(StartPoint,EndPoint));
    }
}

osg::PositionAttitudeTransform* OSGWidget::create_wheels(osg::Vec3 StartPoint,osg::Vec3 EndPoint)
{
    float radius = 119;
    osg::Vec4 CylinderColor = osg::Vec4(0.5f,0.5f,0.5f,1.f);

    osg::Vec3 center;
    float height;

    osg::ref_ptr<osg::Cylinder> cylinder;
    osg::ref_ptr<osg::ShapeDrawable> cylinderDrawable;
    osg::ref_ptr<osg::Material> material;
    osg::ref_ptr<osg::Geode> geode;

    height = (StartPoint- EndPoint).length();
    center = osg::Vec3( (StartPoint.x() + EndPoint.x()) / 2,  (StartPoint.y() + EndPoint.y()) / 2,  (StartPoint.z() + EndPoint.z()) / 2);

    // This is the default direction for the cylinders to face in OpenGL
    osg::Vec3   z = osg::Vec3(0,0,1);

    // Get diff between two points you want cylinder along
    osg::Vec3 p = (StartPoint - EndPoint);

    // Get CROSS product (the axis of rotation)
    osg::Vec3   t = z ^  p;

    // Get angle. length is magnitude of the vector
    double angle = acos( (z * p) / p.length());

    //   Create a cylinder between the two points with the given radius
    cylinder = new osg::Cylinder(center,radius,height);
    cylinder->setRotation(osg::Quat(angle, osg::Vec3(t.x(), t.y(), t.z())));

    //   A geode to hold our cylinder
    geode = new osg::Geode;
    cylinderDrawable = new osg::ShapeDrawable(cylinder);

    geode->addDrawable(cylinderDrawable);

    //   Set the color of the cylinder that extends between the two points.
    material = new osg::Material;
    material->setDiffuse( osg::Material::FRONT, CylinderColor);
    geode->getOrCreateStateSet()->setAttributeAndModes( material, osg::StateAttribute::ON );
    geode->getOrCreateStateSet()->setMode( GL_DEPTH_TEST, osg::StateAttribute::ON );

    //   Add the cylinder between the two points to an existing group
    osg::PositionAttitudeTransform* transform3 = new osg::PositionAttitudeTransform;
    transform3->addChild(geode);
    return transform3;
}

void OSGWidget::flipView()
{
    osg::Vec3f eye;
    osg::Vec3f center;
    osg::Vec3f up;
    camera->getViewMatrixAsLookAt(eye,center,up);
    camera->setViewMatrixAsLookAt(-eye,center,up);
    this->update();
}


void OSGWidget::flipCamera()
{
    osg::Vec3f eye;
    osg::Vec3f center;
    osg::Vec3f up;
    camera->getViewMatrixAsLookAt(eye,center,up);
    camera->setViewMatrixAsLookAt(eye,center,-up);
    this->update();
}

