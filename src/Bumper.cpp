#include "Bumper.h"
#include "Utils.h"

Bumper::Bumper(Vector2 pos, float rad, Color c) : color(c), circleCollider(pos, rad, this)
{
}

void Bumper::Draw()
{
    DrawCircleV(Utils::WorldToScreen(circleCollider.circle.position), circleCollider.circle.radius, color);
}