#ifndef PHYSICSBALL_H
#define PHYSICSBALL_H

#include "physicsvector.h"
#include "physicsobject.h"

class PhysicsBall: public PhysicsObject
{
public:
    PhysicsBall();
    virtual ~PhysicsBall();
    float radius;
};

#endif
