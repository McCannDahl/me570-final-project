#include "gtest/gtest.h"

#include "libs/mccphysicslib/physicsobject.h"

TEST(GivenObjectInit,WhenObjectInit_ThenObjectIsntNull)
{
    PhysicsObject* obj = new PhysicsObject();
    EXPECT_FALSE(obj==nullptr);
}

TEST(GivenObjectInit,WhenObjectInit_ThenObjectTimeStepIs1over60)
{
    PhysicsObject* obj = new PhysicsObject();
    EXPECT_TRUE(obj->timeStep==(1.0/60.0));
}

TEST(GivenObjectInit,WhenObjectInit_ThenObjectSpeedScalerIs8)
{
    PhysicsObject* obj = new PhysicsObject();
    EXPECT_TRUE(obj->speedScaler==(8.f));
}

TEST(GivenObjectInit,WhenObjectInit_ThenObjectRunningIsFalse)
{
    PhysicsObject* obj = new PhysicsObject();
    EXPECT_FALSE(obj->running);
}

TEST(GivenObjectInit,WhenObjectInit_ThenObjectPositionIs0)
{
    PhysicsObject* obj = new PhysicsObject();
    EXPECT_TRUE(obj->position.x == 0);
    EXPECT_TRUE(obj->position.y == 0);
    EXPECT_TRUE(obj->position.z == 0);
}

TEST(GivenObjectInit,WhenObjectInit_ThenObjectVelocityIs0)
{
    PhysicsObject* obj = new PhysicsObject();
    EXPECT_FALSE(obj->velocity.x == 0);
    EXPECT_FALSE(obj->velocity.y == 0);
    EXPECT_FALSE(obj->velocity.z == 0);
}

TEST(GivenObjectInit,WhenObjectInit_ThenObjectAccelerationIs0)
{
    PhysicsObject* obj = new PhysicsObject();
    EXPECT_TRUE(obj->acceleration.x == 0);
    EXPECT_TRUE(obj->acceleration.y == 0);
    EXPECT_TRUE(obj->acceleration.z == 0);
}

TEST(GivenObjectInitAndXAccelerationAndNotRunning,WhenUpdate_ThenDoesntUpdate)
{
    PhysicsObject* obj = new PhysicsObject();
    obj->running = false;
    obj->acceleration.x = 5;
    obj->update_physics_object();
    float newVelocity = 5.0*(1.0/60.0);
    float newPosition = 5.0*(1.0/60.0)*(1.0/60.0);
    EXPECT_TRUE(obj->acceleration.x == 5);
    EXPECT_FALSE(obj->velocity.x == newVelocity);
    EXPECT_FALSE(obj->position.x == newPosition);
}

TEST(GivenObjectInitAndXAccelerationAndRunning,WhenUpdate_ThenUpdatesCorrectly)
{
    PhysicsObject* obj = new PhysicsObject();
    obj->running = true;
    obj->position.x = 0;
    obj->velocity.x = 0;
    obj->acceleration.x = 5;
    obj->update_physics_object();
    float newVelocity = 5.0*(1.0/60.0);
    float newPosition = 5.0*(1.0/60.0)*(1.0/60.0);
    EXPECT_TRUE(obj->acceleration.x == 5);
    EXPECT_TRUE(obj->velocity.x == newVelocity);
    EXPECT_TRUE(obj->position.x == newPosition);
}

TEST(GivenObjectInitAndAccelerationAndRunning,WhenUpdate_ThenUpdatesCorrectly)
{
    PhysicsObject* obj = new PhysicsObject();
    obj->running = true;
    obj->position.x = 0;
    obj->position.y = 0;
    obj->position.z = 0;
    obj->velocity.x = 0;
    obj->velocity.y = 0;
    obj->velocity.z = 0;
    obj->acceleration.x = 5;
    obj->acceleration.y = 6;
    obj->acceleration.z = 7;
    obj->update_physics_object();
    float newVelocityX = 5.0*(1.0/60.0);
    float newVelocityY = 6.0*(1.0/60.0);
    float newVelocityZ = 7.0*(1.0/60.0);
    float newPositionX = 5.0*(1.0/60.0)*(1.0/60.0);
    float newPositionY = 6.0*(1.0/60.0)*(1.0/60.0);
    float newPositionZ = 7.0*(1.0/60.0)*(1.0/60.0);
    EXPECT_TRUE(obj->acceleration.x == 5);
    EXPECT_TRUE(obj->velocity.x == newVelocityX);
    EXPECT_TRUE(obj->velocity.y == newVelocityY);
    EXPECT_TRUE(obj->velocity.z == newVelocityZ);
    EXPECT_TRUE(obj->position.x == newPositionX);
    EXPECT_TRUE(obj->position.y == newPositionY);
    EXPECT_TRUE(obj->position.z == newPositionZ);
}
