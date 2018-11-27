#include "gtest/gtest.h"

#include "libs/mccphysicslib/physicsvector.h"

TEST(GivenVectorInit,WhenVectorInit_ThenVectorIsntNull)
{
    PhysicsVector<int> vec = PhysicsVector<int>(0,0,0);
    EXPECT_FALSE(&vec == nullptr);
}

TEST(GivenVectorInit,WhenVectorInit_ThenVectorIsSetCorrectly)
{
    PhysicsVector<int> vec = PhysicsVector<int>(5,5,5);
    EXPECT_EQ(vec.x,5);
    EXPECT_EQ(vec.y,5);
    EXPECT_EQ(vec.z,5);
}

TEST(GivenTwoVectorInit,WhenVectorsAdded_ThenNewVectorIsSetCorrectly)
{
    PhysicsVector<float> vec1 = PhysicsVector<float>(5.f,5.f,5.f);
    PhysicsVector<float> vec2 = PhysicsVector<float>(5.f,-6.f,5.f);
    vec1 = vec1 + vec2;
    EXPECT_EQ(vec1.x,10.f);
    EXPECT_EQ(vec1.y,-1.f);
    EXPECT_EQ(vec1.z,10.f);
}

TEST(GivenTwoVectorInit,WhenVectorsSubtracted_ThenNewVectorIsSetCorrectly)
{
    PhysicsVector<int> vec1 = PhysicsVector<int>(5,5,5);
    PhysicsVector<int> vec2 = PhysicsVector<int>(2,-2,88);
    vec1 = vec1 - vec2;
    EXPECT_EQ(vec1.x,3);
    EXPECT_EQ(vec1.y,7);
    EXPECT_EQ(vec1.z,-83);
}

TEST(GivenVectorInit,WhenVectorMultipliedByScaler_ThenVectorIsSetCorrectly)
{
    PhysicsVector<double> vec1 = PhysicsVector<double>(5.7,-3.7,66.3);
    vec1 = vec1 * 7.0;
    EXPECT_EQ(roundl(vec1.x),roundl(39.9));
    EXPECT_EQ(roundl(vec1.y),roundl(-25.9));
    EXPECT_EQ(roundl(vec1.z),roundl(464.1));
}
