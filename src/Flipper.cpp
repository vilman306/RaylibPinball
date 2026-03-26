#include "Flipper.h"

Flipper::Flipper(Vector2 rotP, float len, Color c, int dir)
    : rotPos(rotP),
      length(len),
      color(c),
      direction(dir),
      circleRot(rotP, rotRadius, c),
      circleTip({0.0f, 0.0f}, tipRadius, c),
      lineUp(),
      lineDown()
{

    UpdateCircleTipPosition(circleTip, physicalAngle);
    UpdateLineUpPosition(lineUp, physicalAngle);
    UpdateLineDownPosition(lineDown, physicalAngle);

    // Vector2 normal = {-direction * lineDir.y, direction * lineDir.x};
    // if (direction == 1)
    // { // set line points in specific order so line gets wanted normal
    //     lineUp = Line(rotPos + normal * rotRadius, tipPos + normal * tipRadius, c);
    //     lineDown = Line(tipPos - normal * tipRadius, rotPos - normal * rotRadius, c);
    // }
    // else
    // {
    //     lineUp = Line(tipPos + normal * tipRadius, rotPos + normal * rotRadius, c);
    //     lineDown = Line(rotPos - normal * rotRadius, tipPos - normal * tipRadius, c);
    // }

    circleRot.owner = this;
    circleTip.owner = this;
    circleRot.role = Circle::CircleRole::FlipperRot;
    circleTip.role = Circle::CircleRole::FlipperTip;
    lineUp.owner = this;
    lineDown.owner = this;
    lineUp.role = Line::LineRole::FlipperUp;
    lineDown.role = Line::LineRole::FlipperDown;
}

void Flipper::UpdateCircleTipPosition(Circle &circle, float angle)
{
    Vector2 lineDir = {direction * cosf(angle), sinf(angle)};
    tipPos = rotPos + lineDir * length;
    circle.position = tipPos;
}

void Flipper::UpdateLineUpPosition(Line &line, float angle)
{
    Vector2 lineDir = {direction * cosf(angle), sinf(angle)};
    Vector2 normal = {-direction * lineDir.y, direction * lineDir.x};
    if (direction == 1)
    { // set line points in specific order so line gets wanted normal direction
        line.UpdatePosition(rotPos + normal * rotRadius, tipPos + normal * tipRadius);
    }
    else
    {

        line.UpdatePosition(tipPos + normal * tipRadius, rotPos + normal * rotRadius);
    }
}

void Flipper::UpdateLineDownPosition(Line &line, float angle)
{
    Vector2 lineDir = {direction * cosf(angle), sinf(angle)};
    Vector2 normal = {-direction * lineDir.y, direction * lineDir.x};
    if (direction == 1)
    { // set line points in specific order so line gets wanted normal direction
        line.UpdatePosition(tipPos - normal * tipRadius, rotPos - normal * rotRadius);
    }
    else
    {
        line.UpdatePosition(rotPos - normal * rotRadius, tipPos - normal * tipRadius);
    }
}

void Flipper::Draw()
{
    Circle visualCircleTip({0.0f, 0.0f}, tipRadius, color);
    Line visualLineUp({0.0f, 0.0f}, {0.0f, 0.0f}, color);
    Line visualLineDown({0.0f, 0.0f}, {0.0f, 0.0f}, color);
    UpdateCircleTipPosition(visualCircleTip, visualAngle);
    UpdateLineUpPosition(visualLineUp, visualAngle);
    UpdateLineDownPosition(visualLineDown, visualAngle);
    visualCircleTip.Draw();
    visualLineUp.Draw();
    visualLineDown.Draw();
    circleRot.Draw();
}


void Flipper::UpdatePhysics(float dtPhysics)
{
    prevPhysicalAngle = physicalAngle;
    rotDir = rotateUp ? 1 : -1;
    physicalAngle = Clamp(physicalAngle + angularSpeed * rotDir * dtPhysics, minAngle, maxAngle);
    UpdateCircleTipPosition(circleTip, physicalAngle);
    UpdateLineUpPosition(lineUp, physicalAngle);
    UpdateLineDownPosition(lineDown, physicalAngle);
}
