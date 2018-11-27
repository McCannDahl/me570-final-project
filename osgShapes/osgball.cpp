#include "osgball.h"

#include <osg/ref_ptr>
#include <osgViewer/GraphicsWindow>
#include <osgViewer/CompositeViewer>
#include <osgGA/TrackballManipulator>
#include <osgText/Text>
#include <osg/ShapeDrawable>
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
#include <osgParticle/FireEffect>
#include <cassert>
#include <vector>
#include <QString>
#include <cstdlib>

osgBall::osgBall():PhysicsBall()
{
    osg::Sphere* sphere = new osg::Sphere( osg::Vec3( position.x, position.y, position.z ), radius );
    sd = new osg::ShapeDrawable( sphere );
    color = osg::Vec4( ((double)std::rand() / RAND_MAX), ((double)std::rand() / RAND_MAX), ((double)std::rand() / RAND_MAX), 1.f );
    sd->setColor( color );
    sd->setName( "Sphere" );
    geode = new osg::Geode;
    geode->addDrawable( sd );
    stateSet = geode->getOrCreateStateSet();
    material = new osg::Material;

    material->setColorMode( osg::Material::AMBIENT_AND_DIFFUSE );

    stateSet->setAttributeAndModes( material, osg::StateAttribute::ON );
    stateSet->setMode( GL_DEPTH_TEST, osg::StateAttribute::ON );
    setPosition(osg::Vec3( position.x, position.y, position.z ));
    setUpdateCallback(new SphereUpdateCallback());
    addChild(geode);
    setName("osgBall");
}
void osgBall::toggle_ball_animation(QString vx,QString vy,QString vz,QString gx,QString gy,QString gz)
{
    //todo put this back in
    acceleration = PhysicsVector<float>( gx.toFloat()*speedScaler, gy.toFloat()*speedScaler, gz.toFloat()*speedScaler);
    running = !running;
}
void osgBall::change_color_of_ball(QColor color)
{
    sd->setColor( osg::Vec4( color.redF(), color.greenF(), color.blueF(), color.alphaF() ) );
}

