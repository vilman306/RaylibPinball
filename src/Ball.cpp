#include "raylib.h"
#include "Ball.h"
#include "Config.h"
#include "Vec2Extensions.h"
#include "Utils.h"

Ball::Ball(Vector2 pos, float rad, Vector2 vel, Color c)
    : circleCollider(pos, rad), velocity(vel)
{
    visualPosition = circleCollider.circle.position;
    prevPhysicalPosition = circleCollider.circle.position;
    color = c;
}

void Ball::Draw()
{
    Vector2 screenPos = Utils::WorldToScreen(visualPosition);
    DrawCircleV(screenPos, circleCollider.circle.radius, color);
}

