#include "raylib.h"
#include "Player.h"
#include "Config.h"
#include "Vec2Extensions.h"

Player::Player()
{
    velocity = {0.0f, 20.0f};
    color = RED;
    position = {0.5f*Config::virtualWidth, 0.5f*Config::virtualHeight};
}

void Player::Update(float dt)
{
    position += velocity * dt;
}

void Player::Draw()
{
    DrawCircle(position.x, position.y, 20.0f, color);
}