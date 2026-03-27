#include "raylib.h"
#include "Colliders.h"
#include "Utils.h"
#include <iostream>

CircleCollider::CircleCollider(Vector2 pos, float rad) : position(pos), radius(rad)
{
}

void CircleCollider::Draw() // Should only be used for debugging
{
    Vector2 screenPos = Utils::WorldToScreen(position);
    DrawCircleV(screenPos, radius, color);
}

LineCollider::LineCollider(Vector2 p1, Vector2 p2) : pos1(p1), pos2(p2)
{
    Vector2 dir = Vector2Normalize(pos2 - pos1);
    normal = {-dir.y, dir.x};
}

void LineCollider::Draw() // Should only be used for debugging
{
    Vector2 offset = visualThickness * normal;
    Vector2 p1 = Utils::WorldToScreen(pos1 + offset);
    Vector2 p2 = Utils::WorldToScreen(pos1 - offset);
    Vector2 p3 = Utils::WorldToScreen(pos2 - offset);
    Vector2 p4 = Utils::WorldToScreen(pos2 + offset);
    Vector2 points[4] = {p1, p2, p3, p4};
    DrawTriangleFan(points, 4, color);
}

void LineCollider::UpdatePosition(Vector2 p1, Vector2 p2)
{
    pos1 = p1;
    pos2 = p2;
    Vector2 dir = Vector2Normalize(pos2 - pos1);
    normal = {-dir.y, dir.x};
}