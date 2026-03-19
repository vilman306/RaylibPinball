#include "raylib.h"
#include "Line.h"
#include "raymath.h"

void Line::Draw()
{
    int visualThickness = 2.0f;
    Vector2 dir = Vector2Normalize(pos2 - pos1);
    Vector2 normal = {-dir.y, dir.x};
    Vector2 offset = visualThickness * normal;
    Vector2 p1 = pos1 + offset;
    Vector2 p2 = pos2 + offset;
    Vector2 p3 = pos2 - offset;
    Vector2 p4 = pos1 - offset;
    Vector2 points[4] = {p1, p2, p3, p4};
    DrawTriangleFan(points, 4, color);
};