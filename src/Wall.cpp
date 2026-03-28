#include "Wall.h"
#include "Colliders.h"
#include "Utils.h"

Wall::Wall(Vector2 pos1, Vector2 pos2, float circle1Rad, float circle2Rad, bool positionCircle1InPos1, bool positionCircle2InPos2, bool hasBackline, Color c) : lineCollider(pos1, pos2, this), color(c)
{
    material.restitution = restitution;

    Vector2 normal = lineCollider.line.normal;

    float t1 = defaultThickness, t2 = defaultThickness;

    if (circle1Rad > 0.0f) {
        if (positionCircle1InPos1) {
            circle1Collider = CircleCollider(pos1, circle1Rad, this);
            circle1Collider->owner = this;
            lineCollider.UpdatePosition(pos1 + normal * circle1Rad, lineCollider.line.pos2);
        }
        else {
            circle1Collider = CircleCollider(pos1 - normal * circle1Rad, circle1Rad, this);
            circle1Collider->owner = this;
        }
        t2 = circle1Rad;
        t1 = circle1Rad;
    }
    if (circle2Rad > 0.0f) {
        if (positionCircle2InPos2) {
            circle2Collider = CircleCollider(pos2, circle2Rad, this);
            circle2Collider->owner = this;
            lineCollider.UpdatePosition(lineCollider.line.pos1, pos2 + normal * circle2Rad);
        }
        else {
            circle2Collider = CircleCollider(pos2 - normal * circle2Rad, circle2Rad, this);
            circle2Collider->owner = this;
        }
        t1 = circle2Rad;
        if (t2 == defaultThickness)
            t2 = circle2Rad;
    }
    normal = lineCollider.line.normal;
    pos1 = lineCollider.line.pos1;
    pos2 = lineCollider.line.pos2;
    if (hasBackline) {
        backLineCollider = LineCollider(pos2 - 2 * t1 * normal, pos1 - 2 * t2 * normal, this);
        backLineCollider->owner = this;
    }
}

void Wall::Draw()
{
    if (circle1Collider)
        DrawCircleV(Utils::WorldToScreen(circle1Collider->circle.position), circle1Collider->circle.radius, color);
    if (circle2Collider)
        DrawCircleV(Utils::WorldToScreen(circle2Collider->circle.position), circle2Collider->circle.radius, color);
    
    Vector2 p1 = lineCollider.line.pos1;
    Vector2 p4 = lineCollider.line.pos2;
    Vector2 p2, p3;
    if (backLineCollider)
    {
        p2 = backLineCollider->line.pos2;
        p3 = backLineCollider->line.pos1;
    }
    else
    {
        if (circle1Collider)
            p2 = lineCollider.line.pos1 - 2.0f * lineCollider.line.normal * circle1Collider->circle.radius;
        else
            p2 = lineCollider.line.pos1 - lineCollider.line.normal * defaultThickness;
        if (circle2Collider)
            p3 = lineCollider.line.pos2 - 2.0f * lineCollider.line.normal * circle2Collider->circle.radius;
        else
            p3 = lineCollider.line.pos2 - lineCollider.line.normal * defaultThickness;
    }
    Vector2 points[4] = {Utils::WorldToScreen(p1), Utils::WorldToScreen(p2), Utils::WorldToScreen(p3), Utils::WorldToScreen(p4)};
    DrawTriangleFan(points, 4, color);
}