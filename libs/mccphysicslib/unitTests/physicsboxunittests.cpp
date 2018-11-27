#include "gtest/gtest.h"

#include "libs/mccphysicslib/physicsbox.h"

TEST(GivenBoxInit,WhenBoxInit_ThenBoxIsntNull)
{
    PhysicsBox* box = new PhysicsBox();
    EXPECT_FALSE(box==nullptr);
}

TEST(GivenBoxInit,WhenBoxInit_ThenBoxSizeIs5)
{
    PhysicsBox* box = new PhysicsBox();
    EXPECT_TRUE(box->size==5);
}
