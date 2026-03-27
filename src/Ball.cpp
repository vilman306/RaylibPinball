#include "raylib.h"
#include "Ball.h"
#include "Config.h"
#include "Vec2Extensions.h"
#include "Utils.h"

Ball::Ball(Vector2 pos, float rad, Vector2 vel, Color c)
    : circle(pos, rad), velocity(vel)
{
    velocity = {0.0f, 0.0f};
    visualPosition = circle.position;
    prevPhysicalPosition = circle.position;
    color = c;
}

void Ball::Draw()
{
    Vector2 screenPos = Utils::WorldToScreen(visualPosition);
    DrawCircleV(screenPos, circle.radius, color);
}

