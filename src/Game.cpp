#include "raylib.h"
#include "raymath.h"
#include <string>
#include "math.h"
#include "Game.h"
#include "Config.h"
#include "Vec2Extensions.h"
#include <iostream>
#include "Geometry.h"


Flipper::Flipper(Vector2 rotP, float len, Color c, int dir)
    : rotPos(rotP),
      length(len),
      color(c),
      direction(dir),
      circleRot(rotP, rotRadius, c),
      circleTip(),
      lineUp(),
      lineDown()
{
    Vector2 lineDir = {cosf(minAngle), direction * sinf(minAngle)};
    tipPos = rotPos + lineDir * length;

    circleTip = Circle(tipPos, tipRadius, c);

    Vector2 normal = {-direction * lineDir.y, lineDir.x};
    lineUp = Line(rotPos + normal * rotRadius, tipPos + normal * tipRadius, c);
    lineDown = Line(rotPos - normal * rotRadius, tipPos - normal * tipRadius, c);
    std::cout << normal.y << "\n";
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

    Line line({Config::gameWidth / 2.0f - 150.0f, 200.0f},
              {Config::gameWidth / 2.0f, 100.0f},
              VIOLET);
    lines.push_back(line);

    Ball ball({Config::gameWidth / 2.0f, Config::gameHeight / 2.0f}, 20.0f, {0.0f, 0.0f}, BLUE);
    balls.push_back(ball);

    Flipper flipper({Config::gameWidth / 2.0f - 150.0f, Config::gameHeight - 200.0f},
                    200.0f, VIOLET, 1);
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
        balls[0].velocity.x += 200.0f;
    if (IsKeyPressed(KEY_LEFT))
        balls[0].velocity.x -= 200.0f;
    if (IsKeyPressed(KEY_UP))
        balls[0].velocity.y += 200.0f;
    if (IsKeyPressed(KEY_DOWN))
        balls[0].velocity.y -= 200.0f;

    static double prevTime = GetTime();
    double time = GetTime();
    double dt = time - prevTime;

    static double dtSum = 0.0;
    dtSum += dt;
    double dtPhysics = PhysicsManager::dt;

    // Update physics
    std::vector<PhysicsEvents> physicsEventsPerBall;
    while (dtSum >= dtPhysics)
    {
        physicsEventsPerBall = physicsManager.Update(balls, lines);
        dtSum -= dtPhysics;
    }

    // // Play ballBounce sound if ball collided
    // if (physicsEvents.ballBounce)
    // {
    //     float ballSpeed = Vector2Length(ball.velocity);
    //     float bounceVolume = ballSpeed / PhysicsManager::MAX_BALL_SPEED;
    //     float minSoundLevel = 0.01f;
    //     if (bounceVolume >= minSoundLevel) {
    //         SetSoundVolume(audioManager.ballBounce, bounceVolume);
    //         PlaySound(audioManager.ballBounce);
    //     }
    // }

    // Lerp ball positions between its previous and current physical positions:
    float lerpFactor = dtSum / dtPhysics;
    for (Ball &ball : balls)
    {
        ball.circle.position = Vector2Lerp(ball.prevPhysicalPosition, ball.physicalPosition, lerpFactor);
    }


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

    for (Ball &ball : balls)
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