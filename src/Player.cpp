#include "raylib.h"
#include "Player.h"

Player::Player(int gameWidth, int gameHeight)
{
    gameWidth = gameWidth;
    gameHeight = gameHeight;
    velocity = {0.0f, 0.0f};
    color = RED;
    position = {0.5f*gameWidth, 0.5f*gameHeight};
}

void Player::Update()
{
    
}

void Player::Draw()
{
    DrawCircle(position.x, position.y, 20.0f, color);
}