#include "Flipper.h"
#include "Config.h"
#include "Utils.h"

Flipper::Flipper(Vector2 rotP, float len, Color c, int dir)
    : rotPos(rotP),
      length(len),
      color(c),
      direction(dir),
      circleRot(rotP, rotRadius),
      circleTip({0.0f, 0.0f}, tipRadius),
      lineUp({0.0f, 0.0f}, {0.0f, 0.0f}),
      lineDown({0.0f, 0.0f}, {0.0f, 0.0f})
{

    UpdateCircleTipPosition(circleTip, physicalAngle);
    UpdateLineUpPosition(lineUp, physicalAngle);
    UpdateLineDownPosition(lineDown, physicalAngle);

    // Vector2 normal = {-direction * lineDir.y, direction * lineDir.x};
    // if (direction == 1)
    // { // set line points in specific order so line gets wanted normal
    //     lineUp = LineCollider(rotPos + normal * rotRadius, tipPos + normal * tipRadius, c);
    //     lineDown = LineCollider(tipPos - normal * tipRadius, rotPos - normal * rotRadius, c);
    // }
    // else
    // {
    //     lineUp = LineCollider(tipPos + normal * tipRadius, rotPos + normal * rotRadius, c);
    //     lineDown = LineCollider(rotPos - normal * rotRadius, tipPos - normal * tipRadius, c);
    // }

    circleRot.owner = this;
    circleTip.owner = this;
    circleRot.role = CircleCollider::CircleRole::FlipperRot;
    circleTip.role = CircleCollider::CircleRole::FlipperTip;
    lineUp.owner = this;
    lineDown.owner = this;
    lineUp.role = LineCollider::LineRole::FlipperUp;
    lineDown.role = LineCollider::LineRole::FlipperDown;
}

void Flipper::UpdateCircleTipPosition(CircleCollider &circle, float angle)
{
    Vector2 lineDir = {direction * cosf(angle), sinf(angle)};
    tipPos = rotPos + lineDir * length;
    circle.position = tipPos;
}

void Flipper::UpdateLineUpPosition(LineCollider &line, float angle)
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

void Flipper::UpdateLineDownPosition(LineCollider &line, float angle)
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
    DrawCircleV(Utils::WorldToScreen(circleRot.position), circleRot.radius, color);

    CircleCollider visualCircleTip({0.0f, 0.0f}, tipRadius);
    UpdateCircleTipPosition(visualCircleTip, visualAngle);
    DrawCircleV(Utils::WorldToScreen(visualCircleTip.position), tipRadius, color);

    LineCollider visualLineUp({0.0f, 0.0f}, {0.0f, 0.0f});
    LineCollider visualLineDown({0.0f, 0.0f}, {0.0f, 0.0f});
    UpdateLineUpPosition(visualLineUp, visualAngle);
    UpdateLineDownPosition(visualLineDown, visualAngle);
    Vector2 p1 = Utils::WorldToScreen(visualLineUp.pos1);
    Vector2 p2 = Utils::WorldToScreen(visualLineDown.pos2);
    Vector2 p3 = Utils::WorldToScreen(visualLineDown.pos1);
    Vector2 p4 = Utils::WorldToScreen(visualLineUp.pos2);
    Vector2 points[4] = {p1, p2, p3, p4};
    DrawTriangleFan(points, 4, color);

    // float padding = 200.0f;
    // Vector2 leftUp = Utils::WorldToScreen({padding, Config::gameHeight - padding});
    // Vector2 leftDown = Utils::WorldToScreen({padding, padding});
    // Vector2 RightDown = Utils::WorldToScreen({Config::gameWidth / 2.0f - padding, padding});
    // Vector2 RightUp = Utils::WorldToScreen({Config::gameWidth / 2.0f - padding, Config::gameHeight - padding});
    // Rectangle testRec = {leftUp.x, leftUp.y, abs(RightDown.x - leftUp.x), abs(RightDown.y - leftUp.y)};
    // DrawRectanglePro(testRec, {0.0f, 0.0f}, 2.03f, BROWN);
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
