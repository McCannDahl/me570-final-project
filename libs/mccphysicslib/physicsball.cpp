#include "physicsball.h"
#include <cstdlib>

PhysicsBall::PhysicsBall():PhysicsObject()
{
    radius = 0.2 + ((double)std::rand() / RAND_MAX) * (1.0 - 0.2);
}

PhysicsBall::~PhysicsBall()
{

}
