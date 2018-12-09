#include "block.h"
#include "gtest/gtest.h"

TEST(GivenBlockInit,WhenBlockInit_ThenBlockIsntNull)
{
    Block* block = new class Block();
    EXPECT_FALSE(block==nullptr);
}

TEST(GivenBlockInit,WhenBlockSlice_ThenBlockIsSliced)
{
    Block* block = new class Block();
    EXPECT_EQ(block->faces.size(),6);
    float begX = 100;
    float begY = 0;
    float begZ = 0;
    float endX = -100;
    float endY = 0;
    float endZ = 0;
    block->slice(begX,begY,begZ,endX,endY,endZ);
    EXPECT_EQ(block->faces.size(),7);
}

TEST(GivenBlockInit,WhenBlockFindNormalVectorToKnife_ThenCorrectNormalIsReturned)
{
    Block* block = new class Block();
    block->begX = 100;
    block->begY = 0;
    block->begZ = 0;
    block->endX = -100;
    block->endY = 0;
    block->endZ = 0;
    block->find_normal_vector_to_knife();
    osg::Vec3d n = osg::Vec3d(0, 0, 1);
    EXPECT_EQ(block->n,n);
}

TEST(GivenBlockInit,WhenBlockFindIntersectionsBetweenKnifeAndBlock_ThenIntersectionsAreReturnedReturned)
{
    Block* block = new class Block();
    block->begX = 100;
    block->begY = 0;
    block->begZ = 0;
    block->endX = -100;
    block->endY = 0;
    block->endZ = 0;
    block->n = osg::Vec3d(0, 0, 1);
    block->find_intersections_between_knife_and_block();
    EXPECT_EQ(block->newFacesAbove.size(),5);
    EXPECT_EQ(block->newFacesBelow.size(),5);
}

TEST(GivenBlockInit,WhenBlockFindIntersectionsBetweenKnifeAndLine_ThenIntersectionsAreReturnedReturned)
{
    Block* block = new class Block();
    block->begX = 100;
    block->begY = 0;
    block->begZ = 0;
    block->endX = -100;
    block->endY = 0;
    block->endZ = 0;
    block->n = osg::Vec3d(0, 0, 1);
    int faceIndex = 0;
    int lineIndex1 = 0;
    int lineIndex2 = 1;
    block->find_intersections_between_knife_and_line(faceIndex, lineIndex1, lineIndex2);
    EXPECT_EQ(block->newFacesAbove.size(),0);
    EXPECT_EQ(block->newFacesBelow.size(),0);
}

TEST(GivenBlockInit,WhenBlockAddNewKnifePlaneToBlock_ThenKnifePlaneIsAddedToBlock)
{
    Block* block = new class Block();
    EXPECT_EQ(block->faces.size(),6);
    EXPECT_EQ(block->faces.size(),6);
    block->intersections.push_back(osg::Vec3d(0, 0, 1));
    block->add_new_knife_plane_to_block();
    EXPECT_EQ(block->faces.size(),7);
    EXPECT_EQ(block->faces.size(),7);
}

TEST(GivenBlockInit,WhenBlockSetVertexArray_ThenVertexArrayIsSet)
{
    Block* block = new class Block();
    block->set_vertex_array();
    EXPECT_EQ(block->vertexArray.size(),6);
    EXPECT_EQ(block->vertexArray.size(),6);
}

TEST(GivenBlockInit,WhenBlockPutIntersectionsInOrder_ThenIntersectionInOrder)
{
    Block* block = new class Block();
    std::vector<osg::Vec3d> intersections;
    intersections.push_back(osg::Vec3d(1, 0, 0));
    intersections.push_back(osg::Vec3d(-1, 0, 0));
    intersections.push_back(osg::Vec3d(0, 1, 0));
    intersections.push_back(osg::Vec3d(0, -1, 0));
    block->intersections = intersections;
    block->put_intersections_in_order();
    intersections.clear();
    intersections.push_back(osg::Vec3d(1, 0, 0));
    intersections.push_back(osg::Vec3d(0, 1, 0));
    intersections.push_back(osg::Vec3d(-1, 0, 0));
    intersections.push_back(osg::Vec3d(0, -1, 0));
    EXPECT_EQ(block->intersections,intersections);
}

TEST(GivenBlockInit,WhenBlockRoundIntersections_ThenIntersectionsRounded)
{
    Block* block = new class Block();
    std::vector<osg::Vec3d> intersections;
    intersections.push_back(osg::Vec3d(1.000001, 0, 0));
    intersections.push_back(osg::Vec3d(-1.000001, 0, 0));
    intersections.push_back(osg::Vec3d(0.000001, 1, 0));
    intersections.push_back(osg::Vec3d(0.000001, -1, 0));
    block->intersections = intersections;
    block->round_intersections();
    intersections.clear();
    intersections.push_back(osg::Vec3d(1, 0, 0));
    intersections.push_back(osg::Vec3d(-1, 0, 0));
    intersections.push_back(osg::Vec3d(0, 1, 0));
    intersections.push_back(osg::Vec3d(0, -1, 0));
    EXPECT_EQ(block->intersections,intersections);
}

TEST(GivenBlockInit,WhenBlockUndoSlice_ThenBlockIsntSliced)
{
    Block* block = new class Block();
    EXPECT_EQ(block->faces.size(),6);
    float begX = 100;
    float begY = 0;
    float begZ = 0;
    float endX = -100;
    float endY = 0;
    float endZ = 0;
    block->slice(begX,begY,begZ,endX,endY,endZ);
    EXPECT_EQ(block->faces.size(),7);
    block->undo_slice();
    EXPECT_EQ(block->faces.size(),6);
}

TEST(GivenBlockInit,WhenBlockFindIntersectionsBetweenFaceAndPaintBrush_ThenIntersectionsFound)
{
    Block* block = new class Block();
    float begX = 100;
    float begY = 0;
    float begZ = 0;
    float endX = -100;
    float endY = 0;
    float endZ = 0;
    int faceIndex = 0;
    block->intersections.clear();
    block->find_intersections_between_face_and_paint_brush(faceIndex,begX,begY,begZ,endX,endY,endZ);
    EXPECT_EQ(block->intersections.size(),1);
}

TEST(GivenBlockInit,WhenBlockFindIntersectionsBetweenFaceAndBlock_ThenIntersectionsFound)
{
    Block* block = new class Block();
    float begX = 100;
    float begY = 0;
    float begZ = 0;
    float endX = -100;
    float endY = 0;
    float endZ = 0;
    block->intersections.clear();
    block->find_intersections_between_paint_brush_and_block(begX,begY,begZ,endX,endY,endZ);
    EXPECT_EQ(block->intersections.size(),6);
}

TEST(GivenBlockInit,WhenBlockFindNormalVectorToFace_ThenNormalIsFound)
{
    Block* block = new class Block();
    int faceIndex = 0;
    block->find_normal_vector_to_face(faceIndex);
    osg::Vec3d n = osg::Vec3d(0, 0, -1);
    EXPECT_EQ(block->n,n);
}

TEST(GivenBlockInit,WhenPaintFace_ThenFaceIsPainted)
{
    Block* block = new class Block();
    EXPECT_EQ(block->oldColorOfFaces.size(),0);
    float begX = 100;
    float begY = 0;
    float begZ = 0;
    float endX = -100;
    float endY = 0;
    float endZ = 0;
    block->paint_face(begX,begY,begZ,endX,endY,endZ);
    EXPECT_EQ(block->oldColorOfFaces.size(),0);
}


