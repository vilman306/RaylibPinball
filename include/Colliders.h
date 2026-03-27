#pragma once
#include "raylib.h"
#include "Vec2Extensions.h"
#include "raymath.h"
#include "Geometry.h"

struct CircleCollider
{
    CircleCollider(Vector2 position, float radius);
    Circle circle;
    void* owner = nullptr;
    enum class CircleColliderRole { Generic, FlipperRot, FlipperTip };
    CircleColliderRole role = CircleColliderRole::Generic;

    // To draw collider (for debugging)
    Color color = GRAY;
    void Draw();
};

struct LineCollider
{
    LineCollider(Vector2 pos1, Vector2 pos2);
    LineCollider(Line l);
    void UpdatePosition(Vector2 p1, Vector2 p2);
    Line line;
    void* owner = nullptr;
    enum class LineColliderRole { Generic, FlipperUp, FlipperDown };
    LineColliderRole role = LineColliderRole::Generic;

    // To draw collider (for debugging)
    float visualThickness = 2.0f;
    Color color = GRAY;
    void Draw();
};