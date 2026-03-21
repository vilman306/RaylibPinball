#pragma once
#include "raylib.h"
#include "Vec2Extensions.h"
#include "raymath.h"

struct Line
{
    Line() = default;
    Line(Vector2 pos1, Vector2 pos2, Color color);
    Vector2 pos1;
    Vector2 pos2;
    Color color;
    Vector2 normal;
    float visualThickness = 1.0f;
    void Draw();
};