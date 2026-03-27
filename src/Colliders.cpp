#include "raylib.h"
#include "Colliders.h"
#include "Utils.h"
#include <iostream>

CircleCollider::CircleCollider(Vector2 position, float radius) : circle(position, radius)
{
}

void CircleCollider::Draw() // Should only be used for debugging
{
    Vector2 screenPos = Utils::WorldToScreen(circle.position);
    DrawCircleV(screenPos, circle.radius, color);
}

LineCollider::LineCollider(Vector2 pos1, Vector2 pos2) : line(pos1, pos2)
{
}

void LineCollider::Draw() // Should only be used for debugging
{
    Vector2 offset = visualThickness * line.normal;
    Vector2 p1 = Utils::WorldToScreen(line.pos1 + offset);
    Vector2 p2 = Utils::WorldToScreen(line.pos1 - offset);
    Vector2 p3 = Utils::WorldToScreen(line.pos2 - offset);
    Vector2 p4 = Utils::WorldToScreen(line.pos2 + offset);
    Vector2 points[4] = {p1, p2, p3, p4};
    DrawTriangleFan(points, 4, color);
}

void LineCollider::UpdatePosition(Vector2 pos1, Vector2 pos2)
{
    line = Line(pos1, pos2);
}