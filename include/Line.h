#pragma once
#include "raylib.h"
#include "Vec2Extensions.h"
#include "raymath.h"

struct Line
{
    Line(Vector2 pos1, Vector2 pos2, Color color);
    Vector2 pos1;
    Vector2 pos2;
    Color color;
    Vector2 normal;
    float visualThickness = 2.0f;
    void Draw();
};