#pragma once
#include "raylib.h"
#include "Colliders.h"

struct Ball : ColliderOwner
{
public:
    Ball(Vector2 pos, float rad, Vector2 vel, Color c);
    void Draw();

    Vector2 velocity;
    CircleCollider circleCollider; // Collider
    
    Color color;
    Vector2 visualPosition;
    Vector2 prevPhysicalPosition; // Previous position that "circle.position" was lerping to
    Vector2 resetPosition;

    float lastNormalSpeed = 0.0f; // Used in AudioManager::HandleEvents
};