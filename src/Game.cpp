#include "raylib.h"
#include "raymath.h"
#include <string>
#include "math.h"
#include "Game.h"
#include "Config.h"
#include "Vec2Extensions.h"
#include <iostream>
#include "Line.h"

Circle::Circle(Vector2 pos, float rad, Color c) : position(pos), radius(rad), color(c)
{

}

void Circle::Draw()
{
    
    DrawCircleV(position, radius, color);
    std::cout << position.x << "  " << position.y << "  " << radius << "  " << "\n";
}



Flipper::Flipper(Vector2 rotP, Vector2 tipP, Color c)
    : rotPos(rotP),
      tipPos(tipP),
      color(c),
      circleRot(rotP, rotRadius, c),
      circleTip(tipP, tipRadius, c),
      lineUp(),
      lineDown()
{
    Vector2 dir = Vector2Normalize(tipP - rotP);
    Vector2 normal = {dir.y, -dir.x};
    lineUp = Line(rotP + normal * rotRadius, tipP + normal * tipRadius, c);
    lineDown = Line(rotP - normal * rotRadius, tipP - normal * tipRadius, c);
}

void Flipper::Draw()
{
    lineUp.Draw();
    lineDown.Draw();
    circleRot.Draw();
    circleTip.Draw();
}

Game::Game()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1200, 800, Config::windowTitle);
    InitAudioDevice();
    SetTargetFPS(400);
    renderTexture = LoadRenderTexture(Config::gameWidth, Config::gameHeight);
    audioManager.Load();

    // Line line({Config::gameWidth / 2.0f - 150.0f, Config::gameHeight - 200.0f},
    //           {Config::gameWidth / 2.0f, Config::gameHeight - 100.0f},
    //           VIOLET);
    // lines.push_back(line);
    Flipper flipper({Config::gameWidth / 2.0f - 150.0f, Config::gameHeight - 200.0f},
                    {Config::gameWidth / 2.0f, Config::gameHeight - 100.0f},
                    VIOLET);
    flippers.push_back(flipper);
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
    
    if (IsKeyPressed(KEY_RIGHT))
        ball.velocity.x += 100.0f;
    if (IsKeyPressed(KEY_LEFT))
        ball.velocity.x -= 100.0f;
    if (IsKeyPressed(KEY_UP))
        ball.velocity.y -= 100.0f;
    if (IsKeyPressed(KEY_DOWN))
        ball.velocity.y += 100.0f;
    
    static double prevTime = GetTime();
    double time = GetTime();
    double dt = time - prevTime;

    static double dtSum = 0.0;
    dtSum += dt;
    double dtPhysics = PhysicsManager::dt;

    // // for fun
    // static double nextEventTime = time + 10.0;
    // if (time >= nextEventTime) {
    //     ball.velocity.y += 1200.0f;
    //     nextEventTime = time + 10.0;
    // }

    // Update physics
    PhysicsEvents physicsEvents;
    while (dtSum >= dtPhysics)
    {
        physicsEvents = physicsManager.Update(ball, lines);
        dtSum -= dtPhysics;
    }

    // Play ballBounce sound if ball collided
    if (physicsEvents.ballBounce)
    {
        float ballSpeed = Vector2Length(ball.velocity);
        float bounceVolume = ballSpeed / PhysicsManager::MAX_BALL_SPEED;
        float minSoundLevel = 0.01f;
        if (bounceVolume >= minSoundLevel) {
            SetSoundVolume(audioManager.ballBounce, bounceVolume);
            PlaySound(audioManager.ballBounce);
        }
    }

    // Lerp ball position between its previous and current physical positions:
    float lerpFactor = dtSum / dtPhysics;
    ball.visualPosition = Vector2Lerp(ball.prevPhysicalPosition, ball.physicalPosition, lerpFactor);
    prevTime = time;
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
    
    for (Line &line : lines)
        line.Draw();

    for (Flipper &flipper : flippers)
        flipper.Draw();

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