#ifndef OSGROOM_H
#define OSGROOM_H

#include <osg/Group>
#include "osgball.h"
#include "osgbox.h"
#include "libs/mccphysicslib/physicsroom.h"

class osgRoom: public PhysicsRoom, public osg::Group
{
public:
    osgRoom();
    virtual ~osgRoom(){}
    void toggle_ball_animation(QString vx,QString vy,QString vz,QString gx,QString gy,QString gz);
};


class RoomUpdateCallback: public osg::NodeCallback
{
public:
    RoomUpdateCallback(){}
    virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
    {
        osgRoom *room = dynamic_cast<osgRoom*> (node);
        room->update_physics_room();
        traverse(node, nv);
    }
};


#endif
