#include "Flipper.h"

Flipper::Flipper(Vector2 rotP, float len, Color c, int dir)
    : rotPos(rotP),
      length(len),
      color(c),
      direction(dir),
      circleRot(rotP, rotRadius, c),
      circleTip(),
      lineUp(),
      lineDown()
{
    Vector2 lineDir = {direction * cosf(angle), sinf(angle)};
    tipPos = rotPos + lineDir * length;

    circleTip = Circle(tipPos, tipRadius, c);

    Vector2 normal = {-direction * lineDir.y, direction * lineDir.x};
    if (direction == 1)
    { // set line points in specific order so line gets wanted normal
        lineUp = Line(rotPos + normal * rotRadius, tipPos + normal * tipRadius, c);
        lineDown = Line(tipPos - normal * tipRadius, rotPos - normal * rotRadius, c);
    }
    else
    {
        lineUp = Line(tipPos + normal * tipRadius, rotPos + normal * rotRadius, c);
        lineDown = Line(rotPos - normal * rotRadius, tipPos - normal * tipRadius, c);
    }

    circleRot.owner = this;
    circleTip.owner = this;
    lineUp.owner = this;
    lineDown.owner = this;
    lineUp.role = Line::LineRole::FlipperUp;
    lineDown.role = Line::LineRole::FlipperDown;
}

void Flipper::UpdatePositions()
{
    Vector2 lineDir = {direction * cosf(angle), sinf(angle)};
    tipPos = rotPos + lineDir * length;
    circleTip.position = tipPos;

    Vector2 normal = {-direction * lineDir.y, direction * lineDir.x};
    if (direction == 1)
    { // set line points in specific order so line gets wanted normal
        Vector2 newPosUp1 = rotPos + normal * rotRadius;
        Vector2 newPosUp2 = tipPos + normal * tipRadius;
        lineUp.UpdatePosition(newPosUp1, newPosUp2);
        Vector2 newPosDown1 = tipPos - normal * tipRadius;
        Vector2 newPosDown2 = rotPos - normal * rotRadius;
        lineDown.UpdatePosition(newPosDown1, newPosDown2);
    }
    else
    {
        Vector2 newPosUp1 = tipPos + normal * tipRadius;
        Vector2 newPosUp2 = rotPos + normal * rotRadius;
        lineUp.UpdatePosition(newPosUp1, newPosUp2);
        Vector2 newPosDown1 = rotPos - normal * rotRadius;
        Vector2 newPosDown2 = tipPos - normal * tipRadius;
        lineDown.UpdatePosition(newPosDown1, newPosDown2);
    }
}

void Flipper::UpdatePhysics(float dtPhysics)
{
    int rotDir = rotateUp ? 1 : -1;
    angle = Clamp(angle + angularSpeed * rotDir * dtPhysics, minAngle, maxAngle);
    UpdatePositions();
}
