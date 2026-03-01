#pragma once
#include "raylib.h"
class Ball
{
public:
    Ball();
    void Update(float dt);
    void Draw();
    const float radius = 20.0f;
    Vector2 velocity;
    Vector2 position;
private:
    Color color;
};