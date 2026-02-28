#pragma once
#include "raylib.h"
class Ball
{
public:
    Ball();
    void Update(float dt);
    void Draw();
private:
    Vector2 position;
    Vector2 velocity;
    Color color;
};