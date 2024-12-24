
#ifndef VECTOR3D
#define VECTOR3D

#include<iostream>
#include<cmath>

struct vector3d
{
    double x=0;
    double y=0;
    double z=0;

    vector3d(double _x=0, double _y=0, double _z=0): x{_x}, y{_y}, z{_z} {};

    vector3d operator+(const vector3d &v)
    {
        return vector3d{x+v.x,y+v.y,z+v.z};
    }
    vector3d operator*(double k)
    {
        return {x*k,y*k, z*k};
    }

    vector3d (const vector3d  &v)
    {
        x=v.x;
        y=v.y;
        z=v.z;
    }

    double len()
    {
        return sqrt(x*x+y*y+z*z);
    }

    double len_sqr()
    {
        return x*x+y*y+z*z;
    }

};

vector3d operator*(const double k, const vector3d&v)
{
    return {v.x*k, v.y*k, v.z*k};
}

std::ostream & operator<< (std::ostream &fout, const vector3d &v)
{
    return fout<<v.x<<" "<<v.y<<" "<<v.z;
}


#endif