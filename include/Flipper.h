#pragma once
#include "raylib.h"
#include "Geometry.h"

struct Flipper
{
    Flipper(Vector2 rotP, float len, Color c, int dir);
    void UpdatePhysics(float dtPhysics);
    void UpdatePositions();
    Vector2 rotPos; // rotation point
    float length;
    Vector2 tipPos;
    const float rotRadius = 15.0f;
    const float tipRadius = 10.0f;
    Circle circleRot;
    Circle circleTip;
    Line lineUp;
    Line lineDown;
    Color color;

    bool rotateUp = false;
    const float minAngle = -PI / 6.0f;
    const float maxAngle = PI / 4.0f;
    float angle = minAngle;
    float angularSpeed = 16.0f; // rad per sec
    int direction;              // 1: left flipper, -1: right flipper
};