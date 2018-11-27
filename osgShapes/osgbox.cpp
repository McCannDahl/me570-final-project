#include "osgbox.h"

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

osgBox::osgBox()
{
    osg::Vec4 color = osg::Vec4( 1.f, 1.f, 1.f, 1.f );
    osg::Vec3d scaleFactor = osg::Vec3d(size,size,size);
    osg::Vec3Array* v = new osg::Vec3Array;
    v->resize( 8 );
    (*v)[0].set( 1.f, 1.f, 1.f );
    (*v)[1].set(-1.f, 1.f, 1.f );
    (*v)[2].set( 1.f,-1.f, 1.f );
    (*v)[3].set(-1.f,-1.f, 1.f );
    (*v)[4].set( 1.f, 1.f,-1.f );
    (*v)[5].set(-1.f, 1.f,-1.f );
    (*v)[6].set( 1.f,-1.f,-1.f );
    (*v)[7].set(-1.f,-1.f,-1.f );

    osg::Geometry* geom = new osg::Geometry;
    geom->setUseDisplayList( false );
    geom->setVertexArray( v );

    osg::Vec4Array* c = new osg::Vec4Array;
    c->push_back( color );
    geom->setColorArray( c, osg::Array::BIND_OVERALL );

    GLushort idxLines[8] = {0, 4, 1, 5, 2, 6, 3, 7};
    GLushort idxLoops1[4] = {0, 2, 3, 1 };
    GLushort idxLoops2[4] = {4, 6, 7, 5 };

    geom->addPrimitiveSet( new osg::DrawElementsUShort( osg::PrimitiveSet::LINES, 8, idxLines ) );
    geom->addPrimitiveSet( new osg::DrawElementsUShort( osg::PrimitiveSet::LINE_LOOP, 4, idxLoops1 ) );
    geom->addPrimitiveSet( new osg::DrawElementsUShort( osg::PrimitiveSet::LINE_LOOP, 4, idxLoops2 ) );

    osg::Geode* geode = new osg::Geode;
    geode->addDrawable( geom );

    geode->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED );
    geode->getOrCreateStateSet()->setMode( GL_DEPTH_TEST, osg::StateAttribute::ON );
    this->setScale(scaleFactor);
    this->addChild(geode);
    this->setName("osgBox");
}
