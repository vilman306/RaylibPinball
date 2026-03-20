#include "raylib.h"
#include "Line.h"
#include "raymath.h"

Line::Line(Vector2 p1, Vector2 p2, Color c) : pos1(p1), pos2(p2), color(c)
{
    Vector2 dir = Vector2Normalize(pos2 - pos1);
    normal = {dir.y, -dir.x};
}

void Line::Draw()
{
    Vector2 offset = visualThickness * normal;
    Vector2 p1 = pos1 + offset;
    Vector2 p2 = pos1 - offset;
    Vector2 p3 = pos2 - offset;
    Vector2 p4 = pos2 + offset;
    Vector2 points[4] = {p1, p2, p3, p4};
    DrawTriangleFan(points, 4, color);
};