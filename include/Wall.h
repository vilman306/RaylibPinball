#pragma once
#include "raylib.h"
#include "Colliders.h"
#include <optional>

struct Wall
{
    Wall(Vector2 pos1, Vector2 pos2, float circle1Rad, float circle2Rad, bool hasBackline, Color c);
    LineCollider lineCollider;
    std::optional<CircleCollider> circle1Collider; // circle collider at line.pos1
    std::optional<CircleCollider> circle2Collider; // circle collider at line.pos2
    std::optional<LineCollider> backLineCollider;
    float defaultThickness = 10.0f;

    Color color;
    void Draw();
};