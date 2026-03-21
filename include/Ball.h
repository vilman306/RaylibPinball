#pragma once
#include "raylib.h"
struct Ball
{
    // Should have a circle
    Ball();
    void Draw();
    void UpdatePhysicalPosition(Vector2 newPos);
    const float radius = 20.0f;
    Vector2 velocity;
    Vector2 visualPosition; // The ball's visual position. This is lerped between prevPhysicalPosition and physicalPosition.
    Vector2 physicalPosition; // Target position that "position" is lerping to. This is the ball's physical position.
    Vector2 prevPhysicalPosition; // Previous position that "position" was lerping to
    Color color;
};