#include "raylib.h"
#include "Ball.h"
#include "Config.h"
#include "Vec2Extensions.h"

Ball::Ball(Vector2 pos, float rad, Vector2 vel, Color c)
    : circle(pos, rad, c),
      velocity(vel)
{
    velocity = {0.0f, 0.0f};
    physicalPosition = circle.position;
    prevPhysicalPosition = physicalPosition;
}

void Ball::Draw()
{
    circle.Draw();
}

