#pragma once

#include "raylib.h"
#include "Config.h"

namespace Utils
{
    Vector2 WorldToScreen(Vector2 v) {
        return (Vector2) { v.x, Config::gameHeight - v.y };
    }
}
