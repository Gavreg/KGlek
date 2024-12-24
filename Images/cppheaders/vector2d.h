#ifndef VECTOR2DH
#define VECTOR2DH

#include <iostream>

struct vector2d
{
    double x = 0;
    double y = 0;


    vector2d(double _x = 0, double _y = 0) : x{_x}, y{_y} {};

    vector2d operator+(const vector2d &v) const
    {
        return {x + v.x, y + v.y};
    }
    vector2d operator*(double k) const
    {
        return {x * k, y * k};
    }
    vector2d(const vector2d &v) 
    {
        x = v.x;
        y = v.y;
    }
};

vector2d operator*(const double k, const vector2d &v)
{
    return {v.x * k, v.y * k};
}

std::ostream &operator<<(std::ostream &fout, const vector2d &v)
{
    return fout << v.x << " " << v.y;
}


#endif