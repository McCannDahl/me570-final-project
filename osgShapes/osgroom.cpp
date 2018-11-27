#include "osgroom.h"
#include "osgball.h"
#include "osgbox.h"
#include <vector>

osgRoom::osgRoom():PhysicsRoom{}
{
    setName("osgRoom");
    for(int i=0;i<number_of_balls;i++)
    {
        osgBall* ball = new osgBall();
        balls.push_back(ball);
        addChild(dynamic_cast<osgBall*>(ball));
    }

    //todo delte this
    /*
    osgBall* ball1 = new osgBall();
    balls.push_back(ball1);
    addChild(dynamic_cast<osgBall*>(ball1));
    osgBall* ball2 = new osgBall();
    balls.push_back(ball2);
    addChild(dynamic_cast<osgBall*>(ball2));
    ball1->position.x = 0;
    ball1->position.y = 0;
    ball1->position.z = 2;
    ball1->velocity.x = 0;
    ball1->velocity.y = 0;
    ball1->velocity.z = -2;
    ball1->acceleration.x = 0;
    ball1->acceleration.y = 0;
    ball1->acceleration.z = 0;
    ball2->position.x = 0;
    ball2->position.y = 0;
    ball2->position.z = -2;
    ball2->velocity.x = 0;
    ball2->velocity.y = 0;
    ball2->velocity.z = 2;
    ball2->acceleration.x = 0;
    ball2->acceleration.y = 0;
    ball2->acceleration.z = 0;
    */

    box = new osgBox();
    addChild(dynamic_cast<osgBox*>(box));
    setUpdateCallback(new RoomUpdateCallback());
    randomize_position_of_balls_in_box();
}

void osgRoom::toggle_ball_animation(QString vx, QString vy, QString vz, QString gx, QString gy, QString gz)
{
    randomize_position_of_balls_in_box();
    for(int i=0;i<balls.size();i++)
    {
        dynamic_cast<osgBall*>(balls[i])->randomize_velocity();
        dynamic_cast<osgBall*>(balls[i])->toggle_ball_animation(vx,vy,vz,gx,gy,gz);
    }
}

