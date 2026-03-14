#include "raylib.h"
#include "Ball.h"
#include "Config.h"
#include "Vec2Extensions.h"

Ball::Ball()
{
    velocity = {0.0f, 0.0f};
    color = RED;
    visualPosition = {0.5f*Config::gameWidth, 0.5f*Config::gameHeight};
    physicalPosition = visualPosition;
    prevPhysicalPosition = physicalPosition;
}

void Ball::Draw()
{
    DrawCircle(visualPosition.x, visualPosition.y, radius, color);
}

void Ball::UpdatePhysicalPosition(Vector2 newPos)
{
    prevPhysicalPosition = physicalPosition;
    physicalPosition = newPos;
}