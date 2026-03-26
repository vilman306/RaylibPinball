#pragma once
#include "raylib.h"
#include "Vec2Extensions.h"
#include "raymath.h"

struct Circle
{
    Circle() = default;
    Circle(Vector2 pos, float rad, Color c);
    void Draw(); // Should only be used for testing
    Color color;
    Vector2 position;
    float radius;
    void *owner = nullptr;
    enum class CircleRole { Generic, FlipperRot, FlipperTip };
    CircleRole role = CircleRole::Generic;
};

struct Line
{
    Line() = default;
    Line(Vector2 pos1, Vector2 pos2, Color color);
    void UpdatePosition(Vector2 p1, Vector2 p2);
    Vector2 pos1;
    Vector2 pos2;
    Color color;
    Vector2 normal; // normal.y > 0 if pos1.x < pos2.x
    float visualThickness = 2.0f;
    void Draw(); // Should only be used for testing!
    void *owner = nullptr;
    enum class LineRole { Generic, FlipperUp, FlipperDown };
    LineRole role = LineRole::Generic;
};