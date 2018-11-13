#ifndef MEEN_570_OSGWIDGET
#define MEEN_570_OSGWIDGET

#include <QOpenGLWidget>

#include <osg/ref_ptr>
#include <osgViewer/GraphicsWindow>
#include <osgViewer/CompositeViewer>
#include <osgGA/TrackballManipulator>
#include <osgText/Text>
#include <osg/ShapeDrawable>
#include "mainWindow/mainwindow.h"

class OutputWindow;

class OSGWidget : public QOpenGLWidget
{
  Q_OBJECT

public:
  OSGWidget( MainWindow* parent = nullptr, Qt::WindowFlags f = 0, OutputWindow* outputWindow = nullptr);
  virtual ~OSGWidget();
  void set_up_window();
  void set_up_binary_tree();
  void set_up_view();
  void set_aspect_ratio();
  void set_pixel_ratio();
  void set_up_camera();
  void set_up_manipulator();
  void set_up_viewer();
  void go_home();
  void set_up_widget();
  void paintEvent( QPaintEvent* paintEvent );
  void paintGL();
  void resizeGL( int width, int height );

  void keyPressEvent( QKeyEvent* event );
  void keyReleaseEvent( QKeyEvent* event );

  virtual void mouseMoveEvent( QMouseEvent* event );
  virtual void mousePressEvent( QMouseEvent* event );
  virtual void mouseReleaseEvent( QMouseEvent* event );
  void wheelEvent( QWheelEvent* event );
  virtual bool event( QEvent* event );
  void repaint_osg_graphics_after_interaction( QEvent* event );
  void timerEvent(QTimerEvent *);

  void on_resize( int width, int height );
  osgGA::EventQueue* getEventQueue() const;
  float aspectRatio{0};
  float pixelRatio{0};

  osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> mGraphicsWindow;
  osg::ref_ptr<osgViewer::CompositeViewer> mViewer;
  osg::ref_ptr<osgViewer::View> mView;
  osg::ref_ptr<osg::Group> rootNode;
  int mTimerId{0};
  OutputWindow* outputWindow;
  osg::Camera* camera;
  osg::ref_ptr<osgGA::TrackballManipulator> manipulator;
  MainWindow* mainWindow;
};

#endif
