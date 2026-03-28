#pragma once
#include "raylib.h"
#include "Vec2Extensions.h"
#include "raymath.h"
#include "Geometry.h"
#include "PhysicsManager.h"

struct ColliderOwner
{
    PhysicsMaterial material;
    virtual ~ColliderOwner() = default;
};

struct CircleCollider
{
    CircleCollider(Vector2 position, float radius, ColliderOwner* cOwner);
    Circle circle;
    ColliderOwner* owner;
    enum class CircleColliderRole { Generic, FlipperRot, FlipperTip };
    CircleColliderRole role = CircleColliderRole::Generic;

    // To draw collider (for debugging)
    Color color = GRAY;
    void Draw();
};

struct LineCollider
{
    LineCollider(Vector2 pos1, Vector2 pos2, ColliderOwner* cOwner);
    LineCollider(Line l, ColliderOwner* cOwner);
    void UpdatePosition(Vector2 p1, Vector2 p2);
    Line line;
    ColliderOwner* owner;
    enum class LineColliderRole { Generic, FlipperUp, FlipperDown };
    LineColliderRole role = LineColliderRole::Generic;

    // To draw collider (for debugging)
    float visualThickness = 2.0f;
    Color color = GRAY;
    void Draw();
};