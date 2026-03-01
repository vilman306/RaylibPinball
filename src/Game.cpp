#include "raylib.h"
#include "raymath.h"
#include <string>
#include "math.h"
#include "Game.h"
#include "Config.h"
#include "Vec2Extensions.h"

Game::Game()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1200, 800, Config::windowTitle);
    InitAudioDevice();
    SetTargetFPS(120);
    renderTexture = LoadRenderTexture(Config::gameWidth, Config::gameHeight);
    audioManager.Load();
    ball.velocity = {20.0f, -20.0f};
}

Game::~Game()
{
    audioManager.Unload();
    CloseAudioDevice();
    CloseWindow();
}

void Game::Run()
{
    while (!WindowShouldClose())
    {
        Update();
        Draw();
    }
}

void Game::Update()
{
    float dt = GetFrameTime();

    if (IsKeyPressed(KEY_RIGHT))
        ball.velocity.x += 100.0f;
    if (IsKeyPressed(KEY_LEFT))
        ball.velocity.x -= 100.0f;
    if (IsKeyPressed(KEY_UP))
        ball.velocity.y -= 100.0f;
    if (IsKeyPressed(KEY_DOWN))
        ball.velocity.y += 100.0f;

    PhysicsEvents events = physicsManager.Update(ball, dt); // Update physics and retrieve physics information

    // Play ballBounce sound if ball collided
    if (events.ballBounce){
        float ballSpeed = Vector2Length(ball.velocity);
        float bounceVolume = ballSpeed / PhysicsManager::MAX_BALL_SPEED;   
        SetSoundVolume(audioManager.ballBounce, bounceVolume);
        PlaySound(audioManager.ballBounce);
    }

    ball.Update(dt);
}

void Game::Draw()
{
    BeginTextureMode(renderTexture);
    // Draw content:
    // -----------
    ClearBackground(WHITE);
    // Show fps
    std::string fps = std::to_string(GetFPS());
    DrawText(fps.c_str(), 10, 10, 15, PURPLE);
    // Show time
    time = GetTime();
    DrawText(std::to_string(time).c_str(), Config::gameWidth - 100, 10, 15, PURPLE);

    ball.Draw();
    // -----------
    EndTextureMode();

    DrawScaledRenderTexture();
}

void Game::DrawScaledRenderTexture()
{
    BeginDrawing();
    ClearBackground(BLACK);
    float scaleX = (float)GetScreenWidth() / Config::gameWidth;
    float scaleY = (float)GetScreenHeight() / Config::gameHeight;
    float scale = std::fmin(scaleX, scaleY); // keep aspect ratio
    float scaledWidth = Config::gameWidth * scale;
    float scaledHeight = Config::gameHeight * scale;
    float offsetX = (GetScreenWidth() - scaledWidth) * 0.5f;
    float offsetY = (GetScreenHeight() - scaledHeight) * 0.5f;
    DrawTexturePro(renderTexture.texture, {0, 0, (float)renderTexture.texture.width, -(float)renderTexture.texture.height}, {offsetX, offsetY, scaledWidth, scaledHeight}, {0, 0}, 0.0f, WHITE);
    EndDrawing();
}