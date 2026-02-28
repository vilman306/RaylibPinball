#pragma once
#include "raylib.h"

inline Vector2 operator*(const Vector2& v, float s)
{
    return {v.x * s, v.y * s};
}

inline Vector2 operator+(const Vector2 &a, const Vector2& b)
{
    return {a.x + b.x, a.y + b.y};
}

inline Vector2& operator+=(Vector2 &a, const Vector2 &b)
{
    a.x += b.x;
    a.y += b.y;
    return a;
}

inline Vector2& operator*=(Vector2 &a, float b)
{
    a.x *= b;
    a.y *= b;
    return a;
}