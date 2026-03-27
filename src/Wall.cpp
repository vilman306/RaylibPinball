#include "Wall.h"
#include "Colliders.h"
#include "Utils.h"

Wall::Wall(Vector2 pos1, Vector2 pos2, float circle1Rad, float circle2Rad, bool hasBackline, Color c) : lineCollider(pos1, pos2)
{
    Vector2 normal = lineCollider.line.normal;
    if (circle1Rad > 0.0f)
        circle1Collider = CircleCollider(pos1 - circle1Rad * normal, circle1Rad);
    if (circle2Rad > 0.0f)
        circle2Collider = CircleCollider(pos2 - circle2Rad * normal, circle2Rad);
    if (hasBackline)
        backLineCollider = LineCollider(pos1 - 2*circle1Rad*normal, pos2 - 2*circle2Rad*normal);
}

void Wall::Draw()
{
    if (circle1Collider)
        DrawCircleV(circle1Collider->circle.position, circle1Collider->circle.radius, color);
    if (circle2Collider)
        DrawCircleV(circle2Collider->circle.position, circle2Collider->circle.radius, color);
    
    Vector2 p1 = Utils::WorldToScreen(lineCollider.line.pos1);
    Vector2 p4 = Utils::WorldToScreen(lineCollider.line.pos2);
    Vector2 p2, p3;
    if (backLineCollider) {
        p2 = Utils::WorldToScreen(backLineCollider->line.pos2);
        p3 = Utils::WorldToScreen(backLineCollider->line.pos1);
    }
    else {
        p2 = lineCollider.line.pos1 - lineCollider.line.normal * defaultThickness;
        p3 = lineCollider.line.pos2 - lineCollider.line.normal * defaultThickness;
    }
    Vector2 points[4] = {p1, p2, p3, p4};
    DrawTriangleFan(points, 4, color);
}