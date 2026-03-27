#pragma once
#include "raylib.h"
#include "Vec2Extensions.h"
#include "raymath.h"

struct CircleCollider
{
    CircleCollider(Vector2 pos, float rad);
    Vector2 position;
    float radius;
    void* owner = nullptr;
    enum class CircleRole { Generic, FlipperRot, FlipperTip };
    CircleRole role = CircleRole::Generic;

    // To draw collider (for debugging)
    void Draw();
    Color color = BLACK;
};

struct LineCollider
{
    LineCollider(Vector2 pos1, Vector2 pos2);
    void UpdatePosition(Vector2 p1, Vector2 p2);
    Vector2 pos1;
    Vector2 pos2;
    Vector2 normal; // normal.y > 0 if pos1.x < pos2.x
    void* owner = nullptr;
    enum class LineRole { Generic, FlipperUp, FlipperDown };
    LineRole role = LineRole::Generic;

    // To draw collider (for debugging)
    float visualThickness = 2.0f;
    Color color = BLACK;
    void Draw();
};