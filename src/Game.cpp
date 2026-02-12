#include "raylib.h"
#include <string>
#include "math.h"
#include "Game.h"
#include "Config.h"

Game::Game()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1200, 800, "Spelfönster");
    SetTargetFPS(100);
    renderTexture = LoadRenderTexture(Config::virtualWidth,
                                        Config::virtualHeight);
    
}

Game::~Game()
{
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
    player.Update(dt);
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
    DrawText(std::to_string(time).c_str(), 700, 10, 15, PURPLE);

    player.Draw();
    // -----------
    EndTextureMode();

    DrawScaledRenderTexture();
}

void Game::DrawScaledRenderTexture()
{
    BeginDrawing();
    ClearBackground(BLACK);
    float scaleX = (float)GetScreenWidth() / Config::virtualWidth;
    float scaleY = (float)GetScreenHeight() / Config::virtualHeight;
    float scale = std::fmin(scaleX, scaleY); // keep aspect ratio
    float scaledWidth = Config::virtualWidth * scale;
    float scaledHeight = Config::virtualHeight * scale;
    float offsetX = (GetScreenWidth() - scaledWidth) * 0.5f;
    float offsetY = (GetScreenHeight() - scaledHeight) * 0.5f;
    DrawTexturePro(renderTexture.texture, {0, 0, (float)renderTexture.texture.width, -(float)renderTexture.texture.height}, {offsetX, offsetY, scaledWidth, scaledHeight}, {0, 0}, 0.0f, WHITE);
    EndDrawing();
}