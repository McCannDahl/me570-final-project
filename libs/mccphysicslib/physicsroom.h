#ifndef PHYSICSROOM_H
#define PHYSICSROOM_H

#include "physicsball.h"
#include "physicsbox.h"
#include <vector>

class PhysicsRoom
{
public:
    PhysicsRoom();
    virtual ~PhysicsRoom();
    std::vector<PhysicsBall*> balls{};
    PhysicsBox* box{nullptr};
    virtual void update_physics_room();
    void check_for_wall_collisions(PhysicsBall* ball,PhysicsBox* box);
    int number_of_balls{60};
    void randomize_position_of_balls_in_box();
    void randomize_position_of_ball_in_box(PhysicsBall* ball,PhysicsBox* box);
    bool balls_intersect(PhysicsBall* ball1,PhysicsBall* ball2);
    void move_balls_apart(PhysicsBall *ball1, PhysicsBall *ball2);
    void collide_balls(PhysicsBall *ball1, PhysicsBall *ball2);
};

#endif // PHYSICSROOM_H
