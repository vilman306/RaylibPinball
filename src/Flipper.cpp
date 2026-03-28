#include "Flipper.h"
#include "Config.h"
#include "Utils.h"

Flipper::Flipper(Vector2 rotP, float len, Color c, int dir)
    : rotPos(rotP),
      length(len),
      color(c),
      direction(dir),
      circleRot(rotP, rotRadius, this),
      circleTip({0.0f, 0.0f}, tipRadius, this),
      lineUp({0.0f, 0.0f}, {0.0f, 0.0f}, this),
      lineDown({0.0f, 0.0f}, {0.0f, 0.0f}, this)
{
    material.restitution = restitution;

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
    circleRot.role = CircleCollider::CircleColliderRole::FlipperRot;
    circleTip.role = CircleCollider::CircleColliderRole::FlipperTip;
    lineUp.owner = this;
    lineDown.owner = this;
    lineUp.role = LineCollider::LineColliderRole::FlipperUp;
    lineDown.role = LineCollider::LineColliderRole::FlipperDown;
}

void Flipper::UpdateCircleTipPosition(CircleCollider &circleCollider, float angle)
{
    Vector2 lineDir = {direction * cosf(angle), sinf(angle)};
    tipPos = rotPos + lineDir * length;
    circleCollider.circle.position = tipPos;
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

    // USE CIRCLES INSTEAD OF COLLIDERS
    DrawCircleV(Utils::WorldToScreen(circleRot.circle.position), rotRadius, color);

    CircleCollider visualCircleTip({0.0f, 0.0f}, tipRadius, this);
    UpdateCircleTipPosition(visualCircleTip, visualAngle);
    DrawCircleV(Utils::WorldToScreen(visualCircleTip.circle.position), tipRadius, color);

    LineCollider visualLineUp({0.0f, 0.0f}, {0.0f, 0.0f}, this);
    LineCollider visualLineDown({0.0f, 0.0f}, {0.0f, 0.0f}, this);
    UpdateLineUpPosition(visualLineUp, visualAngle);
    UpdateLineDownPosition(visualLineDown, visualAngle);
    Vector2 p1 = Utils::WorldToScreen(visualLineUp.line.pos1);
    Vector2 p2 = Utils::WorldToScreen(visualLineDown.line.pos2);
    Vector2 p3 = Utils::WorldToScreen(visualLineDown.line.pos1);
    Vector2 p4 = Utils::WorldToScreen(visualLineUp.line.pos2);
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
