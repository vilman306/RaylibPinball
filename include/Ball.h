#pragma once
#include "raylib.h"
#include "Geometry.h"

struct Ball
{
public:
    Ball(Vector2 pos, float rad, Vector2 vel, Color c);
    void Draw();
    // void UpdatePhysicalPosition(Vector2 newPos);
    // void UpdateVisualPosition(Vector2 newPos);
    // float GetRadius() { return circle.radius; }
    // Vector2 GetVelocity() { return velocity; }
    // void SetVelocity(Vector2 newVel) { velocity = newVel; }
    // Vector2 GetVisualPosition() { return circle.position; }
    // void SetVisualPosition(Vector2 newPos) { circle.position = newPos; }
    // Vector2 GetPhysicalPosition() { return physicalPosition; }
    // void SetPhysicalPosition(Vector2 newPos) { physicalPosition = newPos; }
    // Vector2 GetPrevPhysicalPosition() { return prevPhysicalPosition; }
    // void SetPrevPhysicalPosition(Vector2 newPos) { prevPhysicalPosition = newPos; }
    Vector2 velocity;
    Circle circle; // Contains the ball's visual position (in world coordinates!), which is lerped between prevPhysicalPosition and physicalPosition.
    Vector2 physicalPosition; // Target position that "circle.position" is lerping to. This is the ball's physical position.
    Vector2 prevPhysicalPosition; // Previous position that "circle.position" was lerping to
};