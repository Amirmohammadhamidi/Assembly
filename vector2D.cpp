#include <iostream>
#include "vector2D.hpp"

vector2D::vector2D()
{
    x = 0;
    y = 0;
}

vector2D::vector2D(float x, float y)
{
    this->x = x;
    this->y = y;
}

vector2D &vector2D::add(const vector2D &vec)
{
    this->x += vec.x;
    this->y += vec.y;
    return *this;
}
// vector2D &vector2D::subtract(const vector2D &vec)
// {
// }
// vector2D &vector2D::multiply(const vector2D &vec)
// {
// }
// vector2D &vector2D::divide(const vector2D &vec)
// {
// }