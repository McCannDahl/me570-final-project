#ifndef BLOCK_H
#define BLOCK_H

#include <osg/Array>
#include <osg/Geometry>
#include <osg/Geode>
#include <osgViewer/Viewer>
#include <vector>
#include <osg/PositionAttitudeTransform>

#include <osg/Material>

class Block
{
public:
    Block();
    std::vector<osg::Vec3d> vertices;
    std::vector<std::vector<osg::Vec3d>> faces;
    std::vector<std::vector<std::vector<osg::Vec3d>>> oldFaces;
    std::vector<std::vector<osg::Vec3d>> newFacesAbove;
    std::vector<std::vector<osg::Vec3d>> newFacesBelow;
    std::vector<osg::Vec3d> newFaceAbove;
    std::vector<osg::Vec3d> newFaceBelow;
    std::vector<osg::Vec3d> intersections;
    std::vector<std::vector<osg::Vec3d>> allIntersectionsCombinations;
    std::vector<osg::Vec3Array*> vertexArray;
    std::vector<osg::Vec4Array*> vertexColorArray;
    std::vector<osg::Vec4f> colorOfFaces;
    std::vector<osg::Vec4f> newColorOfFacesAbove;
    std::vector<osg::Vec4f> newColorOfFacesBelow;
    std::vector<std::vector<osg::Vec4f>> oldColorOfFaces;
    osg::Vec4 paintBrushColor;
    osg::Vec3d n;
    double begX{0};
    double begY{0};
    double begZ{0};
    double endX{0};
    double endY{0};
    double endZ{0};
    int knife_option{0};
    void slice(float begX,float begY,float begZ,float endX,float endY,float endZ);
    void find_normal_vector_to_knife();
    void find_intersections_between_knife_and_block();
    void find_intersections_between_knife_and_line(int faceIndex, int lineIndex1, int lineIndex2);
    void add_new_knife_plane_to_block();
    void set_vertex_array();
    void put_intersections_in_order();
    void put_intersections_in_order_R(std::vector<osg::Vec3d> &availablePoints,std::vector<osg::Vec3d> &unavailablePoints,osg::Vec3d &averageIntersection);
    void round_intersections();
    void keep_biggest_block();
    void keep_smallest_block();
    void keep_both_blocks();
    void undo_slice();
    void find_intersections_between_face_and_paint_brush(int faceIndex, float begX, float begY, float begZ, float endX, float endY, float endZ);
    void find_intersections_between_paint_brush_and_block(float begX, float begY, float begZ, float endX, float endY, float endZ);
    void find_normal_vector_to_face(int faceIndex);
    void paint_face(float begX, float begY, float begZ, float endX, float endY, float endZ);
    void remove_face(float begX, float begY, float begZ, float endX, float endY, float endZ);
    int find_face_closest_to_user(float begX, float begY, float begZ);
    void set_vertex_color_array();
    bool undo_paint();
    void put_intersections_in_order_R(std::vector<osg::Vec3d> availablePoints, std::vector<osg::Vec3d> unavailablePoints);
    void find_normal_vector_to_face(std::vector<osg::Vec3d> faceIndexes);
    void reset();
};

#endif
