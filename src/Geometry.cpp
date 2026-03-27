#include "Geometry.h"
#include "raymath.h"
#include "Vec2Extensions.h"
#include <iostream>

Circle::Circle(Vector2 p, float r) : position(p), radius(r)
{
}

Line::Line(Vector2 p1, Vector2 p2) : pos1(p1), pos2(p2)
{
    Vector2 dir = Vector2Normalize(pos2 - pos1);
    normal = {-dir.y, dir.x};
}