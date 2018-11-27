#ifndef PHYSICSVECTOR_H
#define PHYSICSVECTOR_H

template <class Type>

class PhysicsVector {

public:

    Type x, y, z;

    PhysicsVector(Type x, Type y, Type z) :x(x), y(y), z(z) {}
    ~PhysicsVector() {}

    PhysicsVector operator+(const PhysicsVector& other)
    {
        return PhysicsVector(this->x + other.x, this->y + other.y, this->z + other.z);
    }

    PhysicsVector operator-(const PhysicsVector& other)
    {
        return PhysicsVector(this->x - other.x, this->y - other.y, this->z - other.z);
    }

    PhysicsVector operator*(double multiplyer)
    {
        return PhysicsVector(this->x*multiplyer, this->y*multiplyer, this->z*multiplyer);
    }
    PhysicsVector operator/(double divider)
    {
        return PhysicsVector(this->x/divider, this->y/divider, this->z/divider);
    }
    double operator*(const PhysicsVector& other)
    {
        return this->x*other.x+this->y*other.y+this->z*other.z;
    }
};

#endif
