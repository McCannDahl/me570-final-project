#include "gtest/gtest.h"

#include "libs/mccphysicslib/physicsball.h"

TEST(GivenBallInit,WhenBallInit_ThenBallIsntNull)
{
    PhysicsBall* ball = new PhysicsBall();
    EXPECT_FALSE(ball==nullptr);
}

TEST(GivenBallInit,WhenBallInit_ThenBallRadiusIsNot1)
{
    PhysicsBall* ball = new PhysicsBall();
    EXPECT_FALSE(ball->radius==1);
}
