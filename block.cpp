#include "block.h"

#include <osg/Array>
#include <osg/Geometry>
#include <osg/Geode>
#include <osgViewer/Viewer>
#include <vector>
#include <osg/PositionAttitudeTransform>
#include <osg/Material>
#include <QtDebug>

Block::Block()
{
    paintBrushColor = osg::Vec4( 1.0f, 0.8f, 0.2f, 1.f );

    vertices.clear();
    vertices.push_back(osg::Vec3d( 125, 175, 700));
    vertices.push_back(osg::Vec3d(-125, 175, 700));
    vertices.push_back(osg::Vec3d(-125,-175, 700));
    vertices.push_back(osg::Vec3d( 125,-175, 700));
    vertices.push_back(osg::Vec3d( 125,-175,-700));
    vertices.push_back(osg::Vec3d(-125,-175,-700));
    vertices.push_back(osg::Vec3d(-125, 175,-700));
    vertices.push_back(osg::Vec3d( 125, 175,-700));

    std::vector<osg::Vec3d> face;

    face.clear();
    face.push_back(vertices[0]);
    face.push_back(vertices[1]);
    face.push_back(vertices[2]);
    face.push_back(vertices[3]);
    faces.push_back(face);
    colorOfFaces.push_back(paintBrushColor);


    face.clear();
    face.push_back(vertices[4]);
    face.push_back(vertices[5]);
    face.push_back(vertices[6]);
    face.push_back(vertices[7]);
    faces.push_back(face);
    colorOfFaces.push_back(paintBrushColor);

    face.clear();
    face.push_back(vertices[0]);
    face.push_back(vertices[1]);
    face.push_back(vertices[6]);
    face.push_back(vertices[7]);
    faces.push_back(face);
    colorOfFaces.push_back(paintBrushColor);

    face.clear();
    face.push_back(vertices[2]);
    face.push_back(vertices[3]);
    face.push_back(vertices[4]);
    face.push_back(vertices[5]);
    faces.push_back(face);
    colorOfFaces.push_back(paintBrushColor);

    face.clear();
    face.push_back(vertices[0]);
    face.push_back(vertices[3]);
    face.push_back(vertices[4]);
    face.push_back(vertices[7]);
    faces.push_back(face);
    colorOfFaces.push_back(paintBrushColor);

    face.clear();
    face.push_back(vertices[1]);
    face.push_back(vertices[2]);
    face.push_back(vertices[5]);
    face.push_back(vertices[6]);
    faces.push_back(face);
    colorOfFaces.push_back(paintBrushColor);


    set_vertex_array();
    set_vertex_color_array();
}


void Block::slice(float begX,float begY,float begZ,float endX,float endY,float endZ)
{
    if(begX==endX && begY==endY && begZ==endZ)
    {
        return;
    }
    qDebug() << "slice " << begX << " " << begY << " " << begZ << " " << endX << " " << endY << " " << endZ;
    this->begX = begX;
    this->begY = begY;
    this->begZ = begZ;
    this->endX = endX;
    this->endY = endY;
    this->endZ = endZ;
    find_normal_vector_to_knife();
    find_intersections_between_knife_and_block();
    oldFaces.push_back(faces);
    oldColorOfFaces.push_back(colorOfFaces);
    keep_biggest_block();
    if(knife_option == 0)
    {
        keep_biggest_block();
        add_new_knife_plane_to_block();
    }
    else if(knife_option == 1)
    {
        keep_smallest_block();
        add_new_knife_plane_to_block();
    }
    else if(knife_option == 2)
    {
        keep_both_blocks();
        add_new_knife_plane_to_block();
    }
    set_vertex_array();
    set_vertex_color_array();
}

void Block::find_normal_vector_to_knife()
{
    qDebug() << "find_normal_vector_to_knife ";
    if(begX-endX==0)
    {
        n = osg::Vec3d(begX-endX, begY-endY, begZ-endZ)^osg::Vec3d(100, 0, 0);
    }
    else if(begY-endY==0)
    {
        n = osg::Vec3d(begX-endX, begY-endY, begZ-endZ)^osg::Vec3d(0, 100, 0);
    }
    else if(begZ-endZ==0)
    {
        n = osg::Vec3d(begX-endX, begY-endY, begZ-endZ)^osg::Vec3d(0, 0, 100);
    }
    else
    {
        qDebug() << "error no normal vector found";
    }
    n.normalize();
}
void Block::find_intersections_between_knife_and_block()
{
    qDebug() << "find_intersections_between_knife_and_block ";
    intersections.clear();
    newFacesAbove.clear();
    newFacesBelow.clear();
    newColorOfFacesAbove.clear();
    newColorOfFacesBelow.clear();
    for(int i=0;i<faces.size();i++)
    {
        newFaceAbove.clear();
        newFaceBelow.clear();
        for(int j=0;j<faces[i].size();j++)
        {
            qDebug() << "faces " << faces[i][j].x() << " " << faces[i][j].y() << " " << faces[i][j].z() << " ";

            if(j==faces[i].size()-1)
            {
                find_intersections_between_knife_and_line(i,j,0);
            }
            else
            {
                find_intersections_between_knife_and_line(i,j,j+1);
            }
        }
        if(newFaceAbove.size()>0){
            newFacesAbove.push_back(newFaceAbove);
            newColorOfFacesAbove.push_back(colorOfFaces[i]);
        }
        if(newFaceBelow.size()>0){
            newFacesBelow.push_back(newFaceBelow);
            newColorOfFacesBelow.push_back(colorOfFaces[i]);
        }
    }
}
void Block::find_intersections_between_knife_and_line(int faceIndex,int lineIndex1,int lineIndex2)
{
    qDebug() << "find_intersections_between_knife_and_line ";
    osg::Vec3d p1 = faces[faceIndex][lineIndex1];
    osg::Vec3d p2 = faces[faceIndex][lineIndex2];
    osg::Vec3d p3 = osg::Vec3d(begX,begY,begZ);

    osg::Vec3d p3_to_p1 = p1-p3;
    if(p3_to_p1*n > 0)
    {
        qDebug() << "P1 is above";
        newFaceAbove.push_back(p1);
    }
    else if(p3_to_p1*n < 0)
    {
        qDebug() << "P1 is below";
        newFaceBelow.push_back(p1);
    }

    osg::Vec3d intersect = osg::Vec3d(0,0,0);
    double u = (n.x()*(p3.x()-p2.x()) + n.y()*(p3.y()-p2.y()) + n.z()*(p3.z()-p2.z())) / (n.x()*(p1.x()-p2.x()) + n.y()*(p1.y()-p2.y()) + n.z()*(p1.z()-p2.z()));
    if (u > 0.0 && u < 1.0)
    {
        intersect.x() = u * p1.x() + (1-u) * p2.x();
        intersect.y() = u * p1.y() + (1-u) * p2.y();
        intersect.z() = u * p1.z() + (1-u) * p2.z();
        intersections.push_back(intersect);
        qDebug() << "intersect " << intersect.x() << " " << intersect.y() << " " << intersect.z() << " ";
        newFaceAbove.push_back(intersect);
        newFaceBelow.push_back(intersect);
    }
}

void Block::keep_biggest_block()
{
    qDebug() << "keep_biggest_block ";
    double abovePerimeter{0};
    for(int i=0;i<newFacesAbove.size();i++)
    {
        for(int j=0;j<newFacesAbove[i].size();j++)
        {
            //qDebug() << "newFacesAbove " << newFacesAbove[i][j].x() << " " << newFacesAbove[i][j].y() << " " << newFacesAbove[i][j].z() << " ";
            if(j==newFacesAbove[i].size()-1)
            {
                abovePerimeter += (newFacesAbove[i][j]-newFacesAbove[i][0]).length();
            }
            else
            {
                abovePerimeter += (newFacesAbove[i][j]-newFacesAbove[i][j+1]).length();
            }
        }
    }
    double belowPerimeter{0};
    for(int i=0;i<newFacesBelow.size();i++)
    {
        for(int j=0;j<newFacesBelow[i].size();j++)
        {
            if(j==newFacesBelow[i].size()-1)
            {
                belowPerimeter += (newFacesBelow[i][j]-newFacesBelow[i][0]).length();
            }
            else
            {
                belowPerimeter += (newFacesBelow[i][j]-newFacesBelow[i][j+1]).length();
            }
        }
    }
    qDebug() << "abovePerimeter " << abovePerimeter;
    qDebug() << "belowPerimeter " << belowPerimeter;

    if(abovePerimeter>belowPerimeter)
    {
        faces = newFacesAbove;
        colorOfFaces = newColorOfFacesAbove;
    }
    else if(abovePerimeter<belowPerimeter)
    {
        faces = newFacesBelow;
        colorOfFaces = newColorOfFacesBelow;
    }
}

void Block::keep_smallest_block()
{
    qDebug() << "keep_smallest_block ";
    double abovePerimeter{0};
    for(int i=0;i<newFacesAbove.size();i++)
    {
        for(int j=0;j<newFacesAbove[i].size();j++)
        {
            qDebug() << "newFacesAbove " << newFacesAbove[i][j].x() << " " << newFacesAbove[i][j].y() << " " << newFacesAbove[i][j].z() << " ";
            if(j==newFacesAbove[i].size()-1)
            {
                abovePerimeter += (newFacesAbove[i][j]-newFacesAbove[i][0]).length();
            }
            else
            {
                abovePerimeter += (newFacesAbove[i][j]-newFacesAbove[i][j+1]).length();
            }
        }
    }
    double belowPerimeter{0};
    for(int i=0;i<newFacesBelow.size();i++)
    {
        for(int j=0;j<newFacesBelow[i].size();j++)
        {
            if(j==newFacesBelow[i].size()-1)
            {
                belowPerimeter += (newFacesBelow[i][j]-newFacesBelow[i][0]).length();
            }
            else
            {
                belowPerimeter += (newFacesBelow[i][j]-newFacesBelow[i][j+1]).length();
            }
        }
    }
    qDebug() << "abovePerimeter " << abovePerimeter;
    qDebug() << "belowPerimeter " << belowPerimeter;

    if(abovePerimeter<belowPerimeter)
    {
        faces = newFacesAbove;
        colorOfFaces = newColorOfFacesAbove;
    }
    else if(abovePerimeter>belowPerimeter)
    {
        faces = newFacesBelow;
        colorOfFaces = newColorOfFacesBelow;
    }
}

void Block::keep_both_blocks()
{
    qDebug() << "keep_both_blocks ";
    faces.clear();
    colorOfFaces.clear();
    for(int i=0;i<newFacesAbove.size();i++)
    {
        faces.push_back(newFacesAbove[i]);
        colorOfFaces.push_back(newColorOfFacesAbove[i]);
    }
    for(int i=0;i<newFacesBelow.size();i++)
    {
        faces.push_back(newFacesBelow[i]);
        colorOfFaces.push_back(newColorOfFacesBelow[i]);
    }
}

void Block::add_new_knife_plane_to_block()
{
    qDebug() << "add_new_knife_plane_to_block ";
    round_intersections();
    sort( intersections.begin(), intersections.end() );
    intersections.erase( unique( intersections.begin(), intersections.end() ), intersections.end() );
    put_intersections_in_order();
    faces.push_back(intersections);
    colorOfFaces.push_back(paintBrushColor);
    qDebug() << "add_new_knife_plane_to_block end";
}

void Block::round_intersections()
{
    qDebug() << "add_new_knife_plane_to_block round_intersections";
    for(int i=0;i<intersections.size();i++)
    {
        intersections[i].x() = roundf(intersections[i].x() * 100) / 100;
        intersections[i].y() = roundf(intersections[i].y() * 100) / 100;
        intersections[i].z() = roundf(intersections[i].z() * 100) / 100;
    }
}

void Block::put_intersections_in_order()
{
    qDebug() << "add_new_knife_plane_to_block put_intersections_in_order";
    if(intersections.size()>8)
    {

        osg::Vec3d averageIntersection = osg::Vec3d(0,0,0);
        for(int i=0;i<intersections.size();i++)
        {
            averageIntersection.x() += intersections[i].x();
            averageIntersection.y() += intersections[i].y();
            averageIntersection.z() += intersections[i].z();
        }
        averageIntersection.x() = averageIntersection.x()/intersections.size();
        averageIntersection.y() = averageIntersection.y()/intersections.size();
        averageIntersection.z() = averageIntersection.z()/intersections.size();


        find_normal_vector_to_face(intersections);
        std::vector<osg::Vec3d> newIntersections;
        newIntersections.clear();
        newIntersections.push_back(intersections.back());
        intersections.pop_back();
        put_intersections_in_order_R(intersections,newIntersections,averageIntersection);
        intersections = newIntersections;
    }
    else
    {
        allIntersectionsCombinations.clear();
        std::vector<osg::Vec3d> unavailablePoints;
        unavailablePoints.push_back(intersections[0]);
        intersections.erase(intersections.begin());
        put_intersections_in_order_R(intersections,unavailablePoints);
        double minPerimeter = 100000;
        double minPerimeterIndex = 0;
        for(int i=0;i<allIntersectionsCombinations.size();i++)
        {
            double perimeter = 0;
            for(int j=0;j<allIntersectionsCombinations[i].size();j++)
            {
                if(j==allIntersectionsCombinations[i].size()-1)
                {
                    perimeter += (allIntersectionsCombinations[i][j]-allIntersectionsCombinations[i][0]).length();
                }
                else
                {
                    perimeter += (allIntersectionsCombinations[i][j]-allIntersectionsCombinations[i][j+1]).length();
                }
            }
            if(perimeter<minPerimeter)
            {
                minPerimeter = perimeter;
                minPerimeterIndex = i;
            }
        }
        intersections = allIntersectionsCombinations[minPerimeterIndex];
    }

}

void Block::put_intersections_in_order_R(std::vector<osg::Vec3d> availablePoints,std::vector<osg::Vec3d> unavailablePoints)
{
    if(availablePoints.size()==0)
    {
        allIntersectionsCombinations.push_back(unavailablePoints);
        return;
    }
    for(int i=0;i<availablePoints.size();i++)
    {
        std::vector<osg::Vec3d> newAvailablePoints = availablePoints;
        std::vector<osg::Vec3d> newUnavailablePoints = unavailablePoints;
        newUnavailablePoints.push_back(availablePoints[i]);
        newAvailablePoints.erase(newAvailablePoints.begin() + i);
        put_intersections_in_order_R(newAvailablePoints,newUnavailablePoints);
    }

}

void Block::put_intersections_in_order_R(std::vector<osg::Vec3d> &availablePoints,std::vector<osg::Vec3d> &unavailablePoints,osg::Vec3d &averageIntersection)
{
    double minAngle{99999};
    int minAngleIndex{0};

    if(availablePoints.size()==0)
    {
        return;
    }

    for(int i=0;i<availablePoints.size();i++)
    {
        double m1,m2;
        double costheta;
        double dotProduct;
        double angle{0};
        osg::Vec3d p1 = unavailablePoints.back() - averageIntersection;
        osg::Vec3d p2 = availablePoints[i] - averageIntersection;


        osg::Vec3d p90 = n^p1;
        p90.normalize();

        m1 = sqrt(p1.x()*p1.x() + p1.y()*p1.y() + p1.z()*p1.z());
        m2 = sqrt(p2.x()*p2.x() + p2.y()*p2.y() + p2.z()*p2.z());
        dotProduct = p1.x()*p2.x() + p1.y()*p2.y() + p1.z()*p2.z();
        costheta = dotProduct / (m1*m2);

        if(costheta>1)
        {
            costheta = 1;
            qDebug() << "error costheta>1 " <<costheta;
        }
        if(costheta<-1)
        {
            costheta = -1;
            qDebug() << "error costheta<-1 " <<costheta;
        }

        angle = acos(costheta);

        if(p90*p2 < 0)
        {
            angle += 3.14159;
        }

        if(angle < minAngle)
        {
            minAngle = angle;
            minAngleIndex = i;
        }

    }

    unavailablePoints.push_back(availablePoints[minAngleIndex]);
    availablePoints.erase(availablePoints.begin()+minAngleIndex);

    put_intersections_in_order_R(availablePoints, unavailablePoints, averageIntersection);
}

void Block::set_vertex_array()
{
    qDebug() << "set_vertex_array ";

    vertexArray.clear();
    for(int i=0;i<faces.size();i++)
    {
        if(faces[i].size()>0)
        {
            osg::Vec3Array* v = new osg::Vec3Array;
            v->resize(faces[i].size());
            for(int j=0;j<faces[i].size();j++)
            {
                //qDebug() << "vertexArray i="<< i << faces[i][j].x() << " " << faces[i][j].y() << " " << faces[i][j].z() << " ";

                (*v)[j].set(faces[i][j].x(),faces[i][j].y(),faces[i][j].z());
            }
            vertexArray.push_back(v);
        }
    }
}

void Block::set_vertex_color_array()
{
    qDebug() << "set_vertex_color_array ";

    vertexColorArray.clear();
    for(int i=0;i<colorOfFaces.size();i++)
    {
        osg::Vec4Array* v = new osg::Vec4Array;
        v->push_back(colorOfFaces[i]);
        vertexColorArray.push_back(v);
    }
}

void Block::undo_slice()
{
    if(oldColorOfFaces.size()>0)
    {
        while(oldFaces.size() < oldColorOfFaces.size())
        {
            oldColorOfFaces.pop_back();
        }
        colorOfFaces = oldColorOfFaces.back();
        oldColorOfFaces.pop_back();
        set_vertex_color_array();
    }
    if(oldFaces.size()>0)
    {
        faces = oldFaces.back();
        oldFaces.pop_back();
        set_vertex_array();
    }
}

void Block::paint_face(float begX, float begY, float begZ,float endX, float endY, float endZ)
{
    qDebug() << "paint_face ";
    find_intersections_between_paint_brush_and_block(begX,begY,begZ,endX,endY,endZ);
    int faceToPaint = find_face_closest_to_user(begX, begY, begZ);
    if(faceToPaint != -1)
    {
        oldColorOfFaces.push_back(colorOfFaces);
        colorOfFaces[faceToPaint] =  paintBrushColor;
        set_vertex_color_array();
    }
}

void Block::remove_face(float begX, float begY, float begZ,float endX, float endY, float endZ)
{
    qDebug() << "remove_face ";
    find_intersections_between_paint_brush_and_block(begX,begY,begZ,endX,endY,endZ);
    int faceToRemove = find_face_closest_to_user(begX, begY, begZ);
    if(faceToRemove != -1)
    {
        oldColorOfFaces.push_back(colorOfFaces);
        colorOfFaces.erase(colorOfFaces.begin()+faceToRemove);
        set_vertex_color_array();
        oldFaces.push_back(faces);
        faces.erase(faces.begin()+faceToRemove);
        set_vertex_array();
    }
}

void Block::find_normal_vector_to_face(int faceIndex)
{
    if(faces[faceIndex].size()>1){
        qDebug() << "find_normal_vector_to_face ";
        osg::Vec3d averageIntersection = osg::Vec3d(0,0,0);
        for(int i=0;i<faces[faceIndex].size();i++)
        {
            averageIntersection.x() += faces[faceIndex][i].x();
            averageIntersection.y() += faces[faceIndex][i].y();
            averageIntersection.z() += faces[faceIndex][i].z();
        }
        averageIntersection.x() = averageIntersection.x()/faces[faceIndex].size();
        averageIntersection.y() = averageIntersection.y()/faces[faceIndex].size();
        averageIntersection.z() = averageIntersection.z()/faces[faceIndex].size();
        n = (faces[faceIndex][1]-averageIntersection)^(faces[faceIndex][0]-averageIntersection);
        n.normalize();
    }
}
void Block::find_normal_vector_to_face(std::vector<osg::Vec3d> faceIndexes)
{
    if(faceIndexes.size()>1){
        qDebug() << "find_normal_vector_to_face ";
        osg::Vec3d averageIntersection = osg::Vec3d(0,0,0);
        for(int i=0;i<faceIndexes.size();i++)
        {
            averageIntersection.x() += faceIndexes[i].x();
            averageIntersection.y() += faceIndexes[i].y();
            averageIntersection.z() += faceIndexes[i].z();
        }
        averageIntersection.x() = averageIntersection.x()/faceIndexes.size();
        averageIntersection.y() = averageIntersection.y()/faceIndexes.size();
        averageIntersection.z() = averageIntersection.z()/faceIndexes.size();
        n = (faceIndexes[0]-averageIntersection)^(faceIndexes[1]-averageIntersection);
        n.normalize();
    }
}
void Block::find_intersections_between_paint_brush_and_block(float begX, float begY, float begZ,float endX, float endY, float endZ)
{
    qDebug() << "find_intersections_between_paint_brush_and_block ";
    intersections.clear();
    for(int i=0;i<faces.size();i++)
    {
        find_intersections_between_face_and_paint_brush(i,begX,begY,begZ,endX,endY,endZ);
    }
}
void Block::find_intersections_between_face_and_paint_brush(int faceIndex,float begX, float begY, float begZ,float endX, float endY, float endZ)
{
    osg::Vec3d intersect = osg::Vec3d(0,0,0);
    if(faces[faceIndex].size()>2){
        find_normal_vector_to_face(faceIndex);
        qDebug() << "find_intersections_between_face_and_paint_brush ";
        osg::Vec3d p1 = osg::Vec3d(begX,begY,begZ);
        osg::Vec3d p2 = osg::Vec3d(endX,endY,endZ);
        osg::Vec3d p3 = faces[faceIndex][0];

        double u = (n.x()*(p3.x()-p2.x()) + n.y()*(p3.y()-p2.y()) + n.z()*(p3.z()-p2.z())) / (n.x()*(p1.x()-p2.x()) + n.y()*(p1.y()-p2.y()) + n.z()*(p1.z()-p2.z()));
        if (u > 0.0 && u < 1.0)
        {
            intersect.x() = u * p1.x() + (1-u) * p2.x();
            intersect.y() = u * p1.y() + (1-u) * p2.y();
            intersect.z() = u * p1.z() + (1-u) * p2.z();
            qDebug() << "intersect " << intersect.x() << " " << intersect.y() << " " << intersect.z() << " ";
        }


       double m1,m2;
       double anglesum=0,costheta;
       bool onANode{false};

       for (int i=0;i<faces[faceIndex].size();i++) {

          p1.x() = faces[faceIndex][i].x() - intersect.x();
          p1.y() = faces[faceIndex][i].y() - intersect.y();
          p1.z() = faces[faceIndex][i].z() - intersect.z();
          p2.x() = faces[faceIndex][(i+1)%faces[faceIndex].size()].x() - intersect.x();
          p2.y() = faces[faceIndex][(i+1)%faces[faceIndex].size()].y() - intersect.y();
          p2.z() = faces[faceIndex][(i+1)%faces[faceIndex].size()].z() - intersect.z();

          m1 = sqrt(p1.x()*p1.x() + p1.y()*p1.y() + p1.z()*p1.z());
          m2 = sqrt(p2.x()*p2.x() + p2.y()*p2.y() + p2.z()*p2.z());
          if (m1*m2 <= 0.0000001)
              onANode = true;
          else
             costheta = (p1.x()*p2.x() + p1.y()*p2.y() + p1.z()*p2.z()) / (m1*m2);

          anglesum += acos(costheta);
       }
       qDebug() << "anglesum " << anglesum << " ";
       if(!(anglesum>6.283 || onANode))
       {
            intersect = osg::Vec3d(0,0,0);
       }

    }
    intersections.push_back(intersect);
}

int Block::find_face_closest_to_user(float begX, float begY, float begZ)
{
    int faceIndex{-1};
    double minDistance{90000};
    for(int i=0;i<intersections.size();i++)
    {
        if(!(intersections[i].x()==0&&intersections[i].y()==0&&intersections[i].z()==0))
        {
            double distance = (osg::Vec3d(begX,begY,begZ)-intersections[i]).length();
            if(distance<minDistance)
            {
                minDistance = distance;
                faceIndex = i;
            }
        }
    }
    return faceIndex;
}

bool Block::undo_paint()
{
    if(oldColorOfFaces.size()>0)
    {
        if(colorOfFaces.size() == oldColorOfFaces.back().size()){
            if(oldColorOfFaces.size() > oldFaces.size()){
                colorOfFaces = oldColorOfFaces.back();
                oldColorOfFaces.pop_back();
                set_vertex_color_array();
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
}
