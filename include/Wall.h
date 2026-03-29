#pragma once
#include "raylib.h"
#include "Colliders.h"
#include <optional>

struct Wall : ColliderOwner
{
    Wall(Vector2 pos1, Vector2 pos2, float circle1Rad, float circle2Rad, bool positionCircle1InPos1, bool positionCircle2InPos2, bool hasBackline, Color c);
        // positionCircle1InPos1 : true => circle1Collider is placed in pos1, lineCollider.pos1 is positioned with respect to that
        //                        false => lineCollider.pos1 is placed in pos1, circle1Collider is positioned with respect to that
    
    LineCollider lineCollider;
    std::optional<CircleCollider> circle1Collider; // circle collider at line.pos1
    std::optional<CircleCollider> circle2Collider; // circle collider at line.pos2
    std::optional<LineCollider> backLineCollider;
    float defaultThickness = 10.0f;
    
    Color color;
    void Draw();
    Vector2 GetPos1() { return lineCollider.line.pos1; };
    Vector2 GetPos2() { return lineCollider.line.pos2; };
private:
    const float restitution = 0.3f;
};