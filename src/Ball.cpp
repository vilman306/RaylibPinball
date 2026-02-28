#include "raylib.h"
#include "Ball.h"
#include "Config.h"
#include "Vec2Extensions.h"

Ball::Ball()
{
    velocity = {0.0f, 20.0f};
    color = RED;
    position = {0.5f*Config::virtualWidth, 0.5f*Config::virtualHeight};
}

void Ball::Update(float dt)
{
    position += velocity * dt;
}

void Ball::Draw()
{
    DrawCircle(position.x, position.y, 20.0f, color);
}