#pragma once
#include "raylib.h"
#include "Colliders.h"

struct Flipper
{
    Flipper(Vector2 rotP, float len, Color c, int dir);
    void UpdatePhysics(float dtPhysics);
    void UpdateCircleTipPosition(Circle& circle, float angle);
    void UpdateLineUpPosition(Line& line, float angle);
    void UpdateLineDownPosition(Line& line, float angle);
    void Draw();
    Vector2 rotPos; // rotation point
    float length;
    Vector2 tipPos;
    const float rotRadius = 10.0f;
    const float tipRadius = 8.0f;
    Circle circleRot; // used in physics
    Circle circleTip; // used in physics
    Line lineUp; // used in physics
    Line lineDown; // used in physics
    Color color;

    bool rotateUp = false;
    int rotDir = 1; // 1: up, -1: down
    const float angularSpeed = 20.0f; // rad per sec
    const float minAngle = -PI / 5.0f;
    const float maxAngle = PI / 6.0f;
    float physicalAngle = minAngle;
    float prevPhysicalAngle = minAngle;
    float visualAngle = minAngle;
    const int direction;              // 1: left flipper, -1: right flipper
};