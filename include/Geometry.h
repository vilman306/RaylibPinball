#pragma once
#include "raylib.h"

struct Circle
{
    Circle(Vector2 position, float radius);
    Vector2 position;
    float radius;
};

struct Line
{
    Line(Vector2 pos1, Vector2 pos2);
    Vector2 pos1;
    Vector2 pos2;
    Vector2 normal; // normal.y > 0 if pos1.x < pos2.x
};