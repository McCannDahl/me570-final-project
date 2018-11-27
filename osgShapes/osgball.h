#ifndef OSGBALL_H
#define OSGBALL_H

#include "../libs/mccphysicslib/physicsball.h"

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
#include <QColor>

class osgBall: public osg::PositionAttitudeTransform, public PhysicsBall
{
public:
    osgBall();
    ~osgBall(){}
    void toggle_ball_animation(QString vx,QString vy,QString vz,QString gx,QString gy,QString gz);
    void change_color_of_ball(QColor color);
    osg::Geode* geode;
    osg::StateSet* stateSet;
    osg::Material* material;
    osg::ShapeDrawable* sd;
    osg::Vec4 color;
};



class SphereUpdateCallback: public osg::NodeCallback
{
public:
    SphereUpdateCallback(){}
    virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
    {
        osgBall *ball = dynamic_cast<osgBall*> (node);
        ball->setPosition(osg::Vec3(ball->position.x,ball->position.y,ball->position.z));
        traverse(node, nv);
    }
};


#endif
