#ifndef OSGWALL_H
#define OSGWALL_H

#include "../libs/mccphysicslib/physicsbox.h"

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

class osgBox: public osg::PositionAttitudeTransform, public PhysicsBox
{
public:
    osgBox();
    ~osgBox(){};
};

#endif
