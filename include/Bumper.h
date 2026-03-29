#pragma once
#include "raylib.h"
#include "Colliders.h"

struct Bumper : ColliderOwner {
    Bumper(Vector2 pos, float rad, Color c);
    void Draw();

    CircleCollider circleCollider; // Collider

    Color color;
    float impulse = 700.0f;
};