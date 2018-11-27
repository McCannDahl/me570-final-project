#include "physicsroom.h"

#include <cstdlib>
#include <cmath>

PhysicsRoom::PhysicsRoom()
{

}

PhysicsRoom::~PhysicsRoom()
{

}

void PhysicsRoom::update_physics_room()
{
    for(int i=0;i<balls.size();i++)
    {
        balls[i]->update_physics_object();
    }
    //todo somehow put a while loop in here to check if everything is okay before moving on

    for(int i=0;i<balls.size();i++)
    {
        balls[i]->collided = false;
    }
    bool overlapsExist = true;
    while(overlapsExist)
    {
        overlapsExist = false;
        for(int i=0;i<balls.size();i++)
        {
            check_for_wall_collisions(balls[i],box);
        }
        for(int i=0;i<balls.size();i++)
        {
            for(int j=0;j<balls.size();j++)
            {
                if(i!=j)
                {
                    if(balls_intersect(balls[i],balls[j]))
                    {
                        overlapsExist = true;
                        move_balls_apart(balls[i],balls[j]);
                        collide_balls(balls[i],balls[j]);
                    }
                }
            }
        }
    }
}

void PhysicsRoom::check_for_wall_collisions(PhysicsBall* ball,PhysicsBox* box)
{
    if(ball!=nullptr && box!=nullptr)
    {
        if(ball->position.x<-(box->size-ball->radius))
        {
            ball->position.x = -(box->size-ball->radius);
            ball->velocity.x = -0.8f*ball->velocity.x;
            ball->velocity.y = 0.9f*ball->velocity.y;
            ball->velocity.z = 0.9f*ball->velocity.z;
            ball->collided = true;
        }
        if(ball->position.x>(box->size-ball->radius))
        {
            ball->position.x = (box->size-ball->radius);
            ball->velocity.x = -0.8f*ball->velocity.x;
            ball->velocity.y = 0.9f*ball->velocity.y;
            ball->velocity.z = 0.9f*ball->velocity.z;
            ball->collided = true;
        }
        if(ball->position.y<-(box->size-ball->radius))
        {
            ball->position.y = -(box->size-ball->radius);
            ball->velocity.x = 0.9f*ball->velocity.x;
            ball->velocity.y = -0.8f*ball->velocity.y;
            ball->velocity.z = 0.9f*ball->velocity.z;
            ball->collided = true;
        }
        if(ball->position.y>(box->size-ball->radius))
        {
            ball->position.y = (box->size-ball->radius);
            ball->velocity.x = 0.9f*ball->velocity.x;
            ball->velocity.y = -0.8f*ball->velocity.y;
            ball->velocity.z = 0.9f*ball->velocity.z;
            ball->collided = true;
        }
        if(ball->position.z<-(box->size-ball->radius))
        {
            ball->position.z = -(box->size-ball->radius);
            ball->velocity.x = 0.9f*ball->velocity.x;
            ball->velocity.y = 0.9f*ball->velocity.y;
            ball->velocity.z = -0.8f*ball->velocity.z;
            ball->collided = true;
        }
        if(ball->position.z>(box->size-ball->radius))
        {
            ball->position.z = (box->size-ball->radius);
            ball->velocity.x = 0.9f*ball->velocity.x;
            ball->velocity.y = 0.9f*ball->velocity.y;
            ball->velocity.z = -0.8f*ball->velocity.z;
            ball->collided = true;
        }
    }
}

void PhysicsRoom::randomize_position_of_balls_in_box()
{
    for(PhysicsBall* ball : balls)
    {
        randomize_position_of_ball_in_box(ball,box);
    }
    bool overlapsExist = true;
    while(overlapsExist)
    {
        overlapsExist = false;
        for(int i=0;i<balls.size();i++)
        {
            for(int j=0;j<balls.size();j++)
            {
                if(i!=j)
                {
                    if(balls_intersect(balls[i],balls[j]))
                    {
                        overlapsExist = true;
                        move_balls_apart(balls[i],balls[j]);
                    }
                }
            }
        }
    }
}
void PhysicsRoom::randomize_position_of_ball_in_box(PhysicsBall* ball,PhysicsBox* box)
{
    ball->position.x = -(box->size-ball->radius) + ((double)std::rand() / RAND_MAX) * ((box->size-ball->radius) + (box->size-ball->radius));
    ball->position.y = -(box->size-ball->radius) + ((double)std::rand() / RAND_MAX) * ((box->size-ball->radius) + (box->size-ball->radius));
    ball->position.z = -(box->size-ball->radius) + ((double)std::rand() / RAND_MAX) * ((box->size-ball->radius) + (box->size-ball->radius));
}
bool PhysicsRoom::balls_intersect(PhysicsBall* ball1,PhysicsBall* ball2)
{
    double r = sqrt( std::pow((double)ball1->position.x-(double)ball2->position.x,2) + std::pow((double)ball1->position.y-(double)ball2->position.y,2) + std::pow((double)ball1->position.z-(double)ball2->position.z,2) );
    if(r <= (ball1->radius+ball2->radius))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void PhysicsRoom::move_balls_apart(PhysicsBall* ball1,PhysicsBall* ball2)
{
    double r = sqrt( std::pow((double)ball1->position.x-(double)ball2->position.x,2) + std::pow((double)ball1->position.y-(double)ball2->position.y,2) + std::pow((double)ball1->position.z-(double)ball2->position.z,2) );
    double d = (ball1->radius+ball2->radius) - r;
    if(d > 0)
    {
        d += .001;
        double r1_to_0 = sqrt( std::pow((double)ball1->position.x,2) + std::pow((double)ball1->position.y,2) + std::pow((double)ball1->position.z,2) );
        double r2_to_0 = sqrt( std::pow((double)ball2->position.x,2) + std::pow((double)ball2->position.y,2) + std::pow((double)ball2->position.z,2) );
        PhysicsVector<float> u(PhysicsVector<float>((ball2->position.x-ball1->position.x)/r,(ball2->position.y-ball1->position.y)/r,(ball2->position.z-ball1->position.z)/r));
        //WAY 1
        //PhysicsVector<float> adjustment = u*d;
        //if(r1_to_0 < r2_to_0)
        //{
        //    ball1->position = ball1->position - adjustment;
        //}
        //else
        //{
        //    ball2->position = ball2->position + adjustment;
        //}
        //WAY 2
        PhysicsVector<float> adjustment = u*(d/2.0);
        double avgMass = (ball1->mass + ball2->mass)/2.0;
        ball1->position = ball1->position - adjustment*(ball1->mass/avgMass);
        ball2->position = ball2->position + adjustment*(ball2->mass/avgMass);
        //double rnew = sqrt( std::pow((double)ball1->position.x-(double)ball2->position.x,2) + std::pow((double)ball1->position.y-(double)ball2->position.y,2) + std::pow((double)ball1->position.z-(double)ball2->position.z,2) );
        //double dnew = (ball1->radius+ball2->radius) - rnew;
    }
}

void PhysicsRoom::collide_balls(PhysicsBall* ball1,PhysicsBall* ball2)
{
    PhysicsVector<float> v1 = ball1->velocity;
    PhysicsVector<float> v2 = ball2->velocity;
    if(!ball1->collided)
    {
        ball1->velocity = (v1*(ball1->mass-ball2->mass)+v2*2*ball2->mass)/(ball1->mass+ball2->mass);
        ball1->collided = true;
    }
    if(!ball2->collided)
    {
        ball2->velocity = (v2*(ball2->mass-ball1->mass)+v1*2*ball1->mass)/(ball1->mass+ball2->mass);
        ball2->collided = true;
    }
}
