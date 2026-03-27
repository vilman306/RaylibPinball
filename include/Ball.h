#pragma once
#include "raylib.h"
#include "Colliders.h"

struct Ball
{
public:
    Ball(Vector2 pos, float rad, Vector2 vel, Color c);
    void Draw();

    Vector2 velocity;
    Circle circle; // Collider
    
    Color color;
    Vector2 visualPosition;
    Vector2 physicalPosition; // Target position that "circle.position" is lerping to. This is the ball's physical position.
    Vector2 prevPhysicalPosition; // Previous position that "circle.position" was lerping to
};