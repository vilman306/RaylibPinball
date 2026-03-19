#pragma once
#include "raylib.h"
#include "Vec2Extensions.h"
#include "raymath.h"

class Line
{
public:
    Vector2 pos1;
    Vector2 pos2;
    Color color;
    float visualThickness = 2.0f;
    void Draw();
    void GetNormal();
};