#include <vector>
#include "raylib.h"
#include "raymath.h"
#include "Bird.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

int main(void)
{
    const int screenWidth = 1920;
    const int screenHeight = 1080;

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "Bird");
    SetTargetFPS(120);

    std::vector<Bird> birds;
    std::srand(static_cast<unsigned>(std::time(nullptr)));


    while (!WindowShouldClose())
    {
        Bird::setRaylibConfig(RaylibConfig{ screenWidth, screenHeight, 200, 100, GetFrameTime() });
        Bird::setSimulationConfig(SimulationConfig{ 80, 0.2f, 100, 0.075f, 100, 0.1f, 20.0f, 500.0f, 100.0f });

        // Create bird
        Vector2 mouse = GetMousePosition();
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) || IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        {
            const float speedScale = 300.0;
            float randomFloat1 = -1.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2.0f));
            float randomFloat2 = -1.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2.0f));
            Vector2 initVelocity = Vector2Scale(Vector2Normalize(Vector2{ randomFloat1, randomFloat2 }), speedScale);
            Bird b{ mouse, initVelocity };
            birds.push_back(b);
        };

        // Draw birds
        BeginDrawing();
        ClearBackground(RAYWHITE);
        for (Bird &b : birds)
        {
            Vector2 pos = b.getPosition();
            Vector2 vel = b.getVelocity();

            float angle = atan2(vel.y, vel.x);
            const float triangleSize = 10.0f; // control size
            const float triangleAngle = 7 * PI / 9; // control shape
            Vector2 base = Vector2Scale(Vector2{ cos(angle), sin(angle) }, triangleSize);
            Vector2 vertex1 = Vector2Add(pos, base);
            Vector2 vertex2 = Vector2Add(pos, Vector2Rotate(base, -triangleAngle));
            Vector2 vertex3 = Vector2Add(pos, Vector2Rotate(base, triangleAngle));
            DrawTriangle(vertex1, vertex2, vertex3, DARKGREEN);
            b.update(birds);
        }
        DrawFPS(30, 30);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}