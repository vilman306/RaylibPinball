#pragma once
#include "raylib.h"
class Box // A rectangular, stationary collision object used to implement and test circle (ball) - rectangle (box) collision.
{
public:
    Box();
    Vector2 position; // left, upper corner
    Vector2 size; // width, height
    Rectangle rec;
    float rotation; // clockwise rotation in radians around (position + rotationPoint) 
    Vector2 rotationPoint; // relative to position
    Color color;

    void Draw();
};