#include "Box.h"
#include "Config.h"
#include "Vec2Extensions.h"

Box::Box()
{
    size = {100.0f, 50.0f};
    position = {Config::gameWidth*0.5f - size.x/2.0f, Config::gameHeight*0.9f - size.y/2.0f};
    rec = {position.x, position.y, size.x, size.y};
    rotation = 0.0f;
    rotationPoint = size / 2.0f;
    color = BLUE;
}

void Box::Draw()
{
    DrawRectanglePro(rec, {0.0f, 0.0f}, -rotation*RAD2DEG, color);
}