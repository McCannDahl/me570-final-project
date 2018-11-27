#ifndef PHYSICSOBJECT_H
#define PHYSICSOBJECT_H

#include "physicsvector.h"

class PhysicsObject
{
public:
    PhysicsObject();
    void update_physics_object();
    void randomize_velocity();
    PhysicsVector<float> position;
    PhysicsVector<float> velocity;
    PhysicsVector<float> acceleration;
    bool running{false};
    bool collided{false};
    double timeStep{1.0/60.0};
    float speedScaler{8.f};
    float mass;
    float coefficient_of_restitution;
};

#endif
