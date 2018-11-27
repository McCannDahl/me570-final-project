#include "physicsobject.h"
#include <cstdlib>

PhysicsObject::PhysicsObject():position(PhysicsVector<float>(0.f, 0.f, 0.f)), velocity(PhysicsVector<float>(0.f, 0.f, 0.f)), acceleration(PhysicsVector<float>(0.f, 0.f, 0.f))
{
    velocity.x = 0.0 + ((double)std::rand() / RAND_MAX) * (16.0 - 0.0);
    velocity.y = 0.0 + ((double)std::rand() / RAND_MAX) * (16.0 - 0.0);
    velocity.z = 0.0 + ((double)std::rand() / RAND_MAX) * (16.0 - 0.0);
    mass = 2.0 + ((double)std::rand() / RAND_MAX) * (10.0 - 2.0);
    coefficient_of_restitution = 0.2 + ((double)std::rand() / RAND_MAX) * (1.0 - 0.2);
}

void PhysicsObject::update_physics_object()
{
    if(running)
    {
        velocity = velocity + acceleration*timeStep;
        position = position + velocity*timeStep;
    }
}

void PhysicsObject::randomize_velocity()
{
    velocity.x = -10 + ((double)std::rand() / RAND_MAX) * (10 + 10);
    velocity.y = -10 + ((double)std::rand() / RAND_MAX) * (10 + 10);
    velocity.z = -10 + ((double)std::rand() / RAND_MAX) * (10 + 10);
}
