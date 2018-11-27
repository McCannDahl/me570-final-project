#include "gtest/gtest.h"

#include "libs/mccphysicslib/physicsroom.h"
#include "libs/mccphysicslib/physicsball.h"
#include "libs/mccphysicslib/physicsbox.h"

TEST(GivenRoomInit,WhenRoomInit_ThenRoomIsntNull)
{
    PhysicsRoom* room = new PhysicsRoom();
    EXPECT_FALSE(room==nullptr);
}

TEST(GivenRoomInit,WhenRoomInit_ThenNumberOfBallIs10)
{
    PhysicsRoom* room = new PhysicsRoom();
    EXPECT_EQ(room->number_of_balls,10);
}

TEST(GivenRoomInit,WhenRoomInit_ThenRoomBallsIsNull)
{
    PhysicsRoom* room = new PhysicsRoom();
    EXPECT_TRUE(room->balls.size() == 0);
}

TEST(GivenRoomInit,WhenRoomInit_ThenRoomBoxIsNull)
{
    PhysicsRoom* room = new PhysicsRoom();
    EXPECT_TRUE(room->box==nullptr);
}

TEST(GivenRoomInitAndBallInitAndBoxInit,WhenRoomCheckForWallCollisionsAndBallOutOfBoxInX_ThenBallReturnsToBox)
{
    PhysicsRoom* room = new PhysicsRoom();
    PhysicsBall* ball = new PhysicsBall();
    PhysicsBox* box = new PhysicsBox();
    ball->position.x = 10;
    room->balls.push_back(ball);
    room->box = box;
    room->check_for_wall_collisions(room->balls[0],box);
    float newPosition = box->size-ball->radius;
    EXPECT_EQ(room->balls[0]->position.x,newPosition);
}

TEST(GivenRoomInitAndBallInitAndBoxInit,WhenRoomCheckForWallCollisionsAndBallOutOfBoxInXYZ_ThenBallReturnsToBoxAndBouncesOffWall)
{
    PhysicsRoom* room = new PhysicsRoom();
    PhysicsBall* ball = new PhysicsBall();
    PhysicsBox* box = new PhysicsBox();
    ball->position.x = 10;
    ball->position.y = 10;
    ball->position.z = 10;
    ball->velocity.x = -8;
    ball->velocity.y = 20;
    ball->velocity.z = 6;
    room->balls.push_back(ball);
    room->box = box;
    room->check_for_wall_collisions(room->balls[0],box);
    float newPositionX = box->size-ball->radius;
    float newPositionY = box->size-ball->radius;
    float newPositionZ = box->size-ball->radius;
    float newVelocityX = -.8f*(-8.0f)*.9f*.9f;
    float newVelocityY = -.8f*(20.0f)*.9f*.9f;
    float newVelocityZ = -.8f*(6.0f)*.9f*.9f;
    EXPECT_EQ(roundf(room->balls[0]->position.x),roundf(newPositionX));
    EXPECT_EQ(roundf(room->balls[0]->position.y),roundf(newPositionY));
    EXPECT_EQ(roundf(room->balls[0]->position.z),roundf(newPositionZ));
    EXPECT_EQ(roundf(room->balls[0]->velocity.x),roundf(newVelocityX));
    EXPECT_EQ(roundf(room->balls[0]->velocity.y),roundf(newVelocityY));
    EXPECT_EQ(roundf(room->balls[0]->velocity.z),roundf(newVelocityZ));
}
TEST(GivenRoomInitAndBallInit,WhenBallsColide_ThenMoveBallsApart1D)
{
    PhysicsRoom* room = new PhysicsRoom();
    PhysicsBall* ball1 = new PhysicsBall();
    PhysicsBall* ball2 = new PhysicsBall();
    ball1->radius = 10;
    ball2->radius = 10;
    ball1->position.x = 0;
    ball1->position.y = 0;
    ball1->position.z = 0;
    ball2->position.x = 18;
    ball2->position.y = 0;
    ball2->position.z = 0;
    room->move_balls_apart(ball1,ball2);
    EXPECT_EQ(ball1->position.x,-1);
    EXPECT_EQ(ball2->position.x,19);
}

TEST(GivenRoomInitAndBallInit,WhenBallsColide_ThenMoveBallsApart3D)
{
    PhysicsRoom* room = new PhysicsRoom();
    PhysicsBall* ball1 = new PhysicsBall();
    PhysicsBall* ball2 = new PhysicsBall();
    ball1->radius = 10;
    ball2->radius = 10;
    ball1->position.x = 0;
    ball1->position.y = 0;
    ball1->position.z = 0;
    ball2->position.x = 10;
    ball2->position.y = 10;
    ball2->position.z = 10;
    room->move_balls_apart(ball1,ball2);
    EXPECT_EQ(ball1->position.x,-0.7735027074813843);
    EXPECT_EQ(ball2->position.x,10.773502349853516);
}

