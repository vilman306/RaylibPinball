#pragma once
#include "raylib.h"
#include "Colliders.h"
#include <optional>

struct Wall
{
    Wall(bool hasCircle1, bool hasCircle2, Color c);
    LineCollider line;
    std::optional<CircleCollider> circle1; // circle collider at line.pos1
    std::optional<CircleCollider> circle2; // circle collider at line.pos2

    Color color;
    void Draw();
};